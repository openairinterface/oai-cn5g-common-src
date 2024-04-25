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
 \email: Tien-Thinh.Nguyen@eurecom.fr, stefan.spettel@phine.tech,
 lukas.rotheneder@phine.tech
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
// To read content of the response from NF
static std::size_t callback(
    const char* in, std::size_t size, std::size_t num, std::string* out) {
  const std::size_t totalBytes(size * num);
  out->clear();
  out->append(in, totalBytes);
  return totalBytes;
}

//---------------------------------------------------------------------------------------------
response http_client_iface::send_post(const request& request) {
  return send_http_request(method_e::POST, request);
}

//---------------------------------------------------------------------------------------------
response http_client_iface::send_get(const request& request) {
  return send_http_request(method_e::GET, request);
}

//---------------------------------------------------------------------------------------------
response http_client_iface::send_put(const request& request) {
  return send_http_request(method_e::PUT, request);
}

//---------------------------------------------------------------------------------------------
response http_client_iface::send_patch(const request& request) {
  return send_http_request(method_e::PATCH, request);
}

//---------------------------------------------------------------------------------------------
response http_client_iface::send_delete(const request& request) {
  return send_http_request(method_e::DELETE, request);
}

//---------------------------------------------------------------------------------------------
request http_client_iface::prepare_json_request(
    const std::string& uri, const std::string& body) {
  request req;
  req.uri      = uri;
  json::json j = json::json::parse(body);
  req.body     = j.dump();
  req.headers.add<ContentType>(MediaType("application/json"));
  return req;
}

//---------------------------------------------------------------------------------------------
request http_client_iface::prepare_multipart_request(
    const std::string& uri, const std::string& body) {
  request req;
  req.uri  = uri;
  req.body = body;
  req.headers.add<ContentType>(MediaType(
      "multipart/related;boundary=" + std::string(CURL_MIME_BOUNDARY)));
  return req;
}

//---------------------------------------------------------------------------------------------
http_client::~http_client() {
  m_sbi_logger.info("Delete HTTP client instance");
}

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
      "HTTP Client successfully initiated on interface %s with HTTP timeout "
      "%ud s., HTTP version %d",
      m_interface, m_timeout_ms / 1000, m_http_version);

  m_multiPerform = std::make_shared<cpr::MultiPerform>();
}

//---------------------------------------------------------------------------------------------
std::shared_ptr<http_client_iface> http_client::create_instance(
    const oai::logger::printf_logger& logger, int timeout_ms,
    const std::string& interface, uint8_t http_version,
    request_type_e request_type) {
  if (!instance) {
    instance = std::make_shared<http_client>(
        logger, timeout_ms, interface, http_version, request_type);
    return instance;
  }
  return nullptr;
}

//---------------------------------------------------------------------------------------------
std::shared_ptr<http_client_iface> http_client::get_instance() {
  return instance;
}

//---------------------------------------------------------------------------------------------
response http_client::send_http_request(
    const method_e& method, const request& request) {
  m_request_type = request_type_e::MULTI_PERFORM;
  switch (m_request_type) {
    case request_type_e::ASYNC: {
      auto resp = send_async_http_request(method, request);
      return resp;
    } break;
    case request_type_e::MULTI_ASYNC: {
      // TODO:
    } break;

    case request_type_e::MULTI_PERFORM: {
      std::shared_ptr<cpr::MultiPerform> multiPerform =
          std::make_shared<cpr::MultiPerform>();
      auto future =
          send_multi_peform_http_request(method, request, multiPerform);
      future.wait();
      response resp = future.get();
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
response http_client::send_simple_http_request(
    const method_e& method, const request& request) {
  m_sbi_logger.debug("Send a simple HTTP request");

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

  m_sbi_logger.trace(request.to_string() + " (%s)", method_to_string(method));

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
std::future<response> http_client::send_multi_peform_http_request(
    const method_e& method, const request& request,
    const std::shared_ptr<cpr::MultiPerform>& multiPerform) {
  m_sbi_logger.info("Send a MultiPerform HTTP request");
  std::shared_ptr<cpr::Session> session = std::make_shared<cpr::Session>();

  prepare_session(method, request, session);

  // TODO: Should declare a MultiPerform as a class member so that we can
  // actually support multiple sessions per MultiPerform (causing issue when
  // using multi-threading, similar issue with Curl Multi Interface)
  //  std::shared_ptr<cpr::MultiPerform> multiPerform =
  //      std::make_shared<cpr::MultiPerform>();
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

  m_sbi_logger.trace(request.to_string() + " (%s)", method_to_string(method));
  return std::async(
      [this, multiPerform] { return execute_http_request(multiPerform); });
}

//---------------------------------------------------------------------------------------------
response http_client::execute_http_request(
    const std::shared_ptr<cpr::MultiPerform>& multiPerform) {
  response resp;
  if (!multiPerform) {
    resp.status_code = status_code_e::NO_RESPONSE;
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
    resp.status_code = status_code_e::NO_RESPONSE;
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
  // set HTTP version
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

  // set Interface
  session->SetInterface(cpr::Interface{m_interface});

  // set URL
  cpr::Url url = cpr::Url{request.uri};
  session->SetUrl(url);

  // set HTTP client timeout
  session->SetTimeout(cpr::Timeout(m_timeout_ms));

  // set HTTP header (convert pistache header to cpr headers)
  cpr::Header cpr_header{};
  for (const auto& pistache_header : request.headers.list()) {
    std::stringstream ss;
    pistache_header->write(ss);
    cpr_header.insert({pistache_header->name(), ss.str()});
  }
  cpr_header.insert(
      {{"Expect", ""}, {"Accept", "application/json"}, {"Charset", "UTF-8"}});
  session->SetHeader(cpr_header);

  // set HTTP method
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
http_client_curl::http_client_curl(
    oai::logger::printf_logger logger, int timeout_ms,
    const std::string& interface, uint8_t http_version)
    : m_sbi_logger(std::move(logger)) {
  m_http_version = http_version;
  m_timeout_ms   = timeout_ms;
  m_interface    = interface;

  m_sbi_logger.info(
      "HTTP Client CURL successfully initiated on interface %s with HTTP "
      "timeout "
      "%ud s., HTTP version %d",
      m_interface, m_timeout_ms / 1000, m_http_version);
}

//---------------------------------------------------------------------------------------------
bool http_client_curl::initialize() {
  CURLcode code = curl_global_init(CURL_GLOBAL_DEFAULT);
  curl_multi    = curl_multi_init();
  handles       = {};
  headers       = nullptr;
  headers       = curl_slist_append(headers, "Accept: application/json");
  headers       = curl_slist_append(headers, "Charsets: utf-8");
  headers       = curl_slist_append(headers, "Content-Type: application/json");

  if ((code < 0) or (curl_multi == nullptr) or (headers == nullptr)) {
    m_sbi_logger.error("Cannot initialize Curl Multi Interface");
    return false;
  }

  m_sbi_logger.debug("Initialize Curl Multi Interface successful!");
  return true;
}

//---------------------------------------------------------------------------------------------
bool http_client_curl::initialize(
    const oai::logger::printf_logger& logger, int timeout_ms,
    const std::string& interface, uint8_t http_version) {
  m_sbi_logger   = std::move(logger);
  m_http_version = http_version;
  m_timeout_ms   = timeout_ms;
  m_interface    = interface;

  CURLcode code = curl_global_init(CURL_GLOBAL_DEFAULT);
  curl_multi    = curl_multi_init();
  handles       = {};
  headers       = nullptr;
  headers       = curl_slist_append(headers, "Accept: application/json");
  headers       = curl_slist_append(headers, "Charsets: utf-8");
  headers       = curl_slist_append(headers, "Content-Type: application/json");

  if ((code < 0) or (curl_multi == nullptr) or (headers == nullptr)) {
    m_sbi_logger.error("Cannot initialize Curl Multi Interface");
    return false;
  }

  m_sbi_logger.debug("Initialize Curl Multi Interface successful!");
  return true;
}
//---------------------------------------------------------------------------------------------
http_client_curl::~http_client_curl() {
  m_sbi_logger.info("Delete HTTP client CURL instance");
  // Remove handle, free memory
  for (auto h : handles) {
    if (curl_multi) curl_multi_remove_handle(curl_multi, h);
    curl_easy_cleanup(h);
  }
  handles.clear();

  if (curl_multi) curl_multi_cleanup(curl_multi);
  curl_global_cleanup();
  curl_slist_free_all(headers);

  headers == nullptr;
  curl_multi == nullptr;
}

//---------------------------------------------------------------------------------------------
bool http_client_curl::create_instance(
    const oai::logger::printf_logger& logger, int timeout_ms,
    const std::string& interface, uint8_t http_version) {
  if (!instance) {
    instance = std::make_shared<http_client_curl>(
        logger, timeout_ms, interface, http_version);
    return instance->initialize();
  }
  return false;
}

//---------------------------------------------------------------------------------------------
std::shared_ptr<http_client_curl> http_client_curl::get_instance() {
  return instance;
  return nullptr;
}

//---------------------------------------------------------------------------------------------
response http_client_curl::send_http_request(
    const method_e& method, const request& req) {
  // Generate a promise and associate this promise to the curl handle
  uint32_t promise_id = generate_promise_id();
  m_sbi_logger.debug("Promise ID generated %d", promise_id);
  uint32_t* pid_ptr = &promise_id;
  boost::shared_ptr<boost::promise<uint32_t>> p =
      boost::make_shared<boost::promise<uint32_t>>();
  boost::shared_future<uint32_t> f;
  f = p->get_future();
  add_promise(promise_id, p);
  response res = {};
  if (!curl_create_handle(method, req, res, pid_ptr)) {
    m_sbi_logger.warn("Could not create a new handle to send message");
    remove_promise(promise_id);
    pid_ptr = nullptr;
    return res;
  }
  // Wait for the response back
  uint32_t response_code = get_available_response(f);

  m_sbi_logger.debug("Got result for promise ID %d", promise_id);
  m_sbi_logger.debug("Response data %s", res.body.c_str());
  pid_ptr = nullptr;

  return res;
}
//---------------------------------------------------------------------------------------------
uint32_t http_client_curl::get_available_response(
    boost::shared_future<uint32_t>& f) {
  boost::future_status status;
  // wait for timeout or ready
  status = f.wait_for(boost::chrono::milliseconds(m_timeout_ms));
  if (status == boost::future_status::ready) {
    assert(f.is_ready());
    assert(f.has_value());
    assert(!f.has_exception());
    uint32_t response_code = f.get();
    return response_code;
  } else {
    return 408;
  }
}

//---------------------------------------------------------------------------------------------
bool http_client_curl::curl_create_handle(
    const method_e& method, const request& request, response& res,
    uint32_t* promise_id) {
  m_sbi_logger.error("curl_create_handle");
  // Create handle for a curl request
  CURL* curl = curl_easy_init();

  headers = nullptr;
  headers = curl_slist_append(headers, "Accept: application/json");
  headers = curl_slist_append(headers, "Charsets: utf-8");

  if ((curl == nullptr) or (headers == nullptr)) {
    m_sbi_logger.error("Cannot initialize a new Curl Handle");
    return false;
  }

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_URL, request.uri.c_str());
  curl_easy_setopt(curl, CURLOPT_PRIVATE, promise_id);
  if (method == method_e::POST)
    curl_easy_setopt(curl, CURLOPT_POST, 1);
  else if (method == method_e::PATCH)
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
  else if (method == method_e::PUT)
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
  else
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

  curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, m_timeout_ms);
  curl_easy_setopt(curl, CURLOPT_INTERFACE, m_interface.c_str());

  if (m_http_version == 2) {
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    // We use a self-signed test server, skip verification during debugging
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(
        curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE);
  }

  // Hook up data handling function.
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res.body);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request.body.length());
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.body.c_str());

  mtx.lock();
  // Add to the multi handle
  curl_multi_add_handle(curl_multi, curl);
  handles.push_back(curl);
  // The curl cmd will actually be performed in perform_curl_multi
  perform_curl_multi(
      0);  // TODO: current time as parameter if curl is performed per event
  mtx.unlock();

  return true;
}

//------------------------------------------------------------------------------
void http_client_curl::perform_curl_multi(uint64_t ms) {
  //_unused(ms);
  int still_running = 0;
  int numfds        = 0;

  m_sbi_logger.error("perform_curl_multi");
  CURLMcode code = curl_multi_perform(curl_multi, &still_running);

  do {
    code = curl_multi_wait(curl_multi, NULL, 0, 200000, &numfds);
    if (code != CURLM_OK) {
      // Logger::smf_app().debug("curl_multi_wait() returned %d!", code);
    }
    curl_multi_perform(curl_multi, &still_running);
  } while (still_running);

  curl_release_handles();
}

//------------------------------------------------------------------------------
void http_client_curl::curl_release_handles() {
  CURLMsg* curl_msg = nullptr;
  CURL* curl        = nullptr;
  CURLcode code     = {};
  int http_code     = 0;
  int msgs_left     = 0;

  while ((curl_msg = curl_multi_info_read(curl_multi, &msgs_left))) {
    if (curl_msg && curl_msg->msg == CURLMSG_DONE) {
      curl = curl_msg->easy_handle;
      code = curl_msg->data.result;

      if (code != CURLE_OK) {
        m_sbi_logger.debug("CURL error code  %d!", curl_msg->data.result);

      } else {
        // Get HTTP code
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        m_sbi_logger.debug("Got response with HTTP code  %d!", http_code);
        uint32_t* promise_id = nullptr;
        curl_easy_getinfo(curl, CURLINFO_PRIVATE, &promise_id);
        if (promise_id) {
          m_sbi_logger.debug(
              "Prepare to make promise id %d ready!", *promise_id);
          // trigger_process_response(*promise_id, http_code);
        }
      }

      curl_multi_remove_handle(curl_multi, curl);
      curl_easy_cleanup(curl);

      std::vector<CURL*>::iterator it;
      it = find(handles.begin(), handles.end(), curl);
      if (it != handles.end()) {
        handles.erase(it);
      }

    } else if (curl_msg) {
      curl = curl_msg->easy_handle;
      m_sbi_logger.debug("Error after curl_multi_info_read()");
      curl_multi_remove_handle(curl_multi, curl);
      curl_easy_cleanup(curl);

      std::vector<CURL*>::iterator it;
      it = find(handles.begin(), handles.end(), curl);
      if (it != handles.end()) {
        handles.erase(it);
      }
    } else {
      m_sbi_logger.debug("curl_msg null");
    }
  }
}

//---------------------------------------------------------------------------------------------
void http_client_curl::add_promise(
    uint32_t id, boost::shared_ptr<boost::promise<uint32_t>>& p) {
  std::unique_lock lock(m_curl_handle_promises);
  curl_handle_promises.emplace(id, p);
}

//---------------------------------------------------------------------------------------------
void http_client_curl::remove_promise(uint32_t id) {
  std::unique_lock lock(m_curl_handle_promises);
  curl_handle_promises.erase(id);
}
