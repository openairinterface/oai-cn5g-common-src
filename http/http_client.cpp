/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "http_client.hpp"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/steady_timer.hpp>
#include <nghttp2/nghttp2.h>
#include <openssl/ssl.h>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstring>
#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <variant>
#include <vector>

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
  std::string scheme;
  std::string host;
  std::string port;
  std::string path;
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
  // Fields for request body provider (set only for POST/PUT/PATCH):
  std::string request_body;
  std::size_t body_offset{0};
  // Timer stored here so on_stream_close_cb can cancel it:
  std::shared_ptr<boost::asio::steady_timer> timer;
};

// ============================================================================
// h2_session — wraps one HTTP/2 connection using nghttp2 C API
// ============================================================================
class h2_session : public std::enable_shared_from_this<h2_session> {
 public:
  static constexpr int MAX_CONCURRENT_STREAMS = 1000;
  static constexpr int CONNECT_TIMEOUT_S      = 10;
  static constexpr size_t READ_BUFFER_SIZE    = 16384;

  // Plain TCP constructor
  h2_session(
      boost::asio::io_service& io_service, const std::string& host,
      const std::string& port);

  // TLS constructor
  h2_session(
      boost::asio::io_service& io_service, boost::asio::ssl::context& tls_ctx,
      const std::string& host, const std::string& port);

  ~h2_session();

  h2_session(const h2_session&) = delete;
  h2_session& operator=(const h2_session&) = delete;

  // Called once after make_shared<> — safe to use shared_from_this()
  void start();

  bool is_available() const noexcept;
  bool connected() const noexcept;
  bool errored() const noexcept;
  int active_streams() const noexcept;
  const std::string& host() const { return host_; }
  const std::string& port() const { return port_; }

  void submit(
      const std::string& method, const std::string& full_uri,
      const std::map<std::string, std::string>& req_headers,
      const std::string& body, std::shared_ptr<std::promise<response>> prom,
      int timeout_ms);

  void shutdown();

 private:
  using plain_socket_t = boost::asio::ip::tcp::socket;
  using tls_socket_t   = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;

  void do_resolve();
  void do_connect(boost::asio::ip::tcp::resolver::results_type endpoints);
  void do_tls_handshake();
  void on_connected();
  void do_read();
  void do_write();
  void pump_send();

  void submit_internal(
      const std::string& method, const std::string& full_uri,
      const std::map<std::string, std::string>& req_headers,
      const std::string& body, std::shared_ptr<std::promise<response>> prom,
      int timeout_ms);

  void fail_all(const std::string& reason);
  void initialize_session();

  boost::asio::ip::tcp::socket& tcp_layer();

  static int on_header_cb(
      nghttp2_session*, const nghttp2_frame*, const uint8_t* name,
      size_t namelen, const uint8_t* value, size_t valuelen, uint8_t flags,
      void* user_data);

  static int on_data_chunk_recv_cb(
      nghttp2_session*, uint8_t flags, int32_t stream_id, const uint8_t* data,
      size_t len, void* user_data);

  static int on_stream_close_cb(
      nghttp2_session*, int32_t stream_id, uint32_t error_code,
      void* user_data);

  static nghttp2_ssize body_read_cb(
      nghttp2_session*, int32_t stream_id, uint8_t* buf, size_t length,
      uint32_t* data_flags, nghttp2_data_source* source, void* user_data);

  struct pending_request {
    std::string method;
    std::string full_uri;
    std::map<std::string, std::string> req_headers;
    std::string body;
    std::shared_ptr<std::promise<response>> prom;
    int timeout_ms;
  };

  boost::asio::io_service& io_service_;
  boost::asio::io_service::strand strand_;
  std::variant<plain_socket_t, tls_socket_t> socket_;
  boost::asio::ip::tcp::resolver resolver_;
  boost::asio::steady_timer connect_timer_;

  std::string host_;
  std::string port_;
  bool is_tls_{false};

  nghttp2_session* session_ = nullptr;

  std::vector<uint8_t> send_buf_;
  std::array<uint8_t, READ_BUFFER_SIZE> recv_buf_;
  bool write_in_progress_{false};

  std::unordered_map<int32_t, std::shared_ptr<response_accumulator>> streams_;
  std::vector<pending_request> pending_;

  std::atomic<bool> m_connected{false};
  std::atomic<bool> m_errored{false};
  std::atomic<int> m_active_streams{0};
};

// ============================================================================
// h2_session — constructors, destructor, start
// ============================================================================

boost::asio::ip::tcp::socket& h2_session::tcp_layer() {
  return std::visit(
      [](auto& s) -> boost::asio::ip::tcp::socket& {
        if constexpr (std::is_same_v<std::decay_t<decltype(s)>, tls_socket_t>)
          return s.next_layer();
        else
          return s;
      },
      socket_);
}

h2_session::h2_session(
    boost::asio::io_service& io, const std::string& host,
    const std::string& port)
    : io_service_(io),
      strand_(io),
      socket_(plain_socket_t(io)),
      resolver_(io),
      connect_timer_(io),
      host_(host),
      port_(port),
      is_tls_(false) {}

h2_session::h2_session(
    boost::asio::io_service& io, boost::asio::ssl::context& tls_ctx,
    const std::string& host, const std::string& port)
    : io_service_(io),
      strand_(io),
      socket_(tls_socket_t(io, tls_ctx)),
      resolver_(io),
      connect_timer_(io),
      host_(host),
      port_(port),
      is_tls_(true) {}

h2_session::~h2_session() {
  if (session_) {
    nghttp2_session_del(session_);
    session_ = nullptr;
  }
}

void h2_session::start() {
  do_resolve();
}

// ============================================================================
// h2_session — Connection chain
// ============================================================================

void h2_session::do_resolve() {
  auto self = shared_from_this();
  connect_timer_.expires_from_now(std::chrono::seconds(CONNECT_TIMEOUT_S));
  connect_timer_.async_wait(boost::asio::bind_executor(
      strand_, [this, self](boost::system::error_code ec) {
        if (ec == boost::asio::error::operation_aborted) return;
        fail_all("connect timeout");
      }));

  resolver_.async_resolve(
      host_, port_,
      boost::asio::bind_executor(
          strand_, [this, self](
                       boost::system::error_code ec,
                       boost::asio::ip::tcp::resolver::results_type results) {
            if (ec) {
              fail_all("resolve failed: " + ec.message());
              return;
            }
            do_connect(results);
          }));
}

void h2_session::do_connect(
    boost::asio::ip::tcp::resolver::results_type endpoints) {
  auto self = shared_from_this();
  boost::asio::async_connect(
      tcp_layer(), endpoints,
      boost::asio::bind_executor(
          strand_, [this, self](
                       boost::system::error_code ec,
                       const boost::asio::ip::tcp::endpoint&) {
            if (ec) {
              fail_all("connect failed: " + ec.message());
              return;
            }
            if (is_tls_)
              do_tls_handshake();
            else
              on_connected();
          }));
}

void h2_session::do_tls_handshake() {
  auto& tls_sock = std::get<tls_socket_t>(socket_);
  if (SSL_set_tlsext_host_name(tls_sock.native_handle(), host_.c_str()) != 1) {
    fail_all("SSL_set_tlsext_host_name failed for host: " + host_);
    return;
  }

  auto self = shared_from_this();
  tls_sock.async_handshake(
      boost::asio::ssl::stream_base::client,
      boost::asio::bind_executor(
          strand_, [this, self](boost::system::error_code ec) {
            if (ec) {
              fail_all("TLS handshake failed: " + ec.message());
              return;
            }
            auto& ssl_sock             = std::get<tls_socket_t>(socket_);
            const unsigned char* proto = nullptr;
            unsigned int proto_len     = 0;
            SSL_get0_alpn_selected(
                ssl_sock.native_handle(), &proto, &proto_len);
            if (proto == nullptr || proto_len != 2 ||
                memcmp(proto, "h2", 2) != 0) {
              fail_all("ALPN negotiation did not select h2");
              return;
            }
            on_connected();
          }));
}

void h2_session::on_connected() {
  connect_timer_.cancel();
  initialize_session();

  if (m_errored.load(std::memory_order_acquire)) return;

  m_connected.store(true, std::memory_order_release);

  for (auto& req : pending_) {
    submit_internal(
        req.method, req.full_uri, req.req_headers, req.body, req.prom,
        req.timeout_ms);
  }
  pending_.clear();

  pump_send();
  do_read();
}

// ============================================================================
// h2_session — nghttp2 session initialization
// ============================================================================

void h2_session::initialize_session() {
  nghttp2_option* opt = nullptr;
  if (nghttp2_option_new(&opt) != 0) {
    fail_all("nghttp2_option_new failed");
    return;
  }
  nghttp2_option_set_no_auto_window_update(opt, 1);

  nghttp2_session_callbacks* cbs = nullptr;
  if (nghttp2_session_callbacks_new(&cbs) != 0) {
    nghttp2_option_del(opt);
    fail_all("nghttp2_session_callbacks_new failed");
    return;
  }
  nghttp2_session_callbacks_set_on_header_callback(cbs, on_header_cb);
  nghttp2_session_callbacks_set_on_data_chunk_recv_callback(
      cbs, on_data_chunk_recv_cb);
  nghttp2_session_callbacks_set_on_stream_close_callback(
      cbs, on_stream_close_cb);

  int rv = nghttp2_session_client_new3(
      &session_, cbs, /*user_data=*/this, opt, /*mem=*/nullptr);

  nghttp2_session_callbacks_del(cbs);
  nghttp2_option_del(opt);

  if (rv != 0) {
    fail_all("nghttp2_session_client_new3 failed: " + std::to_string(rv));
    return;
  }

  nghttp2_settings_entry iv[] = {
      {NGHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS,
       static_cast<uint32_t>(MAX_CONCURRENT_STREAMS)},
      {NGHTTP2_SETTINGS_INITIAL_WINDOW_SIZE, 65535},
  };
  rv = nghttp2_submit_settings(session_, NGHTTP2_FLAG_NONE, iv, 2);
  if (rv != 0) {
    fail_all("nghttp2_submit_settings failed: " + std::to_string(rv));
    return;
  }
}

// ============================================================================
// h2_session — nghttp2 static callbacks
// ============================================================================

/*static*/ int h2_session::on_header_cb(
    nghttp2_session* /*session*/, const nghttp2_frame* frame,
    const uint8_t* name, size_t namelen, const uint8_t* value, size_t valuelen,
    uint8_t /*flags*/, void* user_data) {
  auto* self = static_cast<h2_session*>(user_data);
  auto it    = self->streams_.find(frame->hd.stream_id);
  if (it == self->streams_.end()) return 0;
  auto& acc = it->second;

  if (namelen == 7 && memcmp(name, ":status", 7) == 0) {
    std::string s(reinterpret_cast<const char*>(value), valuelen);
    try {
      acc->status_code = std::stoi(s);
    } catch (...) {
      acc->status_code = 0;
    }
  } else {
    if (namelen > 0 && name[0] != ':') {
      acc->headers.emplace(
          std::string(reinterpret_cast<const char*>(name), namelen),
          std::string(reinterpret_cast<const char*>(value), valuelen));
    }
  }
  return 0;
}

/*static*/ int h2_session::on_data_chunk_recv_cb(
    nghttp2_session* session, uint8_t /*flags*/, int32_t stream_id,
    const uint8_t* data, size_t len, void* user_data) {
  auto* self = static_cast<h2_session*>(user_data);
  auto it    = self->streams_.find(stream_id);
  if (it != self->streams_.end()) {
    it->second->body.append(reinterpret_cast<const char*>(data), len);
  }
  nghttp2_session_consume(session, stream_id, len);
  return 0;
}

/*static*/ int h2_session::on_stream_close_cb(
    nghttp2_session* /*session*/, int32_t stream_id, uint32_t error_code,
    void* user_data) {
  auto* self = static_cast<h2_session*>(user_data);
  auto it    = self->streams_.find(stream_id);
  if (it == self->streams_.end()) return 0;

  auto acc = it->second;
  self->streams_.erase(it);
  self->m_active_streams.fetch_sub(1, std::memory_order_relaxed);

  if (acc->timer) acc->timer->cancel();

  if (error_code == NGHTTP2_NO_ERROR) {
    try {
      acc->prom->set_value({acc->status_code, acc->body, acc->headers});
    } catch (...) {
    }
  } else if (error_code == NGHTTP2_CANCEL) {
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
  return 0;
}

/*static*/ nghttp2_ssize h2_session::body_read_cb(
    nghttp2_session* /*session*/, int32_t /*stream_id*/, uint8_t* buf,
    size_t length, uint32_t* data_flags, nghttp2_data_source* source,
    void* /*user_data*/) {
  auto* acc        = static_cast<response_accumulator*>(source->ptr);
  size_t remaining = acc->request_body.size() - acc->body_offset;
  size_t n         = std::min(length, remaining);

  if (n > 0) {
    memcpy(buf, acc->request_body.data() + acc->body_offset, n);
    acc->body_offset += n;
  }
  if (acc->body_offset >= acc->request_body.size()) {
    *data_flags |= NGHTTP2_DATA_FLAG_EOF;
  }
  return static_cast<nghttp2_ssize>(n);
}

// ============================================================================
// h2_session — I/O loop
// ============================================================================

void h2_session::pump_send() {
  if (!session_) return;

  while (nghttp2_session_want_write(session_)) {
    const uint8_t* data = nullptr;
    nghttp2_ssize len   = nghttp2_session_mem_send2(session_, &data);

    if (len < 0) {
      fail_all("nghttp2_session_mem_send2 error: " + std::to_string(len));
      return;
    }
    if (len == 0) break;
    send_buf_.insert(send_buf_.end(), data, data + len);
  }

  if (!send_buf_.empty() && !write_in_progress_) {
    do_write();
  }
}

void h2_session::do_read() {
  auto self = shared_from_this();
  std::visit(
      [&](auto& sock) {
        sock.async_read_some(
            boost::asio::buffer(recv_buf_),
            boost::asio::bind_executor(
                strand_,
                [this, self](boost::system::error_code ec, std::size_t bytes) {
                  if (ec) {
                    fail_all("read error: " + ec.message());
                    return;
                  }
                  if (!session_) return;

                  nghttp2_ssize rv = nghttp2_session_mem_recv2(
                      session_, recv_buf_.data(), bytes);

                  if (rv < 0) {
                    fail_all(
                        "nghttp2_session_mem_recv2 error: " +
                        std::to_string(rv));
                    return;
                  }

                  pump_send();
                  do_read();
                }));
      },
      socket_);
}

void h2_session::do_write() {
  write_in_progress_ = true;
  auto self          = shared_from_this();

  std::visit(
      [&](auto& sock) {
        boost::asio::async_write(
            sock, boost::asio::buffer(send_buf_),
            boost::asio::bind_executor(
                strand_,
                [this, self](boost::system::error_code ec, std::size_t) {
                  write_in_progress_ = false;
                  if (ec) {
                    fail_all("write error: " + ec.message());
                    return;
                  }
                  send_buf_.clear();
                  pump_send();
                }));
      },
      socket_);
}

// ============================================================================
// h2_session — submit
// ============================================================================

void h2_session::submit(
    const std::string& method, const std::string& full_uri,
    const std::map<std::string, std::string>& req_headers,
    const std::string& body, std::shared_ptr<std::promise<response>> prom,
    int timeout_ms) {
  auto self = shared_from_this();
  boost::asio::dispatch(
      strand_, [this, self, method, full_uri, req_headers, body, prom,
                timeout_ms]() mutable {
        if (m_errored.load(std::memory_order_acquire)) {
          try {
            prom->set_value({503, "Session errored", {}});
          } catch (...) {
          }
          return;
        }
        if (!m_connected.load(std::memory_order_acquire)) {
          pending_.push_back(
              {method, full_uri, req_headers, body, prom, timeout_ms});
          return;
        }
        submit_internal(method, full_uri, req_headers, body, prom, timeout_ms);
      });
}

void h2_session::submit_internal(
    const std::string& method, const std::string& full_uri,
    const std::map<std::string, std::string>& req_headers,
    const std::string& body, std::shared_ptr<std::promise<response>> prom,
    int timeout_ms) {
  uri_components uri_parts;
  try {
    uri_parts = parse_uri(full_uri);
  } catch (const std::exception& e) {
    try {
      prom->set_value({400, e.what(), {}});
    } catch (...) {
    }
    return;
  }

  // Build nghttp2_nv[] array with stable string storage
  std::vector<std::string> name_store, val_store;
  std::vector<nghttp2_nv> nva;
  name_store.reserve(10);
  val_store.reserve(10);
  nva.reserve(10);

  auto push_hdr = [&](std::string name, std::string val) {
    name_store.push_back(std::move(name));
    val_store.push_back(std::move(val));
    nghttp2_nv nv;
    nv.name     = reinterpret_cast<uint8_t*>(name_store.back().data());
    nv.namelen  = name_store.back().size();
    nv.value    = reinterpret_cast<uint8_t*>(val_store.back().data());
    nv.valuelen = val_store.back().size();
    nv.flags    = NGHTTP2_NV_FLAG_NO_COPY_NAME | NGHTTP2_NV_FLAG_NO_COPY_VALUE;
    nva.push_back(nv);
  };

  // Pseudo-headers MUST come first (HTTP/2 spec)
  push_hdr(":method", method);
  push_hdr(":scheme", uri_parts.scheme);
  push_hdr(":authority", uri_parts.host + ":" + uri_parts.port);
  push_hdr(":path", uri_parts.path);

  bool has_accept = false;
  for (const auto& [orig_k, v] : req_headers) {
    std::string k = orig_k;
    std::transform(k.begin(), k.end(), k.begin(), ::tolower);
    if (k.empty() || k[0] == ':') continue;
    if (k == "expect") continue;
    if (k == "accept") has_accept = true;
    push_hdr(k, v);
  }
  if (!has_accept) push_hdr("accept", "application/json");
  if (!body.empty()) push_hdr("content-length", std::to_string(body.size()));

  auto acc          = std::make_shared<response_accumulator>();
  acc->prom         = prom;
  acc->request_body = body;

  nghttp2_data_provider2 dp{};
  nghttp2_data_provider2* dp_ptr = nullptr;
  if (!body.empty()) {
    dp.source.ptr    = acc.get();
    dp.read_callback = body_read_cb;
    dp_ptr           = &dp;
  }

  int32_t stream_id = nghttp2_submit_request2(
      session_, /*pri_spec=*/nullptr, nva.data(), nva.size(), dp_ptr,
      /*stream_user_data=*/acc.get());

  if (stream_id < 0) {
    try {
      prom->set_value(
          {503,
           "nghttp2_submit_request2 failed: " + std::to_string(stream_id),
           {}});
    } catch (...) {
    }
    return;
  }

  streams_[stream_id] = acc;
  m_active_streams.fetch_add(1, std::memory_order_relaxed);

  auto timer = std::make_shared<boost::asio::steady_timer>(io_service_);
  acc->timer = timer;
  timer->expires_from_now(std::chrono::milliseconds(timeout_ms));
  auto self = shared_from_this();
  timer->async_wait(boost::asio::bind_executor(
      strand_, [this, self, stream_id](boost::system::error_code tec) {
        if (tec == boost::asio::error::operation_aborted) return;
        if (streams_.find(stream_id) == streams_.end()) return;
        nghttp2_submit_rst_stream(
            session_, NGHTTP2_FLAG_NONE, stream_id, NGHTTP2_CANCEL);
        pump_send();
      }));

  pump_send();
}

// ============================================================================
// h2_session — fail_all and shutdown
// ============================================================================

void h2_session::fail_all(const std::string& reason) {
  m_errored.store(true, std::memory_order_release);
  m_connected.store(false, std::memory_order_release);

  for (auto& [sid, acc] : streams_) {
    if (acc->timer) acc->timer->cancel();
    try {
      acc->prom->set_value({503, reason, {}});
    } catch (...) {
    }
  }
  streams_.clear();
  m_active_streams.store(0, std::memory_order_relaxed);

  for (auto& req : pending_) {
    try {
      req.prom->set_value({503, reason, {}});
    } catch (...) {
    }
  }
  pending_.clear();

  if (session_) {
    nghttp2_session_del(session_);
    session_ = nullptr;
  }
}

void h2_session::shutdown() {
  auto self = shared_from_this();
  boost::asio::dispatch(strand_, [this, self]() {
    if (!session_) return;
    nghttp2_submit_goaway(
        session_, NGHTTP2_FLAG_NONE,
        nghttp2_session_get_last_proc_stream_id(session_), NGHTTP2_NO_ERROR,
        nullptr, 0);
    pump_send();
    boost::system::error_code ec;
    tcp_layer().close(ec);
  });
}

// ============================================================================
// h2_session — status accessors
// ============================================================================

bool h2_session::is_available() const noexcept {
  return m_connected.load(std::memory_order_acquire) &&
         !m_errored.load(std::memory_order_acquire) &&
         m_active_streams.load(std::memory_order_relaxed) <
             MAX_CONCURRENT_STREAMS;
}
bool h2_session::connected() const noexcept {
  return m_connected.load(std::memory_order_acquire);
}
bool h2_session::errored() const noexcept {
  return m_errored.load(std::memory_order_acquire);
}
int h2_session::active_streams() const noexcept {
  return m_active_streams.load(std::memory_order_relaxed);
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

  std::shared_ptr<h2_session> get_or_create(
      const std::string& host, const std::string& port, bool tls);

  void shutdown_all();

 private:
  std::shared_ptr<h2_session> make_session(const pool_key& key);

  boost::asio::io_service& m_io_service;
  bool m_enable_tls;
  int m_max_per_host;
  boost::asio::ssl::context m_ssl_ctx;

  mutable std::mutex m_mutex;
  std::unordered_map<
      pool_key, std::vector<std::shared_ptr<h2_session>>, pool_key_hash>
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

    m_ssl_ctx.set_verify_mode(boost::asio::ssl::verify_none);

    // Set ALPN to "h2" so the server selects HTTP/2 during TLS negotiation.
    // Format: one byte length prefix + protocol name (RFC 7301 wire format).
    static const unsigned char alpn_proto[] = "\x02h2";
    // SSL_CTX_set_alpn_protos returns 0 on success (OpenSSL convention).
    if (SSL_CTX_set_alpn_protos(
            m_ssl_ctx.native_handle(), alpn_proto, sizeof(alpn_proto) - 1) !=
        0) {
      throw std::runtime_error("SSL_CTX_set_alpn_protos failed");
    }
  }
}

std::shared_ptr<h2_session> nghttp2_connection_pool::get_or_create(
    const std::string& host, const std::string& port, bool tls) {
  pool_key key{host, port, tls};
  std::lock_guard<std::mutex> lk(m_mutex);

  auto& vec = m_pool[key];

  // Evict errored sessions
  vec.erase(
      std::remove_if(
          vec.begin(), vec.end(),
          [](const std::shared_ptr<h2_session>& s) { return s->errored(); }),
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
      [](const std::shared_ptr<h2_session>& a,
         const std::shared_ptr<h2_session>& b) {
        return a->active_streams() < b->active_streams();
      });
  return *best;
}

std::shared_ptr<h2_session> nghttp2_connection_pool::make_session(
    const pool_key& key) {
  std::shared_ptr<h2_session> sess;
  if (key.tls) {
    sess = std::make_shared<h2_session>(
        m_io_service, m_ssl_ctx, key.host, key.port);
  } else {
    sess = std::make_shared<h2_session>(m_io_service, key.host, key.port);
  }
  // IMPORTANT: start() must be called AFTER make_shared so shared_from_this()
  // works
  sess->start();
  return sess;
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

  std::shared_ptr<internal::h2_session> session;
  try {
    session = m_pool->get_or_create(uri_parts.host, uri_parts.port, use_tls);
  } catch (const std::exception& e) {
    m_logger.error("Connection pool error: %s", e.what());
    return {503, "Connection pool error", {}};
  }

  // Merge caller headers with defaults (caller headers take priority)
  std::map<std::string, std::string> merged = req.headers;
  merged.insert({"accept", "application/json"});

  auto prom                    = std::make_shared<std::promise<response>>();
  std::future<response> fut    = prom->get_future();
  const std::string method_str = internal::method_to_string(method);
  session->submit(method_str, req.uri, merged, req.body, prom, m_timeout_ms);

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
