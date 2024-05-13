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
#define FMT_HEADER_ONLY

#include <curl/curl.h>
#include <fmt/format.h>
#include <pistache/http_headers.h>

#include <string>

#include "3gpp_29.500.h"

namespace oai::http {

// Design choice: We use Pistache headers here because they are type-safe
// It breaks a bit the abstraction of HTTP layer, but the advantage is greater
struct response {
  int status_code;
  std::string body;
  Pistache::Http::Header::Collection headers;
};

struct request {
  std::string uri;
  std::string body;
  Pistache::Http::Header::Collection headers;

  std::string to_string() const {
    return fmt::format("HTTP Request to URI: {}", uri);
  }
};

enum class request_type_e { SIMPLE, ASYNC, MULTI_ASYNC, MULTI_PERFORM };

}  // namespace oai::http
