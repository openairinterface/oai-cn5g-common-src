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

/*! \file http_definitions.hpp
 \brief
 \author Stefan Spettel
 \company phine.tech
 \date 2023
 \email: stefan.spettel@phine.tech
*/

#pragma once
#define FMT_HEADER_ONLY

#include <string>
#include <pistache/http_headers.h>
#include <fmt/format.h>
#include <curl/curl.h>

namespace oai::http {

    enum class status_code_e : uint32_t {
        HTTP_STATUS_CODE_100_CONTINUE                  = 100,
        HTTP_STATUS_CODE_200_OK                        = 200,
        HTTP_STATUS_CODE_201_CREATED                   = 201,
        HTTP_STATUS_CODE_202_ACCEPTED                  = 202,
        HTTP_STATUS_CODE_204_NO_CONTENT                = 204,
        HTTP_STATUS_CODE_300_MULTIPLE_CHOICES          = 300,
        HTTP_STATUS_CODE_303_SEE_OTHER                 = 303,
        HTTP_STATUS_CODE_307_TEMPORARY_REDIRECT        = 307,
        HTTP_STATUS_CODE_308_PERMANENT_REDIRECT        = 308,
        HTTP_STATUS_CODE_400_BAD_REQUEST               = 400,
        HTTP_STATUS_CODE_401_UNAUTHORIZED              = 401,
        HTTP_STATUS_CODE_403_FORBIDDEN                 = 403,
        HTTP_STATUS_CODE_404_NOT_FOUND                 = 404,
        HTTP_STATUS_CODE_405_METHOD_NOT_ALLOWED        = 405,
        HTTP_STATUS_CODE_406_NOT_ACCEPTABLE            = 406,
        HTTP_STATUS_CODE_408_REQUEST_TIMEOUT           = 408,
        HTTP_STATUS_CODE_409_CONFLICT                  = 409,
        HTTP_STATUS_CODE_410_GONE                      = 410,
        HTTP_STATUS_CODE_411_LENGTH_REQUIRED           = 411,
        HTTP_STATUS_CODE_412_PRECONDITION_FAILED       = 412,
        HTTP_STATUS_CODE_413_PAYLOAD_TOO_LARGE         = 413,
        HTTP_STATUS_CODE_414_URI_TOO_LONG              = 414,
        HTTP_STATUS_CODE_415_UNSUPPORTED_MEDIA_TYPE_NA = 415,
        HTTP_STATUS_CODE_429_TOO_MANY_REQUESTS         = 429,
        HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR     = 500,
        HTTP_STATUS_CODE_501_NOT_IMPLEMENTED           = 501,
        HTTP_STATUS_CODE_503_SERVICE_UNAVAILABLE       = 503,
        HTTP_STATUS_CODE_504_GATEWAY_TIMEOUT           = 504,
        NO_RESPONSE                                    = 0
    };

    enum class method_e {
        POST, GET, PUT, PATCH, DELETE
    };

    static std::string method_to_string(method_e method) {
      switch (method) {
        case method_e::POST:
          return "POST";
        case method_e::GET:
          return "GET";
        case method_e::PUT:
          return "PUT";
        case method_e::PATCH:
          return "PATCH";
        case method_e::DELETE:
          return "DELETE";
      }
      return "";
    }
// Design choice: We use Pistache headers here because they are type-safe
// It breaks a bit the abstraction of HTTP layer, but the advantage is greater
    struct response {
        status_code_e status_code;
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

}  // namespace oai::http