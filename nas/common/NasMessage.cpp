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

#include "NasMessage.hpp"

#include "3gpp_24.501.hpp"
#include "NasHelper.hpp"
#include "common_defs.hpp"
#include "logger_base.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
bool NasMessage::Validate(uint32_t len) const {
  uint32_t actual_length = GetLength();
  if (len < actual_length) {
    oai::logger::logger_common::nas().error(
        "Buffer length is less than the minimum length of this message "
        "(0x%x "
        "octet)",
        actual_length);
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void NasMessage::SetMessageName(const std::string& name) {
  msg_name_ = name;
}

//------------------------------------------------------------------------------
std::string NasMessage::GetMessageName() const {
  return msg_name_;
}

//------------------------------------------------------------------------------
void NasMessage::GetMessageName(std::string& name) const {
  name = msg_name_;
}
