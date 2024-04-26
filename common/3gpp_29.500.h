/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
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

#ifndef FILE_3GPP_29_500_SEEN
#define FILE_3GPP_29_500_SEEN

#include <stdint.h>

#include <string>
#include <vector>

// TODO: harmonize HTTP Status Code definition

enum class HttpStatusCode : uint16_t {
  k100Continue               = 100,
  k200Ok                     = 200,
  k201Created                = 201,
  k202Accepted               = 202,
  k204NoContent              = 204,
  k300MultipleChoices        = 300,
  k303SeeOther               = 303,
  k307TemporaryRedirect      = 307,
  k308PermanentRedirect      = 308,
  k400BadRequest             = 400,
  k401Unauthorized           = 401,
  k403Forbidden              = 403,
  k404NotFound               = 404,
  k405MethodNotAllowed       = 405,
  k406NotAcceptable          = 406,
  k408RequestTimeout         = 408,
  k409Conflict               = 409,
  k410Gone                   = 410,
  k411LengthRequired         = 411,
  k412Precondition_failed    = 412,
  k413PayloadTooLarge        = 413,
  k414UriTooLong             = 414,
  k415UnsupportedMediaTypeNa = 415,
  k429TooManyRequests        = 429,
  k500InternalServerError    = 500,
  k501NotImplemented         = 501,
  k503ServiceUnavailable     = 503,
  k504GatewayTimeout         = 504
};

namespace oai::http {

enum class method_e { POST, GET, PUT, PATCH, DELETE, OPTIONS };

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
    case method_e::OPTIONS:
      return "OPTIONS";
  }
  return "";
}

enum class status_code_e : uint32_t {
  HTTP_STATUS_CODE_0_NO_RESPONSE                 = 0,
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
  HTTP_STATUS_CODE_504_GATEWAY_TIMEOUT           = 504
};

enum protocol_application_error_e {
  PROTOCOL_APP_ERROR_INVALID_API                      = 0,   // 400 Bad Request
  PROTOCOL_APP_ERROR_INVALID_MSG_FORMAT               = 1,   // 400 Bad Request
  PROTOCOL_APP_ERROR_INVALID_QUERY_PARAM              = 2,   // 400 Bad Request
  PROTOCOL_APP_ERROR_MANDATORY_QUERY_PARAM_INCORRECT  = 3,   // 400 Bad Request
  PROTOCOL_APP_ERROR_OPTIONAL_QUERY_PARAM_INCORRECT   = 4,   // 400 Bad Request
  PROTOCOL_APP_ERROR_MANDATORY_QUERY_PARAM_MISSING    = 5,   // 400 Bad Request
  PROTOCOL_APP_ERROR_MANDATORY_IE_INCORRECT           = 6,   // 400 Bad Request
  PROTOCOL_APP_ERROR_OPTIONAL_IE_INCORRECT            = 7,   // 400 Bad Request
  PROTOCOL_APP_ERROR_MANDATORY_IE_MISSING             = 8,   // 400 Bad Request
  PROTOCOL_APP_ERROR_UNSPECIFIED_MSG_FAILURE          = 9,   // 400 Bad Request
  PROTOCOL_APP_ERROR_MODIFICATION_NOT_ALLOWED         = 10,  // 403 Forbidden
  PROTOCOL_APP_ERROR_SUBSCRIPTION_NOT_FOUND           = 11,  // 404 Not Found
  PROTOCOL_APP_ERROR_RESOURCE_URI_STRUCTURE_NOT_FOUND = 12,  // 404 Not Found
  PROTOCOL_APP_ERROR_INCORRECT_LENGTH       = 13,  // 411 Length Required
  PROTOCOL_APP_ERROR_NF_CONGESTION_RISK     = 14,  // 429 Too Many Requests
  PROTOCOL_APP_ERROR_INSUFFICIENT_RESOURCES = 15,  // 500 Internal Server Error
  PROTOCOL_APP_ERROR_UNSPECIFIED_NF_FAILURE = 16,  // 500 Internal Server Error
  PROTOCOL_APP_ERROR_SYSTEM_FAILURE         = 17,  // 500 Internal Server Error
  PROTOCOL_APP_ERROR_NF_CONGESTION          = 18,  // 503 Service Unavailable
};

static const std::vector<std::string> protocol_application_error_e2str{
    "INVALID_API",
    "INVALID_MSG_FORMAT",
    "INVALID_QUERY_PARAM",
    "MANDATORY_QUERY_PARAM_INCORRECT",
    "OPTIONAL_QUERY_PARAM_INCORRECT",
    "MANDATORY_QUERY_PARAM_MISSING",
    "MANDATORY_IE_INCORRECT",
    "OPTIONAL_IE_INCORRECT",
    "MANDATORY_IE_MISSING",
    "UNSPECIFIED_MSG_FAILURE",
    "MODIFICATION_NOT_ALLOWED",
    "SUBSCRIPTION_NOT_FOUND",
    "RESOURCE_URI_STRUCTURE_NOT_FOUND",
    "INCORRECT_LENGTH ",
    "NF_CONGESTION_RISK",
    "INSUFFICIENT_RESOURCES",
    "UNSPECIFIED_NF_FAILURE",
    "SYSTEM_FAILURE",
    "NF_CONGESTION"};

}  // namespace oai::http

#endif  // FILE_3GPP_29_500_SEEN
