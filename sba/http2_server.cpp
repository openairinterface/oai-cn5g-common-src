/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

/*! \file http2_server.cpp
 \brief Generic HTTP/2 server wrapper — implementation.
        Uses nghttp2 v1.68.1 C API + libevent (cleartext h2c, no TLS).
        Uses v2 API: nghttp2_submit_response2, nghttp2_session_mem_recv2,
        nghttp2_data_provider2, nghttp2_ssize.
 */

#include "http2_server.h"

#include <algorithm>  // std::sort, std::min, std::remove
#include <cstring>    // memset, memcpy
#include <string>
#include <arpa/inet.h>   // inet_pton, inet_ntop, htons, AF_INET
#include <netinet/in.h>  // sockaddr_in, INADDR_ANY
#include <unistd.h>  // close() — used in accept_cb when max_connections exceeded
#include <event2/thread.h>

#include "logger_base.hpp"

// ---------------------------------------------------------------------------
// Diagnostics logger
// ---------------------------------------------------------------------------

namespace oai::sba {

namespace {
// Category the server logs under. Set once at startup by the NF, then only
// read, so no synchronisation is needed.
std::string& http2_server_logger_name() {
  static std::string name = LOGGER_COMMON;
  return name;
}
}  // namespace

void set_http2_server_logger(const std::string& logger_name) {
  http2_server_logger_name() = logger_name;
}

const oai::logger::printf_logger& http2_server_logger() {
  return oai::logger::logger_registry::get_logger(http2_server_logger_name());
}

}  // namespace oai::sba

// Short alias for the log call sites below.
namespace {
inline const oai::logger::printf_logger& srv_log() {
  return oai::sba::http2_server_logger();
}
}  // namespace

// ---------------------------------------------------------------------------
// Internal types (not exposed in header)
// ---------------------------------------------------------------------------

namespace {

static oai::common::sbi::method_e parse_http_method(const std::string& method) {
  using oai::common::sbi::method_e;

  if (method == "POST") return method_e::POST;
  if (method == "GET") return method_e::GET;
  if (method == "PUT") return method_e::PUT;
  if (method == "PATCH") return method_e::PATCH;
  if (method == "DELETE") return method_e::DELETE;
  if (method == "OPTIONS") return method_e::OPTIONS;

  return method_e::OPTIONS;
}

}  // namespace

// Response Body Provider
// Heap-allocated by http2_response::send(); ownership transferred to
// http2_stream so it is cleaned up even on RST_STREAM / connection drop.

struct response_body {
  std::string data;
  size_t offset = 0;
};

// Per-Stream Data
struct http2_stream {
  int32_t stream_id = 0;
  http2_request request;              // accumulated request (headers + body)
  response_body* body_ptr = nullptr;  // owned; deleted in destructor
  bool pending_worker     = false;  // worker dispatched, response not yet sent
  bool closed_while_pending =
      false;  // RST/GOAWAY arrived while worker was running

  explicit http2_stream(int32_t id) : stream_id(id) { request.stream_id = id; }

  ~http2_stream() {
    delete body_ptr;
    body_ptr = nullptr;
  }

  // Non-copyable (owns body_ptr)
  http2_stream(const http2_stream&) = delete;
  http2_stream& operator=(const http2_stream&) = delete;
};

// Per-Connection Data
struct http2_connection {
  http2_server* server     = nullptr;
  struct bufferevent* bev  = nullptr;
  nghttp2_session* session = nullptr;
  std::map<int32_t, std::unique_ptr<http2_stream>> streams;
  uint64_t conn_id = 0;  // assigned in accept_cb; key in server->connections_
  std::string peer_address;  // TCP peer IP string (set in accept_cb)

  // Rapid Reset tracking
  uint32_t rst_stream_count                               = 0;
  static constexpr uint32_t MAX_RST_STREAM_PER_CONNECTION = 200;

  // Deferred destruction: set when a destruction path fires but workers are
  // still processing.  The connection stays in server->connections_ so
  // response_post_cb can find it; the last completing worker deletes it.
  bool pending_destruction = false;

  // Warn-once flags per discard reason (anti-log-storm).
  // Each flag is set the first time that discard reason fires on this
  // connection.
  bool warned_conn_missing          = false;
  bool warned_stream_missing        = false;
  bool warned_stream_closed_pending = false;
  bool warned_pending_dest_discard  = false;

  // Per-connection discard counters (mirror the four warn-once reasons).
  int discard_conn_missing          = 0;
  int discard_stream_missing        = 0;
  int discard_stream_closed_pending = 0;
  int discard_pending_dest          = 0;

  // Timeout (seconds) for force-destroying deferred connections.
  static constexpr int DEFERRED_DESTRUCTION_TIMEOUT_SEC = 30;

  // Defer destruction: disable I/O, schedule safety timeout, log.
  // Called from event_cb / read_cb when has_pending_workers() is true.
  void start_deferred_destruction(const char* reason);

  http2_connection(http2_server* srv, struct bufferevent* bev_arg)
      : server(srv), bev(bev_arg) {}

  ~http2_connection() {
    // Destroy streams first (frees response_body allocations via http2_stream
    // destructors) BEFORE calling nghttp2_session_del(), so that any
    // on_stream_close_callback invocations triggered by session deletion
    // find an empty map and are safe no-ops.
    streams.clear();
    if (session) {
      nghttp2_session_del(session);
      session = nullptr;
    }
    if (bev) {
      bufferevent_free(bev);
      bev = nullptr;
    }
  }

  // Non-copyable
  http2_connection(const http2_connection&) = delete;
  http2_connection& operator=(const http2_connection&) = delete;

  http2_stream* create_stream(int32_t id) {
    auto uptr   = std::make_unique<http2_stream>(id);
    auto* ptr   = uptr.get();
    streams[id] = std::move(uptr);
    return ptr;
  }

  http2_stream* find_stream(int32_t id) {
    auto it = streams.find(id);
    return (it != streams.end()) ? it->second.get() : nullptr;
  }

  void remove_stream(int32_t id) {
    streams.erase(
        id);  // unique_ptr destructor → http2_stream dtor → delete body_ptr
  }

  // Returns true if any stream still has a worker dispatched (pending_worker).
  // Used by teardown paths to decide whether destruction must be deferred.
  bool has_pending_workers() const {
    for (const auto& kv : streams) {
      if (kv.second && kv.second->pending_worker) return true;
    }
    return false;
  }
};

// Thread Pool Work Item
// Heap-allocated when a request is dispatched to a worker thread. The worker
// fills response fields via threaded-mode http2_response::send(), then posts
// response_post_cb back to the event loop via event_base_once(). The event loop
// thread owns deletion.

struct thread_pool_work_item {
  uint64_t conn_id     = 0;
  int32_t stream_id    = 0;
  http2_server* server = nullptr;
  http2_request request;  // moved from stream->request in dispatch
  // Response data filled by handler via threaded-mode http2_response::send()
  int status_code = 200;
  std::map<std::string, std::string> resp_headers;
  std::string resp_body;
};

// ---------------------------------------------------------------------------
// nghttp2 session initialisation
// ---------------------------------------------------------------------------

// send_callback
// Called by nghttp2 to hand outgoing bytes to the transport layer.
// Appends to the bufferevent output buffer; does NOT call bufferevent_write()
// (which can trigger recursive write callbacks in some libevent configs).
// v2 API (nghttp2 v1.68.1): returns nghttp2_ssize (typedef ptrdiff_t).

static nghttp2_ssize send_callback(
    nghttp2_session* /*session*/, const uint8_t* data, size_t length,
    int /*flags*/, void* user_data) {
  auto* conn              = static_cast<http2_connection*>(user_data);
  struct evbuffer* output = bufferevent_get_output(conn->bev);
  if (evbuffer_add(output, data, length) != 0) {
    return NGHTTP2_ERR_CALLBACK_FAILURE;
  }
  return static_cast<nghttp2_ssize>(length);
}

// on_begin_headers_callback
// Called when nghttp2 begins processing a HEADERS frame.
// For a new client request (NGHTTP2_HCAT_REQUEST), allocate per-stream state.

static int on_begin_headers_callback(
    nghttp2_session* /*session*/, const nghttp2_frame* frame, void* user_data) {
  auto* conn = static_cast<http2_connection*>(user_data);

  // Only create stream state for new request streams, not for trailers or
  // server-push responses (which also use HEADERS frames).
  if (frame->hd.type != NGHTTP2_HEADERS ||
      frame->headers.cat != NGHTTP2_HCAT_REQUEST) {
    return 0;
  }

  conn->create_stream(frame->hd.stream_id);
  return 0;
}

// on_header_callback
// Called once per header in a HEADERS frame.
// Populates http2_request pseudo-header fields and regular headers map.

static int on_header_callback(
    nghttp2_session* /*session*/, const nghttp2_frame* frame,
    const uint8_t* name, size_t namelen, const uint8_t* value, size_t valuelen,
    uint8_t /*flags*/, void* user_data) {
  auto* conn = static_cast<http2_connection*>(user_data);

  if (frame->hd.type != NGHTTP2_HEADERS ||
      frame->headers.cat != NGHTTP2_HCAT_REQUEST) {
    return 0;
  }

  auto* stream = conn->find_stream(frame->hd.stream_id);
  if (!stream) {
    return 0;
  }

  std::string hname(reinterpret_cast<const char*>(name), namelen);
  std::string hval(reinterpret_cast<const char*>(value), valuelen);

  // Route HTTP/2 pseudo-headers to dedicated request fields.
  // Regular headers (already lowercase per HTTP/2 spec) go into the map.
  if (hname == ":method") {
    stream->request.method = parse_http_method(hval);
  } else if (hname == ":path") {
    auto qpos = hval.find('?');
    if (qpos != std::string::npos) {
      stream->request.path      = hval.substr(0, qpos);
      stream->request.raw_query = hval.substr(qpos + 1);
    } else {
      stream->request.path = std::move(hval);
    }
  } else if (hname == ":scheme") {
    stream->request.scheme = std::move(hval);
  } else if (hname == ":authority") {
    stream->request.authority = std::move(hval);
  } else {
    stream->request.headers[std::move(hname)] = std::move(hval);
  }

  return 0;
}

// on_data_chunk_recv_callback
// Called for each chunk of received DATA payload.
// Accumulates body data.  Enforces max_request_body_size.
// CRITICAL: nghttp2_session_consume() MUST be called to replenish the
//           flow-control window; without it the connection stalls.

static int on_data_chunk_recv_callback(
    nghttp2_session* session, uint8_t /*flags*/, int32_t stream_id,
    const uint8_t* data, size_t len, void* user_data) {
  auto* conn   = static_cast<http2_connection*>(user_data);
  auto* stream = conn->find_stream(stream_id);
  if (!stream) {
    return 0;
  }

  // Enforce maximum body size — reset the stream if exceeded.
  size_t max_body = conn->server->config().max_request_body_size;
  if (stream->request.body.size() + len > max_body) {
    nghttp2_submit_rst_stream(
        session, NGHTTP2_FLAG_NONE, stream_id, NGHTTP2_ENHANCE_YOUR_CALM);
    return 0;
  }

  stream->request.body.append(reinterpret_cast<const char*>(data), len);

  // Replenish the connection- and stream-level flow-control windows.
  nghttp2_session_consume(session, stream_id, len);

  return 0;
}

// on_frame_recv_callback
// Called when a complete HTTP/2 frame has been received. When END_STREAM is set
// on a HEADERS or DATA frame, the request is complete; dispatch to the
// registered route handler.

static int on_frame_recv_callback(
    nghttp2_session* /*session*/, const nghttp2_frame* frame, void* user_data) {
  auto* conn = static_cast<http2_connection*>(user_data);

  switch (frame->hd.type) {
    case NGHTTP2_DATA:
    case NGHTTP2_HEADERS: {
      if (!(frame->hd.flags & NGHTTP2_FLAG_END_STREAM)) {
        break;  // More data / headers still coming
      }

      auto* stream = conn->find_stream(frame->hd.stream_id);
      if (!stream) {
        break;
      }

      // Look up the route handler by longest-prefix match.
      http2_handler* handler = conn->server->find_handler(stream->request.path);

      if (handler) {
        if (conn->server->has_thread_pool() &&
            !conn->server->is_shutting_down()) {
          // ── Async path: dispatch to worker, post response via
          // event_base_once
          auto* item      = new thread_pool_work_item;
          item->conn_id   = conn->conn_id;
          item->stream_id = frame->hd.stream_id;
          item->server    = conn->server;
          item->request =
              std::move(stream->request);  // move; stream still alive
          stream->pending_worker = true;

          bool ok = conn->server->get_thread_pool()->enqueue(
              [item, handler, srv = conn->server]() {
                http2_response async_resp(item);
                try {
                  (*handler)(item->request, async_resp);
                  if (!async_resp.was_deferred() && !async_resp.was_sent()) {
                    async_resp.send(
                        500, {{"content-type", "text/plain"}},
                        "Internal Server Error");
                  }
                } catch (...) {
                  if (!async_resp.was_deferred() && !async_resp.was_sent()) {
                    async_resp.send(
                        500, {{"content-type", "text/plain"}},
                        "Internal Server Error");
                  }
                }

                // The handler obtained an http2_deferred_response and will (or
                // its destructor will) post response_post_cb itself. The work
                // item is now owned by the deferred handle — do NOT post or
                // delete it here.
                if (async_resp.was_deferred()) {
                  return;
                }

                // Marshal the captured response back to the event loop thread.
                // Thread-safe cross-thread post: relies on
                // evthread_use_pthreads() having been called in
                // http2_server::start() before event_base_new().
                struct timeval zero_tv = {0, 0};
                if (event_base_once(
                        srv->base(), -1, EV_TIMEOUT,
                        http2_server::response_post_cb, item, &zero_tv) != 0) {
                  // Worker-thread post back to event loop failed; stream
                  // cleanup cannot be performed from this worker thread —
                  // deferred-destruction timeout (if active) or server shutdown
                  // will be the last cleanup opportunity.
                  srv_log().error(
                      "HTTP2 conn %llu stream %d: event_base_once failed,"
                      " response discarded",
                      (unsigned long long) item->conn_id, item->stream_id);
                  delete item;  // prevent leak; client will get a timeout
                }
              });

          if (!ok) {
            // Worker queue full — revert and send 503 synchronously.
            delete item;
            stream->pending_worker = false;
            http2_response sync_resp(
                conn->session, frame->hd.stream_id, conn->bev, stream);
            sync_resp.send(
                503, {{"content-type", "application/problem+json"}},
                R"({"type":"about:blank","title":"Service Unavailable","status":503,"detail":"Server is overloaded, please retry later"})");
          }
        } else {
          // Synchronous path (no pool, or shutting down)
          http2_response response(
              conn->session, frame->hd.stream_id, conn->bev, stream);
          try {
            (*handler)(stream->request, response);
          } catch (...) {
            // handler threw — fall through to was_sent() guard below
          }
          if (!response.was_sent()) {
            response.send(
                500, {{"content-type", "text/plain"}}, "Internal Server Error");
          }
        }
      } else {
        // No matching route.
        http2_response response(
            conn->session, frame->hd.stream_id, conn->bev, stream);
        response.send(404, {{"content-type", "text/plain"}}, "Not Found");
      }
      break;
    }
    default:
      break;
  }

  return 0;
}

// on_stream_close_callback
// Called whenever a stream is closed (normal completion, RST_STREAM, GOAWAY).
// Cleans up per-stream state (including response_body via http2_stream dtor).
// Also detects Rapid Reset.

static int on_stream_close_callback(
    nghttp2_session* /*session*/, int32_t stream_id, uint32_t error_code,
    void* user_data) {
  auto* conn = static_cast<http2_connection*>(user_data);

  // Rapid Reset detection.
  // The client uses NGHTTP2_CANCEL (= 8, RFC 7540 §7) in RST_STREAM frames
  // for this attack.  Do NOT check NGHTTP2_STREAM_CLOSED — that is an
  // nghttp2_error API return code, not a stream error code from the wire.
  if (error_code == NGHTTP2_CANCEL) {
    conn->rst_stream_count++;
    if (conn->rst_stream_count >
        http2_connection::MAX_RST_STREAM_PER_CONNECTION) {
      nghttp2_submit_goaway(
          conn->session, NGHTTP2_FLAG_NONE,
          nghttp2_session_get_last_proc_stream_id(conn->session),
          NGHTTP2_ENHANCE_YOUR_CALM, nullptr, 0);
      // nghttp2_session_send() will be called in read_cb after mem_recv
      // returns.
    }
  }

  // Erase stream — unique_ptr<http2_stream> destructor runs:
  //   http2_stream::~http2_stream() → delete body_ptr
  // This guarantees no response_body leak even on abnormal stream closure
  //
  // Exception: if a worker thread is still processing this stream, defer
  // removal so that response_post_cb can still find the stream and either
  // send the response or detect the closure.
  auto* stream = conn->find_stream(stream_id);
  if (stream && stream->pending_worker) {
    stream->closed_while_pending = true;
    return 0;  // response_post_cb will erase the stream
  }

  conn->remove_stream(stream_id);
  return 0;
}

// Session initialisation helper

static void initialize_nghttp2_session(
    http2_connection* conn, const http2_server_config& config) {
  nghttp2_session_callbacks* callbacks;
  nghttp2_session_callbacks_new(&callbacks);

  // v2 API (nghttp2 v1.68.1): _set_send_callback2
  nghttp2_session_callbacks_set_send_callback2(callbacks, send_callback);
  nghttp2_session_callbacks_set_on_frame_recv_callback(
      callbacks, on_frame_recv_callback);
  nghttp2_session_callbacks_set_on_stream_close_callback(
      callbacks, on_stream_close_callback);
  nghttp2_session_callbacks_set_on_header_callback(
      callbacks, on_header_callback);
  nghttp2_session_callbacks_set_on_begin_headers_callback(
      callbacks, on_begin_headers_callback);
  nghttp2_session_callbacks_set_on_data_chunk_recv_callback(
      callbacks, on_data_chunk_recv_callback);

  nghttp2_option* option;
  nghttp2_option_new(&option);
  // Limit outstanding unacknowledged PING frames — PING flood protection.
  nghttp2_option_set_max_outbound_ack(option, 1000);

  // ── v1.68.1 security hardening ──

  // CVE-2023-44487 (HTTP/2 Rapid Reset): Token-bucket rate limiter.
  // burst=100: initial and maximum token count.
  // rate=30: tokens regenerated per second.
  // Each incoming RST_STREAM consumes one token. When tokens are exhausted,
  // the library sends GOAWAY and closes the connection.
  // This supplements the manual detection in on_stream_close_callback, which
  // can be removed once library-level protection is validated in production.
  // Library defaults: burst=1000, rate=33.
  nghttp2_option_set_stream_reset_rate_limit(option, 1000, 33);

  // CVE-2024-28182 (CONTINUATION flood): Limit CONTINUATION frames per
  // HEADERS sequence. Prevents HPACK bomb / memory exhaustion attacks.
  // Library default is 8. We set 16 to be more permissive for legitimate
  // clients that may split large header blocks across multiple CONTINUATION
  // frames, while still blocking flood attacks.
  nghttp2_option_set_max_continuations(option, 16);

  // NOTE: nghttp2_option_set_max_settings() is NOT called.
  // The library default is 32, which is appropriate (RFC 7540 defines 6
  // standard settings; 32 provides ample room for extensions). Explicitly
  // setting it to 32 would be redundant.

  // NOTE: nghttp2_option_set_glitch_rate_limit() is NOT called.
  // Token-bucket rate limiter for protocol errors / frame violations.
  // Library defaults (burst=1000, rate=33 tokens/sec) are appropriate.
  // burst = initial/max token count, rate = tokens regenerated per second.
  // Each protocol error consumes one token; exhaustion triggers GOAWAY.
  // Uncomment to override defaults:
  // nghttp2_option_set_glitch_rate_limit(option, 1000, 33);

  nghttp2_session_server_new2(&conn->session, callbacks, conn, option);

  nghttp2_option_del(option);
  nghttp2_session_callbacks_del(callbacks);

  // Send initial SETTINGS frame (server connection preface).
  nghttp2_settings_entry settings[] = {
      {NGHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS, config.max_concurrent_streams},
      {NGHTTP2_SETTINGS_INITIAL_WINDOW_SIZE, config.initial_window_size},
      {NGHTTP2_SETTINGS_MAX_HEADER_LIST_SIZE, config.max_header_list_size},
      {NGHTTP2_SETTINGS_ENABLE_PUSH, 0},  // server push not used
  };
  nghttp2_submit_settings(
      conn->session, NGHTTP2_FLAG_NONE, settings,
      sizeof(settings) / sizeof(settings[0]));
}

// ---------------------------------------------------------------------------
// http2_response implementation
// ---------------------------------------------------------------------------

// Forward declaration — response_body_read_callback must be declared before
// http2_response::send() references it as a function pointer.
// v2 API (nghttp2 v1.68.1): returns nghttp2_ssize.
static nghttp2_ssize response_body_read_callback(
    nghttp2_session*, int32_t, uint8_t*, size_t, uint32_t*,
    nghttp2_data_source*, void*);

// ---------------------------------------------------------------------------

http2_response::http2_response(
    nghttp2_session* session, int32_t stream_id, struct bufferevent* bev,
    http2_stream* stream)
    : session_(session), stream_id_(stream_id), bev_(bev), stream_(stream) {}

// Threaded constructor: session_/bev_/stream_ left null; data captured into
// work_item_ by send(), submitted to nghttp2 by response_post_cb.
http2_response::http2_response(thread_pool_work_item* work_item)
    : work_item_(work_item) {}

bool http2_response::was_sent() const {
  return sent_;
}

void http2_response::send(
    int status_code, const std::map<std::string, std::string>& headers,
    const std::string& body) {
  if (sent_) return;  // Guard against double-send
  sent_ = true;

  // Threaded mode: capture data for response_post_cb (event loop thread)
  // Must NOT touch session_/bev_/stream_ — those are null in threaded mode and
  // would be unsafe to use from a worker thread anyway (nghttp2 not
  // thread-safe).
  if (work_item_) {
    work_item_->status_code  = status_code;
    work_item_->resp_headers = headers;
    work_item_->resp_body    = body;
    return;  // response_post_cb will call submit_response + session_send
  }

  // Synchronous mode: submit directly to nghttp2 (event loop thread only) ──
  // No NGHTTP2_NV_FLAG_NO_COPY_* — nghttp2 copies all name/value data during
  // nghttp2_submit_response(). Safe regardless of header string lifetimes
  // after this function returns.
  std::string status_str = std::to_string(status_code);

  std::vector<nghttp2_nv> nva;
  nva.reserve(headers.size() + 1);

  // :status MUST be the first pseudo-header (convention + wire ordering).
  nva.push_back(
      {(uint8_t*) ":status", (uint8_t*) status_str.c_str(), 7,
       status_str.size(), NGHTTP2_NV_FLAG_NONE});

  for (const auto& [name, value] : headers) {
    nva.push_back(
        {(uint8_t*) name.c_str(), (uint8_t*) value.c_str(), name.size(),
         value.size(), NGHTTP2_NV_FLAG_NONE});
  }

  if (body.empty()) {
    // No body — send HEADERS frame with END_STREAM.
    // v2 API (nghttp2 v1.68.0+): nghttp2_submit_response2
    int rv = nghttp2_submit_response2(
        session_, stream_id_, nva.data(), nva.size(), nullptr);
    if (rv != 0) {
      srv_log().error(
          "HTTP2 send: nghttp2_submit_response2 failed for stream {}: {}",
          stream_id_, nghttp2_strerror(rv));
    }
  } else {
    // Allocate response_body on the heap; transfer ownership to http2_stream
    // so it is freed even if the stream is reset before EOF.
    auto* body_data = new response_body{body, 0};

    if (stream_) {
      delete stream_
          ->body_ptr;  // defensive: shouldn't be set with double-send guard
      stream_->body_ptr = body_data;
    }

    // v2 API (nghttp2 v1.68.0+): nghttp2_data_provider2
    nghttp2_data_provider2 data_prd;
    data_prd.source.ptr    = body_data;
    data_prd.read_callback = response_body_read_callback;

    // v2 API (nghttp2 v1.68.0+): nghttp2_submit_response2
    int rv = nghttp2_submit_response2(
        session_, stream_id_, nva.data(), nva.size(), &data_prd);
    if (rv != 0) {
      srv_log().error(
          "HTTP2 send: nghttp2_submit_response2 failed for stream {}: {}",
          stream_id_, nghttp2_strerror(rv));
    }
  }

  // Flush all pending frames immediately.
  // Calling nghttp2_session_send() HERE ensures:
  //   (a) HPACK encoding happens while status_str is still alive on the stack.
  //   (b) The DATA frame(s) are written to the bufferevent in the same
  //       event loop iteration as the request dispatch, reducing latency.
  // For the common case (small responses fitting one DATA frame), this
  // sends everything in one shot without waiting for the next read_cb cycle.
  int rv_send = nghttp2_session_send(session_);
  if (rv_send != 0) {
    srv_log().error(
        "HTTP2 send: nghttp2_session_send failed for stream {}: {}", stream_id_,
        nghttp2_strerror(rv_send));
  }
}

void http2_response::send(
    int status_code, const std::map<std::string, std::string>& headers) {
  send(status_code, headers, "");
}

bool http2_response::was_deferred() const noexcept {
  return was_deferred_;
}

// Create a deferred response handle. Only meaningful in threaded mode, where
// the response is backed by a heap work_item that outlives the worker lambda.
// In synchronous mode there is no work_item to hand off, so an invalid handle
// is returned and the caller must send() directly.
http2_deferred_response http2_response::make_deferred() {
  if (!work_item_ || sent_) {
    // Not threaded mode, or already sent — cannot defer. Return invalid handle.
    return http2_deferred_response{};
  }
  was_deferred_ = true;
  // work_item_->server is the owning http2_server (set at dispatch time).
  return http2_deferred_response(work_item_, work_item_->server);
}

// ---------------------------------------------------------------------------
// http2_deferred_response implementation
// ---------------------------------------------------------------------------

http2_deferred_response::http2_deferred_response(
    thread_pool_work_item* item, http2_server* srv) noexcept
    : item_(item),
      srv_(srv),
      sent_(std::make_shared<std::atomic<bool>>(false)) {}

http2_deferred_response::http2_deferred_response(
    http2_deferred_response&& other) noexcept
    : item_(other.item_), srv_(other.srv_), sent_(std::move(other.sent_)) {
  // Disarm the moved-from handle so its destructor does not post a 500.
  other.item_ = nullptr;
  other.srv_  = nullptr;
}

http2_deferred_response& http2_deferred_response::operator=(
    http2_deferred_response&& other) noexcept {
  if (this != &other) {
    // If this handle still owns an unsent work item, deliver a 500 before
    // overwriting so the in-flight request is not abandoned.
    if (item_ && sent_ && !sent_->load()) {
      deliver(500, {{"content-type", "text/plain"}}, "Internal Server Error");
    }
    item_       = other.item_;
    srv_        = other.srv_;
    sent_       = std::move(other.sent_);
    other.item_ = nullptr;
    other.srv_  = nullptr;
  }
  return *this;
}

http2_deferred_response::~http2_deferred_response() {
  // If a valid handle was never sent, post a 500 so the client does not hang.
  if (item_ && sent_ && !sent_->load()) {
    deliver(500, {{"content-type", "text/plain"}}, "Internal Server Error");
  }
}

void http2_deferred_response::send(
    int status, std::map<std::string, std::string> headers, std::string body) {
  deliver(status, std::move(headers), std::move(body));
}

void http2_deferred_response::deliver(
    int status, std::map<std::string, std::string> headers, std::string body) {
  if (!item_ || !sent_) return;
  // Exactly-once: only the first caller proceeds.
  bool expected = false;
  if (!sent_->compare_exchange_strong(expected, true)) return;

  // Write the response into the work item exactly as a threaded-mode
  // http2_response::send() would, then post response_post_cb onto the event
  // loop. response_post_cb owns deletion of the work item and performs all the
  // connection/stream lifetime guards on the event-loop thread.
  item_->status_code  = status;
  item_->resp_headers = std::move(headers);
  item_->resp_body    = std::move(body);

  struct timeval zero_tv = {0, 0};
  if (event_base_once(
          srv_->base(), -1, EV_TIMEOUT, http2_server::response_post_cb, item_,
          &zero_tv) != 0) {
    // Post failed — cannot reach the event loop from here. Delete the work
    // item to avoid a leak; the client will time out. Mirrors the failure
    // handling in the pool-worker lambda.
    srv_log().error(
        "HTTP2 conn %llu stream %d: deferred event_base_once failed,"
        " response discarded",
        static_cast<unsigned long long>(item_->conn_id), item_->stream_id);
    delete item_;
  }
  item_ = nullptr;  // ownership transferred to response_post_cb (or deleted)
}

// Response Body Data Provider
// v2 API (nghttp2 v1.68.0+): read_callback returns nghttp2_ssize.
// Called by nghttp2_session_send() to read body bytes into a DATA frame.

static nghttp2_ssize response_body_read_callback(
    nghttp2_session* /*session*/, int32_t /*stream_id*/, uint8_t* buf,
    size_t length, uint32_t* data_flags, nghttp2_data_source* source,
    void* /*user_data*/) {
  auto* body       = static_cast<response_body*>(source->ptr);
  size_t remaining = body->data.size() - body->offset;
  size_t nread     = std::min(remaining, length);

  memcpy(buf, body->data.data() + body->offset, nread);
  body->offset += nread;

  if (body->offset >= body->data.size()) {
    *data_flags |= NGHTTP2_DATA_FLAG_EOF;
    // Do NOT delete body here.  Ownership belongs to http2_stream.
    // http2_stream::~http2_stream() will delete body_ptr.
  }

  return static_cast<nghttp2_ssize>(nread);
}

// response_post_cb
// Runs on the event loop thread (scheduled via event_base_once() by a worker).
// Submits the captured response to nghttp2 and cleans up the work item.
//
// Discard branches (bounded diagnostics, anti-log-storm):
//   conn_missing        — connection deleted before postback arrived; warn
//                         unconditionally (no conn to store a warn-once flag
//                         on).
//   pending_destruction — connection in deferred-destruction state; clear
//                         stream and, if last worker, finalize cleanup.
//   stream_missing      — stream removed before postback arrived; warn-once.
//   stream_closed_pending — stream RST/GOAWAY'd while worker ran; warn-once.

void http2_server::response_post_cb(
    evutil_socket_t /*fd*/, short /*what*/, void* arg) {
  auto* item   = static_cast<thread_pool_work_item*>(arg);
  auto* server = item->server;

  // 1. Connection lookup
  // conn_id provides O(1) lookup without dangling pointers.
  http2_connection* conn = server->find_connection(item->conn_id);
  if (!conn) {
    // Connection gone: warn unconditionally (no conn object to hold a
    // once-flag).
    srv_log().warn(
        "HTTP2 conn %llu stream %d: response discarded, connection gone",
        static_cast<unsigned long long>(item->conn_id), item->stream_id);
    delete item;
    return;
  }

  // 2. Deferred-destruction gate
  // The connection is logically dead but kept alive so this postback can find
  // it.  Do NOT send the response; instead clear the stream's pending flag and,
  // if this was the last in-flight worker, complete the deferred cleanup.
  if (conn->pending_destruction) {
    if (!conn->warned_pending_dest_discard) {
      conn->warned_pending_dest_discard = true;
      srv_log().warn(
          "HTTP2 conn %llu stream %d: response discarded, connection in "
          "deferred destruction",
          static_cast<unsigned long long>(conn->conn_id), item->stream_id);
    }
    conn->discard_pending_dest++;

    // Clear the stream's pending flag and remove it from the map.
    auto it = conn->streams.find(item->stream_id);
    if (it != conn->streams.end() && it->second) {
      it->second->pending_worker = false;
      conn->remove_stream(item->stream_id);
    }

    // If this was the last pending worker, finalize deferred destruction.
    if (!conn->has_pending_workers()) {
      srv_log().warn(
          "HTTP2 conn %llu: deferred destruction completing — discards: "
          "conn_missing=%d stream_missing=%d stream_closed_pending=%d "
          "pending_dest=%d",
          static_cast<unsigned long long>(conn->conn_id),
          conn->discard_conn_missing, conn->discard_stream_missing,
          conn->discard_stream_closed_pending, conn->discard_pending_dest);
      server->remove_connection(conn);
      delete conn;
    }
    delete item;
    return;
  }

  // 3. Stream lookup
  http2_stream* stream = conn->find_stream(item->stream_id);
  if (!stream) {
    if (!conn->warned_stream_missing) {
      conn->warned_stream_missing = true;
      srv_log().warn(
          "HTTP2 conn %llu stream %d: response discarded, stream gone",
          static_cast<unsigned long long>(conn->conn_id), item->stream_id);
    }
    conn->discard_stream_missing++;
    delete item;
    return;
  }

  // 4. Stream-closed-while-pending guard
  // The stream was RST or GOAWAY'd after the worker was dispatched — or
  // pending_worker was already cleared by a prior path (defensive guard).
  if (stream->closed_while_pending || !stream->pending_worker) {
    if (!conn->warned_stream_closed_pending) {
      conn->warned_stream_closed_pending = true;
      srv_log().warn(
          "HTTP2 conn %llu stream %d: response discarded, stream closed "
          "while pending",
          static_cast<unsigned long long>(conn->conn_id), item->stream_id);
    }
    conn->discard_stream_closed_pending++;
    conn->remove_stream(item->stream_id);
    delete item;
    return;
  }

  // 5. Normal send path
  // Submit the response on the event loop thread (nghttp2 not thread-safe).
  http2_response response(conn->session, item->stream_id, conn->bev, stream);
  response.send(item->status_code, item->resp_headers, item->resp_body);

  // Clear the deferred-removal guard AFTER send() — nghttp2_session_send()
  // inside send() may trigger on_stream_close_callback, which checks
  // pending_worker and sets closed_while_pending if still true.
  stream->pending_worker = false;

  // If nghttp2 closed the stream during session_send() above, remove it now.
  if (stream->closed_while_pending) {
    conn->remove_stream(item->stream_id);
  }

  delete item;
}

// ---------------------------------------------------------------------------
// http2_server — constructor / destructor
// ---------------------------------------------------------------------------

http2_server::http2_server(
    const std::string& address, uint32_t port, http2_server_config config)
    : address_(address), port_(port), config_(config) {}

http2_server::~http2_server() {
  // Destruction before stop() is a programming error; do best-effort cleanup.
  if (running_.load() && base_) {
    event_base_loopbreak(base_);
  }
  // The authoritative cleanup path is in start() after event_base_dispatch()
  // returns.  If start() was never called, base_ is nullptr — nothing to do.
}

// ---------------------------------------------------------------------------
// Route registration and lookup
// ---------------------------------------------------------------------------

void http2_server::handle(
    const std::string& path_prefix, http2_handler handler) {
  routes_.push_back({path_prefix, std::move(handler)});
}

// Routes are pre-sorted longest-first in start(); simple linear scan suffices
// for the small route tables used by OAI NFs (< 20 routes).
http2_handler* http2_server::find_handler(const std::string& path) {
  for (auto& route : routes_) {
    if (path.compare(0, route.prefix.size(), route.prefix) == 0) {
      return &route.handler;
    }
  }
  return nullptr;
}

// ---------------------------------------------------------------------------
// Server lifecycle — start / stop
// ---------------------------------------------------------------------------

void http2_server::start() {
  // Sort routes longest-prefix-first so find_handler() returns the most
  // specific match without needing further disambiguation.
  std::sort(routes_.begin(), routes_.end(), [](const Route& a, const Route& b) {
    return a.prefix.size() > b.prefix.size();
  });

  // Enable libevent internal locking — MUST be called before event_base_new().
  // This also makes event_base_once() thread-safe, which is the mechanism used
  // by worker-thread postbacks (response_post_cb) and stop() to marshal work
  // back onto the event loop from other threads.
  evthread_use_pthreads();

  base_ = event_base_new();
  if (!base_) {
    srv_log().error("HTTP2 server: failed to create event_base");
    return;
  }

  // Resolve the bind address (IPv4 only — matches the NF SBI configuration's
  // get_addr4(); IPv6 is a future enhancement).
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port   = htons(static_cast<uint16_t>(port_));

  if (address_.empty() || address_ == "0.0.0.0") {
    sin.sin_addr.s_addr = INADDR_ANY;
  } else {
    if (inet_pton(AF_INET, address_.c_str(), &sin.sin_addr) != 1) {
      srv_log().error(
          "HTTP2 server: invalid bind address '%s'", address_.c_str());
      event_base_free(base_);
      base_ = nullptr;
      return;
    }
  }

  listener_ = evconnlistener_new_bind(
      base_, accept_cb, this, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
      -1,  // backlog: let the kernel choose
      reinterpret_cast<struct sockaddr*>(&sin), static_cast<int>(sizeof(sin)));

  if (!listener_) {
    srv_log().error(
        "HTTP2 server: failed to bind to %s:%u", address_.c_str(), port_);
    event_base_free(base_);
    base_ = nullptr;
    return;
  }

  running_.store(true);

  // Create thread pool if configured (0 = synchronous mode).
  if (config_.num_worker_threads > 0) {
    pool_ = std::make_unique<thread_pool>(
        config_.num_worker_threads, config_.max_pending_tasks);
    srv_log().info(
        "HTTP2 server: thread pool created (%u workers)",
        config_.num_worker_threads);
  }

  srv_log().info("HTTP2 server listening on %s:%u", address_.c_str(), port_);

  // Blocks until drain_timer_cb calls event_base_loopbreak().
  event_base_dispatch(base_);

  // Cleanup after event loop exits
  // At this point goaway_and_drain_cb + drain_timer_cb have completed.
  if (listener_) {
    evconnlistener_free(listener_);
    listener_ = nullptr;
  }
  if (drain_timer_) {
    event_free(drain_timer_);
    drain_timer_ = nullptr;
  }
  close_all_connections();
  pool_.reset();  // join worker threads
  event_base_free(base_);
  base_ = nullptr;
  running_.store(false);
  srv_log().info("HTTP2 server fully stopped");
}

void http2_server::stop() {
  if (!running_.load()) return;

  // Schedule the GOAWAY + drain sequence on the event loop thread.
  // event_base_once() is documented thread-safe in libevent.
  struct timeval zero_tv = {0, 0};
  int ret                = event_base_once(
      base_, -1, EV_TIMEOUT, goaway_and_drain_cb, this, &zero_tv);
  if (ret != 0) {
    srv_log().error(
        "HTTP2 server stop: event_base_once failed, using loopbreak fallback");
    event_base_loopbreak(base_);
  }
}

// ---------------------------------------------------------------------------
// libevent callbacks
//      NOTE: NO `static` keyword on these out-of-class definitions.
//      `static` appears only in the in-class declarations.
// ---------------------------------------------------------------------------

// accept_cb, called by libevent when a new TCP connection arrives.

void http2_server::accept_cb(
    struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr,
    int addrlen, void* arg) {
  auto* server            = static_cast<http2_server*>(arg);
  struct event_base* base = evconnlistener_get_base(listener);

  // Enforce max_connections limit.
  {
    std::lock_guard<std::mutex> lock(server->connections_mutex_);
    if (server->connections_.size() >=
        static_cast<size_t>(server->config_.max_connections)) {
      close(fd);  // reject — POSIX close() requires <unistd.h>
      return;
    }
  }

  // Create bufferevent for this connection.
  // BEV_OPT_DEFER_CALLBACKS: callbacks are deferred to the next event loop
  // iteration rather than called from within bufferevent processing.  This
  // makes it safe to call bufferevent_free() (via `delete conn`) from inside
  // read_cb or event_cb — the callback is not on the bufferevent's internal
  // call stack at that point.
  struct bufferevent* bev = bufferevent_socket_new(
      base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
  if (!bev) {
    close(fd);
    return;
  }

  auto* conn    = new http2_connection(server, bev);
  conn->conn_id = server->next_conn_id_.fetch_add(1);

  // Extract and store peer IP address for request context and rate limiting.
  if (addr && addrlen >= static_cast<int>(sizeof(struct sockaddr_in)) &&
      addr->sa_family == AF_INET) {
    char buf[INET_ADDRSTRLEN] = {};
    const auto* sin4 = reinterpret_cast<const struct sockaddr_in*>(addr);
    if (inet_ntop(AF_INET, &sin4->sin_addr, buf, sizeof(buf))) {
      conn->peer_address = buf;
    } else {
      conn->peer_address = "unknown";
    }
  } else {
    conn->peer_address = "unknown";
  }

  initialize_nghttp2_session(conn, server->config_);

  bufferevent_setcb(bev, read_cb, nullptr, event_cb, conn);
  bufferevent_enable(bev, EV_READ | EV_WRITE);

  // Slow loris / idle connection protection.
  struct timeval tv;
  tv.tv_sec  = server->config_.connection_idle_timeout_sec;
  tv.tv_usec = 0;
  bufferevent_set_timeouts(bev, &tv, &tv);

  // Flush the server connection preface (SETTINGS frame submitted in
  // initialize_nghttp2_session).  This constitutes the complete h2c server
  // preface as defined in RFC 7540 §3.5 (Direct h2c — no
  // HTTP/1.1 Upgrade mechanism; clients must use --http2-prior-knowledge).
  nghttp2_session_send(conn->session);

  server->add_connection(conn);
}

// ---------------------------------------------------------------------------
// read_cb, called when data arrives on a connection's bufferevent. Feeds
// received bytes to nghttp2, then flushes any protocol-level frames
// (SETTINGS_ACK, WINDOW_UPDATE, RST_STREAM, etc.) generated during processing.

void http2_server::read_cb(struct bufferevent* bev, void* arg) {
  auto* conn             = static_cast<http2_connection*>(arg);
  struct evbuffer* input = bufferevent_get_input(bev);

  size_t datalen = evbuffer_get_length(input);
  if (datalen == 0) return;

  // Linearise the input buffer into a contiguous region.
  // evbuffer_pullup() returns unsigned char*; nghttp2_session_mem_recv() takes
  // const uint8_t*. On all supported platforms these are identical, but the
  // reinterpret_cast makes the conversion explicit.
  // Note: for large buffers evbuffer_pullup copies memory. For SBI workloads
  // (small SBI frames) this is negligible. Future optimisation:
  // evbuffer_peek().
  unsigned char* raw = evbuffer_pullup(input, static_cast<ev_ssize_t>(datalen));
  if (!raw) return;
  const uint8_t* data = reinterpret_cast<const uint8_t*>(raw);

  // Feed data to nghttp2.
  // v2 API (nghttp2 v1.68.1): nghttp2_session_mem_recv2 returns nghttp2_ssize.
  nghttp2_ssize readlen =
      nghttp2_session_mem_recv2(conn->session, data, datalen);
  if (readlen < 0) {
    // Fatal session error — close connection.
    // SAFE to call delete conn here: BEV_OPT_DEFER_CALLBACKS ensures we are
    // NOT on the bufferevent's internal call stack.
    if (conn->pending_destruction) {
      return;  // already in deferred state
    }
    if (conn->has_pending_workers()) {
      conn->start_deferred_destruction("read_cb:recv_error");
      return;
    }
    conn->server->remove_connection(conn);
    delete conn;
    return;
  }

  evbuffer_drain(input, static_cast<size_t>(readlen));

  // Flush frames generated by nghttp2_session_mem_recv() processing:
  //   SETTINGS_ACK  — response to client's initial SETTINGS
  //   WINDOW_UPDATE — triggered by nghttp2_session_consume() calls
  //   RST_STREAM    — from on_data_chunk_recv (body too large) or Rapid Reset
  //   GOAWAY        — from Rapid Reset detection in on_stream_close_callback
  //
  // NOTE: Response HEADERS + DATA frames are already flushed by the
  // nghttp2_session_send() call inside http2_response::send().  This call
  // handles only protocol-level frames queued by nghttp2 internally during
  // mem_recv (session_send inside send()).
  int rv = nghttp2_session_send(conn->session);
  if (rv != 0) {
    if (conn->pending_destruction) {
      return;  // already in deferred state
    }
    if (conn->has_pending_workers()) {
      conn->start_deferred_destruction("read_cb:send_error");
      return;
    }
    conn->server->remove_connection(conn);
    delete conn;
    return;
  }

  // If both read and write desires are gone, the session is over.
  if (nghttp2_session_want_read(conn->session) == 0 &&
      nghttp2_session_want_write(conn->session) == 0) {
    if (conn->pending_destruction) {
      return;  // already in deferred state
    }
    if (conn->has_pending_workers()) {
      conn->start_deferred_destruction("read_cb:session_exhausted");
      return;
    }
    conn->server->remove_connection(conn);
    delete conn;
  }
}

// ---------------------------------------------------------------------------
// event_cb, called on connection EOF, error, or timeout.
void http2_server::event_cb(
    struct bufferevent* /*bev*/, short events, void* arg) {
  auto* conn = static_cast<http2_connection*>(arg);

  if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
    // BEV_EVENT_TIMEOUT fires when connection_idle_timeout_sec expires,
    // implementing slow loris / idle connection protection.
    // SAFE to delete: BEV_OPT_DEFER_CALLBACKS is set.
    if (conn->pending_destruction) {
      return;  // already in deferred state
    }
    const char* reason = (events & BEV_EVENT_EOF)   ? "event_cb:EOF" :
                         (events & BEV_EVENT_ERROR) ? "event_cb:ERROR" :
                                                      "event_cb:TIMEOUT";
    if (conn->has_pending_workers()) {
      conn->start_deferred_destruction(reason);
      return;
    }
    conn->server->remove_connection(conn);
    delete conn;
  }
}

// goaway_and_drain_cb
// Runs on the event loop thread (scheduled via event_base_once() in stop()).
// Stops accepting new connections, sends GOAWAY to all active connections,
// then starts a drain timer.

void http2_server::goaway_and_drain_cb(
    evutil_socket_t /*fd*/, short /*what*/, void* arg) {
  auto* server = static_cast<http2_server*>(arg);

  // Mark the server as shutting down so new async dispatches fall back to
  // the synchronous path (no new work items enqueued after this point).
  server->shutting_down_ = true;

  // Step 1: Stop accepting new connections.
  if (server->listener_) {
    evconnlistener_disable(server->listener_);
  }

  // Step 2: Drain the thread pool — blocks briefly until in-flight workers
  // finish (acceptable for SBI handlers, which complete quickly).
  if (server->pool_) {
    server->pool_->shutdown();
  }

  // Step 3: Send GOAWAY to every active connection.
  // The mutex is held briefly to iterate the map.  Since we are on the event
  // loop thread, no bufferevent callbacks can fire concurrently — libevent is
  // single-threaded, so there is no deadlock risk.
  {
    std::lock_guard<std::mutex> lock(server->connections_mutex_);
    for (auto& [id, conn] : server->connections_) {
      if (conn->session) {
        nghttp2_submit_goaway(
            conn->session, NGHTTP2_FLAG_NONE,
            nghttp2_session_get_last_proc_stream_id(conn->session),
            NGHTTP2_NO_ERROR, nullptr, 0);
        nghttp2_session_send(conn->session);
      }
    }
  }

  // Step 4: Start the drain timer.  In-flight streams may complete before it
  // fires; remaining connections are force-closed in drain_timer_cb.
  struct timeval tv;
  tv.tv_sec            = server->config_.shutdown_drain_timeout_sec;
  tv.tv_usec           = 0;
  server->drain_timer_ = evtimer_new(server->base_, drain_timer_cb, server);
  evtimer_add(server->drain_timer_, &tv);
}

// drain_timer_cb
// Fires after shutdown_drain_timeout_sec.  Force-closes any remaining
// connections and breaks the event loop, allowing start() to return.

void http2_server::drain_timer_cb(
    evutil_socket_t /*fd*/, short /*what*/, void* arg) {
  auto* server = static_cast<http2_server*>(arg);

  server->close_all_connections();
  event_base_loopbreak(server->base_);
}

// ---------------------------------------------------------------------------
// Deferred connection destruction
//       Safety-timeout callback + http2_connection::start_deferred_destruction
// ---------------------------------------------------------------------------

// Internal context passed to deferred_destruction_timeout_cb via
// event_base_once().  Heap-allocated by start_deferred_destruction(); freed
// inside the callback regardless of which code path is taken.
struct deferred_destruction_ctx {
  uint64_t conn_id;
  http2_server* server;
};

// Safety timeout: force-destroys a connection that has been in
// pending_destruction state for DEFERRED_DESTRUCTION_TIMEOUT_SEC seconds.
// Runs on the event loop thread (scheduled via event_base_once()).
//
// Note: this timeout only fires if the connection entered deferred-destruction
// state AND the timer was successfully scheduled via
// start_deferred_destruction(). If event_base_once failed to post a worker
// result BEFORE deferred-destruction was entered, this timeout offers no
// coverage — the stream remains pending until another lifecycle event (e.g.,
// server shutdown via close_all_connections()).
void http2_server::deferred_destruction_timeout_cb(
    evutil_socket_t /*fd*/, short /*what*/, void* arg) {
  auto* ctx    = static_cast<deferred_destruction_ctx*>(arg);
  uint64_t id  = ctx->conn_id;
  auto* server = ctx->server;
  delete ctx;  // always freed regardless of lookup outcome

  http2_connection* conn = server->find_connection(id);
  if (!conn) {
    srv_log().debug(
        "HTTP2 conn %llu: deferred destruction timeout — connection already "
        "removed",
        static_cast<unsigned long long>(id));
    return;
  }

  if (!conn->pending_destruction) {
    srv_log().debug(
        "HTTP2 conn %llu: deferred destruction timeout — not in deferred "
        "state, skipping",
        static_cast<unsigned long long>(id));
    return;
  }

  srv_log().warn(
      "HTTP2 conn %llu: deferred destruction timeout expired — "
      "force-destroying;"
      " discards: conn_missing=%d stream_missing=%d"
      " stream_closed_pending=%d pending_dest=%d",
      static_cast<unsigned long long>(id), conn->discard_conn_missing,
      conn->discard_stream_missing, conn->discard_stream_closed_pending,
      conn->discard_pending_dest);

  server->remove_connection(conn);
  delete conn;
}

// Out-of-class definition of http2_connection::start_deferred_destruction().
// Called from event_cb / read_cb when has_pending_workers() is true.
void http2_connection::start_deferred_destruction(const char* reason) {
  if (pending_destruction) return;  // guard: no duplicate scheduling

  pending_destruction = true;

  // Stop new I/O: detach bufferevent callbacks and disable read/write events
  // so no further nghttp2 processing occurs on this logically dead connection.
  bufferevent_setcb(bev, nullptr, nullptr, nullptr, nullptr);
  bufferevent_disable(bev, EV_READ | EV_WRITE);

  srv_log().warn(
      "HTTP2 conn %llu: deferring destruction, reason=%s,"
      " waiting for pending workers",
      static_cast<unsigned long long>(conn_id), reason);

  auto* ctx = new deferred_destruction_ctx{conn_id, server};
  struct timeval tv;
  tv.tv_sec  = DEFERRED_DESTRUCTION_TIMEOUT_SEC;
  tv.tv_usec = 0;
  if (event_base_once(
          server->base(), -1, EV_TIMEOUT,
          http2_server::deferred_destruction_timeout_cb, ctx, &tv) != 0) {
    srv_log().error(
        "HTTP2 conn %llu: event_base_once failed for deferred destruction "
        "timeout — connection may leak",
        static_cast<unsigned long long>(conn_id));
    delete ctx;
  }
}

// ---------------------------------------------------------------------------
// Connection tracking helpers
// ---------------------------------------------------------------------------

void http2_server::add_connection(http2_connection* conn) {
  std::lock_guard<std::mutex> lock(connections_mutex_);
  connections_[conn->conn_id] = conn;
}

// ---------------------------------------------------------------------------
void http2_server::remove_connection(http2_connection* conn) {
  std::lock_guard<std::mutex> lock(connections_mutex_);
  connections_.erase(conn->conn_id);
}

// ---------------------------------------------------------------------------
void http2_server::close_all_connections() {
  std::lock_guard<std::mutex> lock(connections_mutex_);
  for (auto& [id, conn] : connections_) {
    delete conn;
  }
  connections_.clear();
}

// ---------------------------------------------------------------------------
http2_connection* http2_server::find_connection(uint64_t conn_id) {
  std::lock_guard<std::mutex> lock(connections_mutex_);
  auto it = connections_.find(conn_id);
  return (it != connections_.end()) ? it->second : nullptr;
}
