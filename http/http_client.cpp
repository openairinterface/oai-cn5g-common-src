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

#include "http_client.hpp"

#include <boost/algorithm/string/split.hpp>
#include <nlohmann/json.hpp>
#include <sstream>

namespace json = nlohmann;
using namespace oai::http;
using namespace Pistache::Http::Header;
using namespace Pistache::Http::Mime;

//---------------------------------------------------------------------------------------------
http_client::http_client(
    oai::logger::printf_logger logger, int timeout_ms,
    const std::string& interface, uint8_t http_version,
    request_type_e request_type)
    : m_sbi_logger(std::move(logger)) {
  m_http_version = http_version;
  m_timeout_ms   = timeout_ms;
  m_interface    = interface;
  m_request_type = request_type;

  m_sbi_logger.info(
      "HTTP Client successfully initiated on interface %s with timeout "
      "%ld ms, HTTP version %d",
      m_interface, m_timeout_ms, m_http_version);
}

//---------------------------------------------------------------------------------------------
http_client::~http_client() {
  m_sbi_logger.info("Delete HTTP client instance");
}

//---------------------------------------------------------------------------------------------
response http_client::send_http_request(
    const method_e& method, const request& request) {
  switch (m_request_type) {
    case request_type_e::ASYNC: {
      auto resp = send_async_http_request(method, request);
      return resp;
    } break;
    case request_type_e::MULTI_ASYNC: {
      // TODO:Use simple HTTP request for the moment
    } break;

    case request_type_e::MULTI_PERFORM: {
      // TODO: Should declare a MultiPerform as a class member so that we can
      // actually support multiple sessions per MultiPerform (causing issue when
      // using multi-threading, similar issue with Curl Multi Interface)
      std::shared_ptr<cpr::MultiPerform> multiPerform =
          std::make_shared<cpr::MultiPerform>();
      add_session_to_multi_peform(method, request, multiPerform);
      auto resp = send_multi_peform_http_request(multiPerform);
      return resp;
    } break;
    case request_type_e::SIMPLE:
    default: {  // Use simple HTTP request
    }
  };

  // By default using a simple HTTP request
  auto resp = send_simple_http_request(method, request);
  return resp;
}

//---------------------------------------------------------------------------------------------
response http_client::send_multi_peform_http_request(
    const std::shared_ptr<cpr::MultiPerform>& multiPerform) {
  auto future = std::async(
      [this, multiPerform] { return execute_http_request(multiPerform); });
  future.wait();
  response resp = future.get();
  return resp;
}

//---------------------------------------------------------------------------------------------
response http_client::send_simple_http_request(
    const method_e& method, const request& request) {
  // m_sbi_logger.debug("Send a simple HTTP request");

  std::shared_ptr<cpr::Session> session = std::make_shared<cpr::Session>();
  response resp                         = {};
  cpr::Response cpr_resp                = {};

  prepare_session(method, request, session);

  // set HTTP method
  switch (method) {
    case method_e::POST: {
      cpr_resp = session->Post();
    } break;
    case method_e::GET: {
      cpr_resp = session->Get();
    } break;
    case method_e::PUT: {
      cpr_resp = session->Put();
    } break;
    case method_e::PATCH: {
      cpr_resp = session->Patch();
    } break;
    case method_e::DELETE: {
      cpr_resp = session->Delete();
    }
  }

  resp.status_code = status_code_e(cpr_resp.status_code);
  resp.body        = cpr_resp.text;

  // m_sbi_logger.trace(request.to_string() + " (%s)",
  // method_to_string(method));

  return resp;
}

//---------------------------------------------------------------------------------------------
response http_client::send_async_http_request(
    const method_e& method, const request& request) {
  m_sbi_logger.info("Send an async HTTP request");

  std::shared_ptr<cpr::Session> session = std::make_shared<cpr::Session>();
  response resp                         = {};
  cpr::Response cpr_resp                = {};

  prepare_session(method, request, session);

  switch (method) {
    case method_e::POST: {
      cpr::AsyncResponse async_response = session->PostAsync();
      cpr_resp                          = async_response.get();
    } break;
    case method_e::GET: {
      cpr::AsyncResponse async_response = session->GetAsync();
      cpr_resp                          = async_response.get();
    } break;
    case method_e::PUT: {
      cpr::AsyncResponse async_response = session->PutAsync();
      cpr_resp                          = async_response.get();
    } break;
    case method_e::PATCH: {
      cpr::AsyncResponse async_response = session->PatchAsync();
      cpr_resp                          = async_response.get();
    } break;
    case method_e::DELETE: {
      cpr::AsyncResponse async_response = session->DeleteAsync();
      cpr_resp                          = async_response.get();
    }
  }

  resp.status_code = status_code_e(cpr_resp.status_code);
  resp.body        = cpr_resp.text;

  m_sbi_logger.trace(request.to_string() + " (%s)", method_to_string(method));

  return resp;
}

//---------------------------------------------------------------------------------------------
void http_client::add_session_to_multi_peform(
    const method_e& method, const request& request,
    const std::shared_ptr<cpr::MultiPerform>& multiPerform) {
  // m_sbi_logger.info("Send a MultiPerform HTTP request");
  std::shared_ptr<cpr::Session> session = std::make_shared<cpr::Session>();

  prepare_session(method, request, session);

  // set HTTP method
  switch (method) {
    case method_e::POST: {
      session->SetBody(cpr::Body{request.body});
      multiPerform->AddSession(
          session, cpr::MultiPerform::HttpMethod::POST_REQUEST);
    } break;
    case method_e::GET: {
      multiPerform->AddSession(
          session, cpr::MultiPerform::HttpMethod::GET_REQUEST);
    } break;
    case method_e::PUT: {
      session->SetBody(cpr::Body{request.body});
      multiPerform->AddSession(
          session, cpr::MultiPerform::HttpMethod::PUT_REQUEST);
    } break;
    case method_e::PATCH: {
      session->SetBody(cpr::Body{request.body});

      multiPerform->AddSession(
          session, cpr::MultiPerform::HttpMethod::PATCH_REQUEST);
    } break;
    case method_e::DELETE: {
      multiPerform->AddSession(
          session, cpr::MultiPerform::HttpMethod::DELETE_REQUEST);
    }
  }
}

//---------------------------------------------------------------------------------------------
response http_client::execute_http_request(
    const std::shared_ptr<cpr::MultiPerform>& multiPerform) {
  response resp;
  if (!multiPerform) {
    resp.status_code = status_code_e::HTTP_STATUS_CODE_0_NO_RESPONSE;
    return resp;
  }

  std::vector<cpr::Response> responses = multiPerform->Perform();

  if (responses.size() != 1) {
    // In case multiPerform->Perform() returns also responses from a parallel
    // executed multiPerform->Perform() call. As far as I know, and based on
    // tests, this situation should never occur.
    m_sbi_logger.error(
        "The number of responses is %i and not 1 as expected. The correct "
        "response cannot be identified.",
        responses.size());
    resp.status_code = status_code_e::HTTP_STATUS_CODE_0_NO_RESPONSE;
  } else {
    auto cpr_resp    = responses.at(0);
    resp.status_code = status_code_e(cpr_resp.status_code);
    resp.body        = cpr_resp.text;

    // convert cpr header to pistache headers
    for (const auto& h : cpr_resp.header) {
      try {
        auto pistache_hdr =
            Pistache::Http::Header::Registry::instance().makeHeader(h.first);
        pistache_hdr->parse(h.second);
        resp.headers.add(std::move(pistache_hdr));
      } catch (std::exception&) {
        m_sbi_logger.debug(
            "Unknown header from HTTP client: '%s : %s'", h.first, h.second);
      }
    }
  }
  return resp;
}

//---------------------------------------------------------------------------------------------
void http_client::prepare_session(
    const method_e& method, const request& request,
    std::shared_ptr<cpr::Session>& session) {
  // Set HTTP version
  switch (m_http_version) {
    case 1:
      session->SetHttpVersion(
          cpr::HttpVersion(cpr::HttpVersionCode::VERSION_1_1));
      break;
    case 2:
      session->SetHttpVersion(
          cpr::HttpVersion(cpr::HttpVersionCode::VERSION_2_0_PRIOR_KNOWLEDGE));
      break;
  }

  // Set Interface
  session->SetInterface(cpr::Interface{m_interface});

  // Set URL
  cpr::Url url = cpr::Url{request.uri};
  session->SetUrl(url);

  // Set HTTP client timeout
  session->SetTimeout(cpr::Timeout(m_timeout_ms));

  // Set HTTP header (convert pistache header to cpr headers)
  cpr::Header cpr_header{};
  for (const auto& pistache_header : request.headers.list()) {
    std::stringstream ss;
    pistache_header->write(ss);
    cpr_header.insert({pistache_header->name(), ss.str()});
  }
  cpr_header.insert(
      {{"Expect", ""}, {"Accept", "application/json"}, {"Charset", "UTF-8"}});
  session->SetHeader(cpr_header);

  // Set HTTP method
  switch (method) {
    case method_e::POST: {
      session->SetBody(cpr::Body{request.body});
    } break;
    case method_e::GET: {
    } break;
    case method_e::PUT: {
      session->SetBody(cpr::Body{request.body});
    } break;
    case method_e::PATCH: {
      session->SetBody(cpr::Body{request.body});
    } break;
    case method_e::DELETE: {
    }
  }
}

//---------------------------------------------------------------------------------------------
request http_client::prepare_json_request(
    const std::string& uri, const std::string& body) {
  request req;
  req.uri      = uri;
  json::json j = json::json::parse(body);
  req.body     = j.dump();
  req.headers.add<ContentType>(MediaType("application/json"));
  return req;
}

//---------------------------------------------------------------------------------------------
request http_client::prepare_multipart_request(
    const std::string& uri, const std::string& body) {
  request req;
  req.uri  = uri;
  req.body = body;
  req.headers.add<ContentType>(MediaType(
      "multipart/related;boundary=" + std::string(CURL_MIME_BOUNDARY)));
  return req;
}
