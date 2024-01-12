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
  uri = nrf_api_root +
        fmt::format(sbi_helper::NrfNfmPathNfInstancesNfInstanceId, nf_instance);
}
