/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef FILE_HTTP2_SERVER_SEEN
#define FILE_HTTP2_SERVER_SEEN

#include <atomic>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include <nghttp2/nghttp2.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "3gpp_29.500.h"
#include "logger_base.hpp"
#include "thread_pool.h"

// Diagnostics logger
//
// This server is shared by every NF, so it must not depend on any single NF's
// Logger facade. It logs through the common logger registry instead: by
// default the "common" category, which every NF registers. An NF that wants
// the server's diagnostics under one of its own categories calls
// set_http2_server_logger() once at startup (before start()); unknown
// category names fall back to "common" inside the registry.
namespace oai::sba {

void set_http2_server_logger(const std::string& logger_name);
const oai::logger::printf_logger& http2_server_logger();

}  // namespace oai::sba

// Forward declarations
struct http2_connection;
struct http2_stream;
struct thread_pool_work_item;
class http2_server;

// Response Body Provider
struct response_body;

// Public Request Type
struct http2_request {
  oai::common::sbi::method_e method =
      oai::common::sbi::method_e::GET;  // GET, PUT, POST, DELETE, PATCH
  std::string path;                     // path component only (before '?')
  std::string
      raw_query;  // query string portion of path (after '?', no leading '?')
  std::string scheme;     // scheme
  std::string authority;  // authority
  std::map<std::string, std::string>
      headers;       // regular headers (all lowercase)
  std::string body;  // accumulated request body (all DATA chunks concatenated)
  int32_t stream_id = 0;
  std::string peer_address;  // TCP peer IP (populated in accept_cb)
};

// Public Response Type
class http2_response {
 public:
  // Write response status + headers + body to the stream.
  // Calls nghttp2_submit_response() and nghttp2_session_send() internally —
  // HPACK encoding completes while all header string data is still on the
  // stack.  Must be called exactly once per request.
  void send(
      int status_code, const std::map<std::string, std::string>& headers,
      const std::string& body);

  // Variant: send with no body (e.g. 204 No Content)
  void send(int status_code, const std::map<std::string, std::string>& headers);

  // Returns true if send() has already been called on this response.
  bool was_sent() const;

  // Create a deferred response handle. Marks this response
  // as deferred so the pool worker does NOT post response_post_cb after the
  // handler returns — the returned handle takes over delivery and may be
  // invoked once, from any thread, after the handler has returned.
  //
  // Only valid in threaded mode (work_item_ != nullptr). In synchronous mode
  // the returned handle is invalid (valid() == false) and send() must be used
  // directly on this response instead.
  class http2_deferred_response make_deferred();

  // Returns true if make_deferred() was called on this response.
  bool was_deferred() const noexcept;

  // Synchronous constructor (event-loop-thread use — session/bev must remain
  // alive for the duration of send()).
  http2_response(
      nghttp2_session* session, int32_t stream_id, struct bufferevent* bev,
      http2_stream* stream);

  // Threaded constructor — response data is captured into the work item and
  // submitted to nghttp2 on the event loop thread via response_post_cb.
  explicit http2_response(thread_pool_work_item* work_item);

 private:
  nghttp2_session* session_ = nullptr;
  int32_t stream_id_        = 0;
  struct bufferevent* bev_  = nullptr;
  http2_stream* stream_     = nullptr;  // for response_body ownership
  bool sent_                = false;    // guard against double-send
  bool was_deferred_        = false;    // handler handed off the work item
  thread_pool_work_item* work_item_ = nullptr;  // non-null in threaded mode
};

// Deferred response handle (detached response).
//
// Created from a threaded-mode http2_response via make_deferred(). Takes over
// the work_item so the route handler may RETURN before the response is
// produced; delivery happens later via send(), which can be called exactly
// once from ANY thread (in particular the http_client Asio I/O thread when a
// southbound response arrives). Internally send() writes the response into the
// work_item — exactly as response_post_cb expects — and posts response_post_cb
// onto the libevent loop via event_base_once().
//
// If the handle is destroyed without send() ever being called, the destructor
// posts a 500 so the client is not left hanging.
//
// Cooperation with the existing safety net: because make_deferred() sets
// was_deferred() on the source response, the pool-worker lambda in
// on_frame_recv_callback must NOT post response_post_cb nor trip the
// was_sent()->500 guard for a deferred handler. The deferred handle owns the
// single posting of response_post_cb. The stream-closed-while-pending /
// connection-gone / deferred-destruction guards inside response_post_cb still
// apply on the event-loop thread, so a stream that closed before send()
// arrives is handled safely there.
class http2_deferred_response {
 public:
  http2_deferred_response()                               = default;
  http2_deferred_response(const http2_deferred_response&) = delete;
  http2_deferred_response& operator=(const http2_deferred_response&) = delete;
  http2_deferred_response(http2_deferred_response&&) noexcept;
  http2_deferred_response& operator=(http2_deferred_response&&) noexcept;
  ~http2_deferred_response();  // posts 500 if never sent

  // Thread-safe, exactly-once. Subsequent calls are no-ops.
  void send(
      int status, std::map<std::string, std::string> headers, std::string body);

  bool valid() const noexcept { return item_ != nullptr; }

 private:
  friend class http2_response;  // make_deferred() constructs these
  http2_deferred_response(
      thread_pool_work_item* item, http2_server* srv) noexcept;

  // Shared implementation of send()/destructor delivery. Performs the CAS on
  // sent_, writes the response into the work item, and posts response_post_cb.
  void deliver(
      int status, std::map<std::string, std::string> headers, std::string body);

  thread_pool_work_item* item_ = nullptr;
  http2_server* srv_           = nullptr;
  std::shared_ptr<std::atomic<bool>>
      sent_;  // shared so moves stay exactly-once
};

// Handler callback type: receives a fully accumulated request + response writer
using http2_handler =
    std::function<void(const http2_request&, http2_response&)>;

// Server Configuration
struct http2_server_config {
  // SETTINGS frame values sent to clients
  uint32_t max_concurrent_streams = 1000;
  uint32_t initial_window_size    = 65535;  // per-stream flow control window
  uint32_t max_header_list_size =
      65536;  // HPACK bomb protection (SETTINGS frame)

  // Application-level limits
  size_t max_request_body_size = 1 * 1024 * 1024;  // 1 MB
  uint32_t max_connections     = 10000;

  // Timeouts (seconds)
  int connection_idle_timeout_sec = 60;
  int shutdown_drain_timeout_sec  = 5;

  // Thread pool (0 = synchronous/event-loop-only mode)
  uint32_t num_worker_threads = 4;
  size_t max_pending_tasks    = 10000;

  // Optional override for the async dispatcher thread-pool size.
  // 0 = auto: keep the default (num_worker_threads + 2). A positive value
  // overrides the dispatcher pool size. Default 0 (auto / unchanged behavior).
  uint32_t dispatcher_pool_size = 0;
};

// Server Class

class http2_server {
 public:
  http2_server(
      const std::string& address, uint32_t port,
      http2_server_config config = {});
  ~http2_server();

  // Non-copyable, non-movable
  http2_server(const http2_server&) = delete;
  http2_server& operator=(const http2_server&) = delete;

  // Register a route handler matched by longest-prefix.
  // Semantics identical to nghttp2-asio server.handle().
  void handle(const std::string& path_prefix, http2_handler handler);

  // Start the server.  Blocks until stop() is called from another thread
  // (runs the libevent event loop internally).
  void start();

  // Graceful shutdown — thread-safe, may be called from any thread.
  // Flow: stop() → event_base_once(goaway_and_drain_cb)
  //             → drain_timer_cb → event_base_loopbreak()
  void stop();

  // Config accessors.
  const http2_server_config& config() const { return config_; }
  http2_server_config& config() { return config_; }

  // Route lookup — public so file-scope nghttp2 callbacks
  // (on_frame_recv_callback etc.) in the .cpp can call
  // conn->server->find_handler() without friendship.
  http2_handler* find_handler(const std::string& path);

  // Thread pool accessors (called from event-loop callbacks).
  bool has_thread_pool() const { return pool_ != nullptr; }
  thread_pool* get_thread_pool() { return pool_.get(); }
  bool is_shutting_down() const { return shutting_down_; }
  struct event_base* base() const {
    return base_;
  }

  // Static callback registered via event_base_once() by worker threads.
  // Must be public so lambdas in file-scope nghttp2 callbacks can reference it.
  static void response_post_cb(evutil_socket_t fd, short what, void* arg);

  // Safety-timeout callback scheduled by start_deferred_destruction().
  // Must be public so http2_connection::start_deferred_destruction() can
  // pass it as a C function pointer to event_base_once().
  static void deferred_destruction_timeout_cb(
      evutil_socket_t fd, short what, void* arg);

 private:
  // libevent primitives
  struct event_base* base_         = nullptr;
  struct evconnlistener* listener_ = nullptr;
  struct event* drain_timer_       = nullptr;

  // server state
  std::string address_;
  uint32_t port_;
  http2_server_config config_;
  std::atomic<bool> running_{false};

  // routing table (sorted longest-prefix-first after start())
  struct Route {
    std::string prefix;
    http2_handler handler;
  };
  std::vector<Route> routes_;

  // active connections (for GOAWAY / graceful shutdown)
  std::mutex connections_mutex_;
  std::unordered_map<uint64_t, http2_connection*> connections_;

  // Internal libevent callbacks (static for C-function-pointer compatibility).
  // NOTE: `static` appears ONLY in these in-class declarations.
  // The out-of-class definitions in http2_server.cpp must NOT use `static`.
  static void accept_cb(
      struct evconnlistener* listener, evutil_socket_t fd,
      struct sockaddr* addr, int addrlen, void* arg);
  static void read_cb(struct bufferevent* bev, void* arg);
  static void event_cb(struct bufferevent* bev, short events, void* arg);
  static void goaway_and_drain_cb(evutil_socket_t fd, short what, void* arg);
  static void drain_timer_cb(evutil_socket_t fd, short what, void* arg);

  // Internal helpers
  void add_connection(http2_connection* conn);
  void remove_connection(http2_connection* conn);
  void close_all_connections();
  http2_connection* find_connection(uint64_t conn_id);

  // Thread pool state (only valid when pool_ != nullptr)
  std::unique_ptr<thread_pool> pool_;
  bool shutting_down_ = false;  // event-loop thread only
  std::atomic<uint64_t> next_conn_id_{1};
};

#endif  // FILE_HTTP2_SERVER_SEEN
