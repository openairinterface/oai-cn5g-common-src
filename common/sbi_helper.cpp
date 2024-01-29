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

#include "sbi_helper.hpp"

#include <fmt/format.h>
#include <regex>

using namespace oai::common::sbi;

//---------------------------------------------------------------------------------------------
void sbi_helper::get_nrf_nfm_api_root(
    const nf_addr_t& nrf_addr, std::string& api_root) {
  api_root = nrf_addr.uri_root + sbi_helper::NrfNfmBase + nrf_addr.api_version;
}

//---------------------------------------------------------------------------------------------
void sbi_helper::get_nrf_nf_instance_uri(
    const nf_addr_t& nrf_addr, const std::string& nf_instance,
    std::string& uri) {
  std::string nrf_api_root = {};
  get_nrf_nfm_api_root(nrf_addr, nrf_api_root);
  std::string path_nf_instance_id = {};
  get_fmt_format_form(
      sbi_helper::NrfNfmPathNfInstancesNfInstanceId, path_nf_instance_id);
  uri = nrf_api_root + fmt::format(path_nf_instance_id, nf_instance);
}

//---------------------------------------------------------------------------------------------
void sbi_helper::get_nrf_disc_api_root(
    const nf_addr_t& nrf_addr, std::string& api_root) {
  api_root = nrf_addr.uri_root + sbi_helper::NrfDiscBase + nrf_addr.api_version;
}

//---------------------------------------------------------------------------------------------
void sbi_helper::get_nrf_disc_search_nf_instances_uri(
    const nf_addr_t& nrf_addr, std::string& uri) {
  std::string api_root = {};
  get_nrf_disc_api_root(nrf_addr, api_root);
  uri = api_root + NrfDiscPathNfInstances;
}

//---------------------------------------------------------------------------------------------
void sbi_helper::get_fmt_format_form(
    const std::string& input_str, std::string& output_str) {
  // First replace request parameters (except the last one) with {}
  std::regex e_parameter("\\:[a-zA-Z0-9]+\\/");
  std::string tmp = std::regex_replace(
      input_str, e_parameter, "{}/", std::regex_constants::match_any);

  // Replace the last request parameter with {}
  std::regex e_last_parameter("\\:[a-zA-Z0-9]+");
  output_str = std::regex_replace(
      tmp, e_last_parameter, "{}", std::regex_constants::match_any);
}
