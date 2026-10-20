/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "oam_server.hpp"

#include <boost/asio/ssl.hpp>

#include <chrono>
#include <thread>

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;

namespace oai::oam {

//------------------------------------------------------------------------------
void oam_server::register_handler(
    const std::string& path, const request_cb& handler) {
  server.handle(path, handler);
}

//------------------------------------------------------------------------------
void oam_server::start() {
  boost::system::error_code ec;

  boost::asio::ssl::context tls(boost::asio::ssl::context::sslv23);

  if (m_enable_tls) {
    try {
      tls.use_private_key_file(m_key_file, boost::asio::ssl::context::pem);
      tls.use_certificate_chain_file(m_cert_file);
      configure_tls_context_easy(ec, tls);
    } catch (std::exception& e) {
      m_logger.error("%s", e.what());
      m_enable_tls = false;
    }
  }

  m_logger.info("HTTP2 OAM server being started");

  running_server = true;
  if (m_enable_tls) {
    server.listen_and_serve(ec, tls, m_address, std::to_string(m_port));
  } else {
    server.listen_and_serve(ec, m_address, std::to_string(m_port));
  }
  m_logger.error("HTTP2 OAM server status: %s", ec.message());
  running_server = false;
  m_logger.info("HTTP2 OAM server fully stopped");
}

//------------------------------------------------------------------------------
void oam_server::stop() {
  m_logger.debug("HTTP2 OAM server to be stopped");
  server.stop();
  while (running_server) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  m_logger.debug("HTTP2 OAM server should be fully stopped");
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

}  // namespace oai::oam
