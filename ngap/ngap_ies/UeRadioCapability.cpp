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

#include "UeRadioCapability.hpp"

#include "ngap_utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
UeRadioCapability::UeRadioCapability() {}

/*
UeRadioCapability::UeRadioCapability(const OCTET_STRING_t& capability) {
          if (!capability.buf) return;
          ngap_utils::bstring_2_octet_string(m_UeRadioCapability, capability);
}

UeRadioCapability::UeRadioCapability(const bstring& capability) {
        ngap_utils::bstring_2_octet_string(capability, m_UeRadioCapability);
}
*/
//------------------------------------------------------------------------------
UeRadioCapability::~UeRadioCapability() {}

//------------------------------------------------------------------------------
bool UeRadioCapability::encode(
    Ngap_UERadioCapability_t& ueRadioCapability) const {
  return ngap_utils::bstring_2_octet_string(
      m_UeRadioCapability, ueRadioCapability);
}

//------------------------------------------------------------------------------
bool UeRadioCapability::decode(
    const Ngap_UERadioCapability_t& ueRadioCapability) {
  if (!ueRadioCapability.buf) return false;
  return ngap_utils::octet_string_2_bstring(
      ueRadioCapability, m_UeRadioCapability);
}

//------------------------------------------------------------------------------
bool UeRadioCapability::set(const OCTET_STRING_t& capability) {
  ngap_utils::octet_string_2_bstring(capability, m_UeRadioCapability);
  return true;
}

//------------------------------------------------------------------------------
bool UeRadioCapability::get(OCTET_STRING_t& capability) const {
  ngap_utils::bstring_2_octet_string(m_UeRadioCapability, capability);
  return true;
}

//------------------------------------------------------------------------------
bool UeRadioCapability::set(const bstring& capability) {
  m_UeRadioCapability = bstrcpy(capability);
  return true;
}

//------------------------------------------------------------------------------
bool UeRadioCapability::get(bstring& capability) const {
  capability = bstrcpy(m_UeRadioCapability);
  return true;
}

}  // namespace oai::ngap
