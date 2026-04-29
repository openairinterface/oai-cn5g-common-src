/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "http_client.hpp"

#include <nghttp2/asio_http2_client.h>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/steady_timer.hpp>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// POSIX thread naming (Linux)
#include <pthread.h>

namespace json = nlohmann;
using namespace oai::http;

// ============================================================================
// Internal helpers (file-scope only)
// ============================================================================

namespace oai::http::internal {

// ---------------------------------------------------------------------------
// method_to_string
// ---------------------------------------------------------------------------
inline std::string method_to_string(const method_e& method) {
  switch (method) {
    case method_e::GET:
      return "GET";
    case method_e::POST:
      return "POST";
    case method_e::PUT:
      return "PUT";
    case method_e::PATCH:
      return "PATCH";
    case method_e::DELETE:
      return "DELETE";
    case method_e::OPTIONS:
      return "OPTIONS";
    default:
      return "UNKNOWN";
  }
}

// ---------------------------------------------------------------------------
// URI parsing
// ---------------------------------------------------------------------------
struct uri_components {
  std::string scheme;  // "http" or "https"
  std::string host;
  std::string port;  // e.g. "80", "443", "8080"
  std::string path;  // "/path?query" — always starts with '/'
};

inline uri_components parse_uri(const std::string& uri) {
  uri_components out;
  const auto scheme_end = uri.find("://");
  if (scheme_end == std::string::npos)
    throw std::invalid_argument("bad URI (no scheme): " + uri);

  out.scheme       = uri.substr(0, scheme_end);
  std::string rest = uri.substr(scheme_end + 3);
  const auto slash = rest.find('/');
  std::string authority =
      (slash == std::string::npos) ? rest : rest.substr(0, slash);
  out.path = (slash == std::string::npos) ? "/" : rest.substr(slash);

  // Handle IPv6 addresses: [::1]:port
  const auto colon = authority.rfind(':');
  if (colon == std::string::npos || authority[0] == '[') {
    out.host = authority;
    out.port = (out.scheme == "https") ? "443" : "80";
  } else {
    out.host = authority.substr(0, colon);
    out.port = authority.substr(colon + 1);
  }
  return out;
}

// ---------------------------------------------------------------------------
// response_accumulator — accumulates one in-flight response
// ---------------------------------------------------------------------------
struct response_accumulator {
  int status_code{0};
  std::string body;
  std::map<std::string, std::string> headers;
  std::shared_ptr<std::promise<response>> prom;
};

// ============================================================================
// nghttp2_session
// ============================================================================
class nghttp2_session {
 public:
  static constexpr int MAX_CONCURRENT_STREAMS = 1000;

  // Plain TCP
  nghttp2_session(
      boost::asio::io_service& io_service, const std::string& host,
      const std::string& port);

  // TLS
  nghttp2_session(
      boost::asio::io_service& io_service, boost::asio::ssl::context& tls_ctx,
      const std::string& host, const std::string& port);

  ~nghttp2_session() = default;

  nghttp2_session(const nghttp2_session&) = delete;
  nghttp2_session& operator=(const nghttp2_session&) = delete;

  bool is_available() const noexcept {
    return m_connected.load(std::memory_order_acquire) &&
           !m_errored.load(std::memory_order_acquire) &&
           m_active_streams.load(std::memory_order_relaxed) <
               MAX_CONCURRENT_STREAMS;
  }

  bool connected() const noexcept {
    return m_connected.load(std::memory_order_acquire);
  }
  bool errored() const noexcept {
    return m_errored.load(std::memory_order_acquire);
  }
  int active_streams() const noexcept {
    return m_active_streams.load(std::memory_order_relaxed);
  }
  const std::string& host() const { return m_host; }
  const std::string& port() const { return m_port; }

  // MUST be called from an io_service thread
  void submit(
      const std::string& method, const std::string& full_uri,
      const std::map<std::string, std::string>& req_headers,
      const std::string& body, std::shared_ptr<std::promise<response>> prom,
      boost::asio::io_service& io_service, int timeout_ms);

  void shutdown() {
    if (m_session) m_session->shutdown();
  }

 private:
  void setup_session_callbacks();

  std::unique_ptr<nghttp2::asio_http2::client::session> m_session;
  std::string m_host;
  std::string m_port;
  std::atomic<bool> m_connected{false};
  std::atomic<bool> m_errored{false};
  std::atomic<int> m_active_streams{0};
};

nghttp2_session::nghttp2_session(
    boost::asio::io_service& io_service, const std::string& host,
    const std::string& port)
    : m_host(host), m_port(port) {
  auto connect_timeout = boost::posix_time::seconds(10);
  m_session            = std::make_unique<nghttp2::asio_http2::client::session>(
      io_service, host, port, connect_timeout);
  setup_session_callbacks();
}

nghttp2_session::nghttp2_session(
    boost::asio::io_service& io_service, boost::asio::ssl::context& tls_ctx,
    const std::string& host, const std::string& port)
    : m_host(host), m_port(port) {
  auto connect_timeout = boost::posix_time::seconds(10);
  m_session            = std::make_unique<nghttp2::asio_http2::client::session>(
      io_service, tls_ctx, host, port, connect_timeout);
  setup_session_callbacks();
}

void nghttp2_session::setup_session_callbacks() {
  m_session->on_connect(
      [this](boost::asio::ip::tcp::resolver::iterator /*ep*/) {
        m_connected.store(true, std::memory_order_release);
      });

  m_session->on_error([this](const boost::system::error_code& /*ec*/) {
    m_errored.store(true, std::memory_order_release);
    m_connected.store(false, std::memory_order_release);
  });
}

void nghttp2_session::submit(
    const std::string& method, const std::string& full_uri,
    const std::map<std::string, std::string>& req_headers,
    const std::string& body, std::shared_ptr<std::promise<response>> prom,
    boost::asio::io_service& io_service, int timeout_ms) {
  boost::asio::dispatch(
      io_service, [this, method, full_uri, req_headers, body, prom, timeout_ms,
                   io_svc = std::ref(io_service)]() mutable {
        // Build nghttp2 header map — HTTP/2 requires lowercase names
        nghttp2::asio_http2::header_map h2_headers;
        for (const auto& [k, v] : req_headers) {
          std::string lk = k;
          std::transform(lk.begin(), lk.end(), lk.begin(), [](unsigned char c) {
            return std::tolower(c);
          });
          h2_headers.emplace(lk, nghttp2::asio_http2::header_value{v, false});
        }
        // Default headers (already lowercase)
        if (h2_headers.find("accept") == h2_headers.end())
          h2_headers.emplace(
              "accept",
              nghttp2::asio_http2::header_value{"application/json", false});
        if (h2_headers.find("expect") == h2_headers.end())
          h2_headers.emplace(
              "expect", nghttp2::asio_http2::header_value{"", false});

        auto acc  = std::make_shared<response_accumulator>();
        acc->prom = prom;

        auto timer = std::make_shared<boost::asio::steady_timer>(io_svc.get());
        timer->expires_from_now(std::chrono::milliseconds(timeout_ms));

        boost::system::error_code ec;
        const nghttp2::asio_http2::client::request* req_ptr = nullptr;

        if (body.empty()) {
          req_ptr =
              m_session->submit(ec, method, full_uri, std::move(h2_headers));
        } else {
          req_ptr = m_session->submit(
              ec, method, full_uri, body, std::move(h2_headers));
        }

        if (!req_ptr || ec) {
          m_errored.store(true, std::memory_order_release);
          try {
            prom->set_value(
                {503, "session::submit error: " + ec.message(), {}});
          } catch (...) {
          }
          return;
        }

        m_active_streams.fetch_add(1, std::memory_order_relaxed);

        // Timeout handler
        timer->async_wait(
            [req_ptr, acc, this](const boost::system::error_code& tec) {
              if (tec == boost::asio::error::operation_aborted) return;
              req_ptr->cancel(NGHTTP2_CANCEL);
              try {
                acc->prom->set_value({408, "Request timeout", {}});
              } catch (...) {
              }
              m_active_streams.fetch_sub(1, std::memory_order_relaxed);
            });

        // Response header handler
        req_ptr->on_response(
            [acc](const nghttp2::asio_http2::client::response& res) {
              acc->status_code = res.status_code();
              for (const auto& [k, v] : res.header()) {
                acc->headers[k] = v.value;
              }
              res.on_data([acc](const uint8_t* data, std::size_t len) {
                if (len > 0)
                  acc->body.append(reinterpret_cast<const char*>(data), len);
              });
            });

        // Stream close handler
        req_ptr->on_close([acc, timer, this](uint32_t error_code) {
          timer->cancel();

          // Only decrement if timeout did NOT cancel the stream first
          // (timeout path already calls fetch_sub).
          const bool timed_out = (error_code == NGHTTP2_CANCEL);
          if (!timed_out) {
            m_active_streams.fetch_sub(1, std::memory_order_relaxed);
          }

          if (error_code == 0) {
            try {
              acc->prom->set_value({acc->status_code, acc->body, acc->headers});
            } catch (...) {
            }
          } else if (timed_out) {
            try {
              acc->prom->set_value({408, "Request timeout", {}});
            } catch (...) {
            }
          } else if (error_code == NGHTTP2_ENHANCE_YOUR_CALM) {
            try {
              acc->prom->set_value({429, "Rate limited", {}});
            } catch (...) {
            }
          } else {
            try {
              acc->prom->set_value(
                  {503, "Stream error: " + std::to_string(error_code), {}});
            } catch (...) {
            }
          }
        });
      });
}

// ============================================================================
// nghttp2_connection_pool
// ============================================================================
struct pool_key {
  std::string host;
  std::string port;
  bool tls;
  bool operator==(const pool_key& o) const noexcept {
    return host == o.host && port == o.port && tls == o.tls;
  }
};

struct pool_key_hash {
  std::size_t operator()(const pool_key& k) const noexcept {
    std::size_t h = std::hash<std::string>{}(k.host);
    h ^= std::hash<std::string>{}(k.port) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<bool>{}(k.tls) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
  }
};

class nghttp2_connection_pool {
 public:
  explicit nghttp2_connection_pool(
      boost::asio::io_service& io_service, bool enable_tls,
      int max_sessions_per_host = 10);

  ~nghttp2_connection_pool() = default;

  std::shared_ptr<nghttp2_session> get_or_create(
      const std::string& host, const std::string& port, bool tls);

  void shutdown_all();

 private:
  std::shared_ptr<nghttp2_session> make_session(const pool_key& key);

  boost::asio::io_service& m_io_service;
  bool m_enable_tls;
  int m_max_per_host;
  boost::asio::ssl::context m_ssl_ctx;

  mutable std::mutex m_mutex;
  std::unordered_map<
      pool_key, std::vector<std::shared_ptr<nghttp2_session>>, pool_key_hash>
      m_pool;
};

nghttp2_connection_pool::nghttp2_connection_pool(
    boost::asio::io_service& io_service, bool enable_tls,
    int max_sessions_per_host)
    : m_io_service(io_service),
      m_enable_tls(enable_tls),
      m_max_per_host(max_sessions_per_host),
      m_ssl_ctx(boost::asio::ssl::context::tls_client) {
  if (enable_tls) {
    m_ssl_ctx.set_options(
        boost::asio::ssl::context::default_workarounds |
        boost::asio::ssl::context::no_sslv2 |
        boost::asio::ssl::context::no_sslv3 |
        boost::asio::ssl::context::no_tlsv1 |
        boost::asio::ssl::context::no_tlsv1_1);

    // verify_none mirrors the former CPR behaviour (VerifyPeer=false).
    m_ssl_ctx.set_verify_mode(boost::asio::ssl::verify_none);

    boost::system::error_code ec;
    nghttp2::asio_http2::client::configure_tls_context(ec, m_ssl_ctx);
    if (ec)
      throw std::runtime_error("NgHttp2 TLS configure failed: " + ec.message());
  }
}

std::shared_ptr<nghttp2_session> nghttp2_connection_pool::get_or_create(
    const std::string& host, const std::string& port, bool tls) {
  pool_key key{host, port, tls};
  std::lock_guard<std::mutex> lk(m_mutex);

  auto& vec = m_pool[key];

  // Evict errored sessions
  vec.erase(
      std::remove_if(
          vec.begin(), vec.end(),
          [](const std::shared_ptr<nghttp2_session>& s) {
            return s->errored();
          }),
      vec.end());

  // Return first available session
  for (auto& s : vec) {
    if (s->is_available()) return s;
  }

  // Create new session if under limit
  if (static_cast<int>(vec.size()) < m_max_per_host) {
    auto ns = make_session(key);
    vec.push_back(ns);
    return ns;
  }

  // All at capacity: return least-loaded
  auto best = std::min_element(
      vec.begin(), vec.end(),
      [](const std::shared_ptr<nghttp2_session>& a,
         const std::shared_ptr<nghttp2_session>& b) {
        return a->active_streams() < b->active_streams();
      });
  return *best;
}

std::shared_ptr<nghttp2_session> nghttp2_connection_pool::make_session(
    const pool_key& key) {
  if (key.tls) {
    return std::make_shared<nghttp2_session>(
        m_io_service, m_ssl_ctx, key.host, key.port);
  }
  return std::make_shared<nghttp2_session>(m_io_service, key.host, key.port);
}

void nghttp2_connection_pool::shutdown_all() {
  std::lock_guard<std::mutex> lk(m_mutex);
  for (auto& [key, vec] : m_pool) {
    for (auto& s : vec) s->shutdown();
  }
  m_pool.clear();
}

}  // namespace oai::http::internal

// ============================================================================
// http_client_impl (PIMPL body — visible only to this translation unit)
// ============================================================================
namespace oai::http {

class http_client_impl {
 public:
  http_client_impl(
      oai::logger::printf_logger logger, int timeout_ms,
      const std::string& interface_name, uint8_t http_version, bool enable_tls);

  ~http_client_impl();

  response send_simple(const method_e& method, const request& req);

  static request build_json_request(
      const std::string& uri, const std::string& body,
      const std::string& content_type);

  static request build_multipart_request(
      const std::string& uri, const std::string& body);

 private:
  oai::logger::printf_logger m_logger;
  int m_timeout_ms;
  std::string m_interface;
  uint8_t m_http_version;
  bool m_enable_tls;

  // io_service held via shared_ptr so lambdas remain valid after destruction
  std::shared_ptr<boost::asio::io_service> m_io_service;
  std::unique_ptr<boost::asio::io_service::work> m_work_guard;
  std::vector<std::thread> m_io_threads;

  std::unique_ptr<internal::nghttp2_connection_pool> m_pool;
};

http_client_impl::http_client_impl(
    oai::logger::printf_logger logger, int timeout_ms,
    const std::string& interface_name, uint8_t http_version, bool enable_tls)
    : m_logger(std::move(logger)),
      m_timeout_ms(timeout_ms),
      m_interface(interface_name),
      m_http_version(http_version),
      m_enable_tls(enable_tls),
      m_io_service(std::make_shared<boost::asio::io_service>()),
      m_work_guard(
          std::make_unique<boost::asio::io_service::work>(*m_io_service)),
      m_pool(std::make_unique<internal::nghttp2_connection_pool>(
          *m_io_service, enable_tls)) {
  const unsigned int n_threads =
      std::max(4u, std::min(16u, std::thread::hardware_concurrency()));

  m_logger.info(
      "NgHttp2 HTTP client starting io_service with %u threads, "
      "timeout=%d ms, TLS=%s",
      n_threads, timeout_ms, enable_tls ? "enabled" : "disabled");

  m_io_threads.reserve(n_threads);
  for (unsigned int i = 0; i < n_threads; ++i) {
    m_io_threads.emplace_back([this]() {
      pthread_setname_np(pthread_self(), "oai-http-io");
      try {
        m_io_service->run();
      } catch (const std::exception& e) {
        m_logger.error("io_service thread exception: %s", e.what());
      }
    });
  }
}

http_client_impl::~http_client_impl() {
  m_pool->shutdown_all();
  m_work_guard.reset();
  m_io_service->stop();
  for (auto& t : m_io_threads) {
    if (t.joinable()) t.join();
  }
}

response http_client_impl::send_simple(
    const method_e& method, const request& req) {
  m_logger.debug(
      "NgHttp2 send_simple: %s %s", internal::method_to_string(method).c_str(),
      req.uri.c_str());

  internal::uri_components uri_parts;
  try {
    uri_parts = internal::parse_uri(req.uri);
  } catch (const std::exception& e) {
    m_logger.error("URI parse error: %s", e.what());
    return {400, "Invalid URI: " + req.uri, {}};
  }

  const bool use_tls = (uri_parts.scheme == "https");

  std::shared_ptr<internal::nghttp2_session> session;
  try {
    session = m_pool->get_or_create(uri_parts.host, uri_parts.port, use_tls);
  } catch (const std::exception& e) {
    m_logger.error("Connection pool error: %s", e.what());
    return {503, "Connection pool error", {}};
  }

  // Merge caller headers with defaults (caller headers take priority)
  std::map<std::string, std::string> merged = req.headers;
  merged.insert({"accept", "application/json"});
  merged.insert({"expect", ""});

  auto prom                    = std::make_shared<std::promise<response>>();
  std::future<response> fut    = prom->get_future();
  const std::string method_str = internal::method_to_string(method);
  session->submit(
      method_str, req.uri, merged, req.body, prom, *m_io_service, m_timeout_ms);

  const auto status =
      fut.wait_for(std::chrono::milliseconds(m_timeout_ms + 100));
  if (status == std::future_status::timeout) {
    m_logger.warn(
        "NgHttp2 client future timed out for URI: %s", req.uri.c_str());
    return {408, "HTTP client future timeout", {}};
  }

  try {
    return fut.get();
  } catch (const std::exception& e) {
    m_logger.error("NgHttp2 future exception: %s", e.what());
    return {503, e.what(), {}};
  }
}

request http_client_impl::build_json_request(
    const std::string& uri, const std::string& body,
    const std::string& content_type) {
  request req;
  req.uri = uri;
  if (json::json::accept(body)) {
    req.body = body;
    req.headers.insert({"content-type", content_type});
  }
  return req;
}

request http_client_impl::build_multipart_request(
    const std::string& uri, const std::string& body) {
  request req;
  req.uri  = uri;
  req.body = body;
  req.headers.insert(
      {"content-type",
       "multipart/related;boundary=" + std::string(MIME_BOUNDARY)});
  return req;
}

}  // namespace oai::http

// ============================================================================
// http_client — thin wrapper delegating to http_client_impl
// ============================================================================
namespace oai::http {

http_client::http_client(
    oai::logger::printf_logger logger, int timeout_ms,
    const std::string& interface, uint8_t http_version, bool enable_tls,
    request_type_e request_type)
    : m_sbi_logger(logger),
      m_timeout_ms(timeout_ms),
      m_interface(interface),
      m_http_version(http_version),
      m_request_type(request_type),
      m_enable_tls(enable_tls),
      m_public_key_path(std::nullopt),
      m_impl(std::make_unique<http_client_impl>(
          logger, timeout_ms, interface, http_version, enable_tls)) {}

http_client::~http_client() {
  m_sbi_logger.info("Delete HTTP client instance");
}

std::shared_ptr<http_client> http_client::create_instance(
    const oai::logger::printf_logger& logger, int timeout_ms,
    const std::string& interface, uint8_t http_version, bool enable_tls,
    request_type_e request_type) {
  if (!instance) {
    instance = std::make_shared<http_client>(
        logger, timeout_ms, interface, http_version, enable_tls, request_type);
  }
  return instance;
}

response http_client::send_http_request(
    const method_e& method, const request& req) {
  return m_impl->send_simple(method, req);
}

request http_client::prepare_json_request(
    const std::string& uri, const std::string& body,
    const std::string& content_type) {
  return http_client_impl::build_json_request(uri, body, content_type);
}

request http_client::prepare_multipart_request(
    const std::string& uri, const std::string& body) {
  return http_client_impl::build_multipart_request(uri, body);
}

}  // namespace oai::http
