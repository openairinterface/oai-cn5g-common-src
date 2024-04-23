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

/*! \file http_client.cpp
 \brief
 \author  Tien-Thinh NGUYEN, Stefan Spettel, Lukas Rotheneder
 \company Eurecom, phine.tech
 \date 2023
 \email: Tien-Thinh.Nguyen@eurecom.fr, stefan.spettel@phine.tech, lukas.rotheneder@phine.tech
 */

#include "http_client.hpp"

#include <sstream>
#include <boost/algorithm/string/split.hpp>
#include <nlohmann/json.hpp>

namespace json = nlohmann;
using namespace oai::http;
using namespace Pistache::Http::Header;
using namespace Pistache::Http::Mime;

http_client::http_client(
        oai::logger::printf_logger logger, int timeout_ms,
        const std::string &interface, uint8_t http_version)
        : m_sbi_logger(std::move(logger)) {
  m_http_version = http_version;
  m_timeout_ms = timeout_ms;
  m_interface = interface;

  m_multiPerform = std::make_shared<cpr::MultiPerform>();

  m_sbi_logger.info(
          "HTTP Client successfully initiated on interface %s with HTTP timeout "
          "%ud s., HTTP version %d",
          m_interface, m_timeout_ms / 1000,m_http_version);
}

response http_client::send_post(const request &request) {
  return send_http_request(method_e::POST, request);
}

response http_client::send_get(const request &request) {
  return send_http_request(method_e::GET, request);
}

response http_client::send_put(const request &request) {
  return send_http_request(method_e::PUT, request);
}

response http_client::send_patch(const request &request) {
  return send_http_request(method_e::PATCH, request);
}

response http_client::send_delete(const request &request) {
  return send_http_request(method_e::DELETE, request);
}

http_client::~http_client() {
  m_sbi_logger.info("Delete HTTP client instance");
}

void http_client::create_instance(
        const oai::logger::printf_logger &logger, int timeout_ms,
        const std::string &interface, uint8_t http_version) {
  if (!instance) {
    instance = std::make_shared<http_client>(
            logger, timeout_ms, interface, http_version);
  }
}

std::shared_ptr<http_client_iface> http_client::get_instance() {
  return instance;
}

response http_client::send_http_request(
        const method_e &method, const request &request) {
  auto future = send_http_request_async(method, request);
  future.wait();
  response resp = future.get();
  return resp;
}


std::future<response> http_client::send_http_request_async(
        const method_e &method, const request &request) {

  std::shared_ptr<cpr::Session> session = std::make_shared<cpr::Session>();

  // set HTTP version
  switch (m_http_version) {
    case 1:
      session->SetHttpVersion(cpr::HttpVersion(cpr::HttpVersionCode::VERSION_1_1));
      break;
    case 2:
      session->SetHttpVersion(cpr::HttpVersion(cpr::HttpVersionCode::VERSION_2_0_PRIOR_KNOWLEDGE));
      break;
  }

  // set Interface
  session->SetInterface(cpr::Interface{m_interface});

  // set URL
  cpr::Url url = cpr::Url{request.uri};
  session->SetUrl(url);

  // set HTTP client timeout
  session->SetTimeout(cpr::Timeout(m_timeout_ms));

  // set HTTP header (convert pistache header to cpr headers)
  cpr::Header cpr_header{};
  for (const auto &pistache_header: request.headers.list()) {
    std::stringstream ss;
    pistache_header->write(ss);
    cpr_header.insert({pistache_header->name(), ss.str()});
  }
  cpr_header.insert({{"Expect", ""}, {"Accept", "application/json"}, {"Charset", "UTF-8"}});
  session->SetHeader(cpr_header);

  // set HTTP method
  std::shared_ptr<cpr::MultiPerform> multiPerform = std::make_shared<cpr::MultiPerform>();
  switch (method) {
    case method_e::POST: {
      session->SetBody(cpr::Body{request.body});

      multiPerform->AddSession(session, cpr::MultiPerform::HttpMethod::POST_REQUEST);
    }
    break;
    case method_e::GET: {
      multiPerform->AddSession(session, cpr::MultiPerform::HttpMethod::GET_REQUEST);
    }
    break;
    case method_e::PUT: {
      session->SetBody(cpr::Body{request.body});

      multiPerform->AddSession(session, cpr::MultiPerform::HttpMethod::PUT_REQUEST);
    }
    break;
    case method_e::PATCH: {
      session->SetBody(cpr::Body{request.body});

      multiPerform->AddSession(session, cpr::MultiPerform::HttpMethod::PATCH_REQUEST);
    }
    break;
    case method_e::DELETE: {
      multiPerform->AddSession(session, cpr::MultiPerform::HttpMethod::DELETE_REQUEST);
    }
  }

  m_sbi_logger.trace(request.to_string() + " (%s)", method_to_string(method));
  return std::async([&] { return execute_http_request(multiPerform); });
}

response http_client::execute_http_request(
        const std::shared_ptr<cpr::MultiPerform> &multiPerform) {

  response resp;
  if (!multiPerform) {
    resp.status_code = status_code_e::NO_RESPONSE;
    return resp;
  }

  std::vector<cpr::Response> responses = multiPerform->Perform();

  if (responses.size() != 1) {
    // In case multiPerform->Perform() returns also responses from a parallel executed multiPerform->Perform() call.
    // As far as I know, and based on tests, this situation should never occur.
    m_sbi_logger.error(
            "The number of responses is %i and not 1 as expected. The correct response cannot be identified.",
            responses.size());
    resp.status_code = status_code_e::NO_RESPONSE;
  } else {
    auto cpr_resp = responses.at(0);
    resp.status_code = status_code_e(cpr_resp.status_code);
    resp.body = cpr_resp.text;

    // convert cpr header to pistache headers
    for (const auto &h: cpr_resp.header) {
      try {
        auto pistache_hdr =
                Pistache::Http::Header::Registry::instance().makeHeader(
                        h.first);
        pistache_hdr->parse(h.second);
        resp.headers.add(std::move(pistache_hdr));
      } catch (std::exception &) {
        m_sbi_logger.debug("Unknown header from HTTP client: '%s : %s'", h.first, h.second);
      }
    }
  }
  return resp;
}

request http_client_iface::prepare_json_request(
        const std::string &uri, const std::string& body) {
  request req;
  req.uri = uri;
  json::json j = json::json::parse(body);
  req.body = j.dump();
  req.headers.add<ContentType>(MediaType("application/json"));
  return req;
}

request http_client_iface::prepare_multipart_request(
        const std::string &uri, const std::string &body) {
  request req;
  req.uri = uri;
  req.body = body;
  req.headers.add<ContentType>(MediaType(
          "multipart/related;boundary=" + std::string(CURL_MIME_BOUNDARY)));
  return req;
}
