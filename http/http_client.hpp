/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#pragma once

#include <functional>
#include <future>
#include <memory>
#include <optional>
#include <string>

#include "3gpp_29.500.h"
#include "http_definitions.hpp"
#include "logger_base.hpp"

using namespace oai::common::sbi;

namespace oai::http {

const std::string MIME_BOUNDARY = "----Boundary";

// Forward declaration — implementation details hidden in http_client.cpp
class http_client_impl;

// Callback type for async responses. Invoked on an Asio I/O thread.
// The callback must be non-blocking; offload heavy work or UI updates.
using response_cb = std::function<void(response)>;

class http_client : public std::enable_shared_from_this<http_client> {
 private:
  oai::logger::printf_logger m_sbi_logger;
  int m_timeout_ms;
  std::string m_interface;
  uint8_t m_http_version;
  request_type_e m_request_type;
  bool m_enable_tls;
  std::optional<std::string> m_public_key_path;
  inline static std::shared_ptr<http_client> instance;

  // PIMPL: hides all nghttp2/boost internals from this header
  std::unique_ptr<http_client_impl> m_impl;

 public:
  explicit http_client(
      oai::logger::printf_logger logger, int timeout_ms,
      const std::string& interface, uint8_t http_version,
      bool enable_tls             = false,
      request_type_e request_type = request_type_e::SIMPLE);

  virtual ~http_client();

  /*
   * Get a static instance
   * @param [const oai::logger::printf_logger&] logger: a logger
   * @param [int] timeout_ms: HTTP Timeout in ms
   * @param [const std::string&] interface: Interface's name
   * @param [uint8_t] http_version: HTTP version
   * @param [bool] enable_tls: Enable TLS
   * @param [request_type_e] request_type: Type of HTTP Request
   * @return an HTTP Client's instance
   */
  static std::shared_ptr<http_client> create_instance(
      const oai::logger::printf_logger& logger, int timeout_ms,
      const std::string& interface, uint8_t http_version,
      bool enable_tls             = false,
      request_type_e request_type = request_type_e::SIMPLE);

  /*
   * Sends an HTTP request
   * @param [const method_e&] method: HTTP method
   * @param [const request&] request: HTTP Request
   * @return the corresponding Response
   */
  response send_http_request(const method_e& method, const request& request);

  /*
   * Non-blocking HTTP request. Returns immediately; callback fires on an
   * Asio I/O thread when the response arrives (or on error). The callback
   * must be non-blocking. Bearer token must be captured by value before
   * calling and re-set inside the callback if nef_app methods are invoked.
   */
  void send_http_request_async(
      const method_e& method, const request& req, response_cb callback);

  /*
   * Sets the correct headers for a JSON request
   * @param [const std::string&] uri: URI to send the request to
   * @param [const std::string&] body: JSON body
   * @param [const std::string&] content_type: Content-Type header value
   * @return request object
   */
  static request prepare_json_request(
      const std::string& uri, const std::string& body = "",
      const std::string& content_type = "application/json");

  /*
   * Sets the correct headers for a multipart/related request
   * @param [const std::string&] uri: URI to send the request to
   * @param [const std::string&] body: multipart/related body (including
   * boundaries)
   * @return request object
   */
  static request prepare_multipart_request(
      const std::string& uri, const std::string& body);
};

}  // namespace oai::http
