/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#pragma once
#define FMT_HEADER_ONLY

#include <fmt/format.h>

#include <map>

#include <nlohmann/json.hpp>

#include <string>

#include "3gpp_29.500.h"

namespace oai::http {

struct response {
  int status_code;
  std::string body;
  std::map<std::string, std::string> headers;
  nlohmann::json get_json() const {
    nlohmann::json json_data = {};
    try {
      json_data = nlohmann::json::parse(body);
    } catch (nlohmann::json::exception& e) {
    }
    return json_data;
  }
};

struct request {
  std::string uri;
  std::string body;
  std::map<std::string, std::string> headers;

  std::string to_string() const {
    return fmt::format("HTTP Request to URI: {}", uri);
  }
};

enum class request_type_e { SIMPLE, ASYNC, MULTI_ASYNC, MULTI_PERFORM };

constexpr auto kSbiResponseJsonData         = "jsonData";
constexpr auto kSbiResponseHttpResponseCode = "httpResponseCode";
constexpr auto kSbiResponseHeaderLocation   = "httpResponseLocation";

}  // namespace oai::http
