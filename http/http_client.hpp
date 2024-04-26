/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#pragma once

#include <cpr/cpr.h>

#include <future>
#include <string>
#include <thread>
#include <unordered_map>

#include "3gpp_29.500.h"
#include "http_definitions.hpp"
#include "logger_base.hpp"

using namespace oai::http;

namespace oai::http {

const std::string CURL_MIME_BOUNDARY = "----Boundary";
const unsigned int HTTP_TIMEOUT_MS   = 10000;

class http_client {
 private:
  response send_simple_http_request(
      const method_e& method, const request& request);

  /**
   * Sends a synchronous HTTP request and waits
   * @param method
   * @param request
   * @return
   */
  response send_async_http_request(
      const method_e& method, const request& request);

  response execute_http_request(
      const std::shared_ptr<cpr::MultiPerform>& multiPerform);

  oai::logger::printf_logger m_sbi_logger;
  int m_timeout_ms;
  std::string m_interface;
  uint8_t m_http_version;
  request_type_e m_request_type;

 public:
  explicit http_client(
      oai::logger::printf_logger logger, int timeout_ms,
      const std::string& interface, uint8_t http_version,
      request_type_e request_type = request_type_e::SIMPLE);

  ~http_client();

  static http_client& get_instance(
      const oai::logger::printf_logger& logger, int timeout_ms,
      const std::string& interface, uint8_t http_version,
      request_type_e request_type = request_type_e::SIMPLE) {
    static http_client instance(
        logger, timeout_ms, interface, http_version, request_type);
    return instance;
  }

  /**
   * Sends a HTTP request
   * @param method
   * @param request
   * @return
   */
  response send_http_request(const method_e& method, const request& request);

  void add_session_to_multi_peform(
      const method_e& method, const request& request,
      const std::shared_ptr<cpr::MultiPerform>& multiPerform);
  response send_multi_peform_http_request(
      const std::shared_ptr<cpr::MultiPerform>& multiPerform);

  void prepare_session(
      const method_e& method, const request& request,
      std::shared_ptr<cpr::Session>& session);
  /**
   * Sets the correct headers for a JSON request
   * @param uri URI to send the request to
   * @param body JSON body
   * @return request object
   */
  static request prepare_json_request(
      const std::string& uri, const std::string& body);

  /**
   * Sets the correct headers for a multipart/related request
   * @param uri URI to send the request to
   * @param body multipart/related body (including boundaries)
   * @return request object
   */
  static request prepare_multipart_request(
      const std::string& uri, const std::string& body);
};
}  // namespace oai::http
