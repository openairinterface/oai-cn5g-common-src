/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#pragma once

#include <nghttp2/asio_http2_server.h>

#include <functional>
#include <string>

#include "logger_base.hpp"

namespace oai::oam {

/**
 * Minimal HTTP/2 server dedicated to OAM interfaces. It allows a network
 * function to serve its management APIs (e.g. the *-oai configuration
 * services) on a dedicated interface, independently from its SBI interface.
 */
class oam_server {
 private:
  std::string m_address;
  uint32_t m_port;
  nghttp2::asio_http2::server::http2 server;
  oai::logger::printf_logger m_logger;
  bool running_server;
  bool m_enable_tls;
  std::string m_key_file;
  std::string m_cert_file;

 public:
  oam_server(
      const std::string& addr, uint32_t port,
      const oai::logger::printf_logger& logger, bool enable_tls = false,
      const std::string& key_file = {}, const std::string& cert_file = {})
      : m_address(addr),
        m_port(port),
        server(),
        m_logger(logger),
        running_server(false),
        m_enable_tls(enable_tls),
        m_key_file(key_file),
        m_cert_file(cert_file) {}
  virtual ~oam_server() = default;

  void start();
  void stop();

  /*
   * Registers a handler for the given path on the OAM server
   * @param [const std::string&] path: the path the handler answers to
   * @param [const request_cb&] handler: the request handler
   */
  void register_handler(
      const std::string& path,
      const nghttp2::asio_http2::server::request_cb& handler);

  /*
   * Gives access to the underlying HTTP/2 server, e.g. to register the
   * routes of an existing API class
   * @return the underlying HTTP/2 server
   */
  nghttp2::asio_http2::server::http2& get_server() { return server; }
};

}  // namespace oai::oam
