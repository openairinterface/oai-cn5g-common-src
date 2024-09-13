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

#include "QosRule.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger_base.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
QosRule::QosRule() : length_(kQosRuleMinimumLength) {}

//------------------------------------------------------------------------------
QosRule::~QosRule() {}

//------------------------------------------------------------------------------
uint16_t QosRule::GetLength() const {
  return length_;
}

//------------------------------------------------------------------------------
void QosRule::SetQosRuleId(uint8_t rule_id) {
  qos_rule_id_ = rule_id;
}

//------------------------------------------------------------------------------
void QosRule::GetQosRuleId(uint8_t& rule_id) const {
  rule_id = qos_rule_id_;
}

//------------------------------------------------------------------------------
uint8_t QosRule::GetQosRuleId() const {
  return qos_rule_id_;
}

//------------------------------------------------------------------------------
void QosRule::SetRuleOperationCode(uint8_t code) {
  rule_operation_code_ = code & 0x07;  // 3 bits
}

//------------------------------------------------------------------------------
void QosRule::GetRuleOperationCode(uint8_t& code) const {
  code = rule_operation_code_;
}

//------------------------------------------------------------------------------
uint8_t QosRule::GetRuleOperationCode() const {
  return rule_operation_code_;
}

//------------------------------------------------------------------------------
void QosRule::SetDqrBit(bool dqr) {
  dqr_bit_ = dqr;
}
//------------------------------------------------------------------------------
void QosRule::GetDqrBit(bool& dqr) const {
  dqr = dqr_bit_;
}

//------------------------------------------------------------------------------
bool QosRule::GetDqrBit() const {
  return dqr_bit_;
}

//------------------------------------------------------------------------------
void QosRule::SetNumberOfPacketFilter(uint8_t no_pf) {
  number_of_packet_filters_ = no_pf & 0x0f;  // 4 bits
}

//------------------------------------------------------------------------------
void QosRule::GetNumberOfPacketFilter(uint8_t& no_pf) const {
  no_pf = number_of_packet_filters_;
}
//------------------------------------------------------------------------------
uint8_t QosRule::GetNumberOfPacketFilter() const {
  return number_of_packet_filters_;
}

//------------------------------------------------------------------------------
void QosRule::SetPacketFilterModifyAndDeleteList(
    const std::vector<PacketFilterModifyAndDelete>& list) {
  pf_modify_and_delete_list_ =
      std::make_optional<std::vector<PacketFilterModifyAndDelete>>(list);
}

//------------------------------------------------------------------------------
void QosRule::GetPacketFilterModifyAndDeleteList(
    std::optional<std::vector<PacketFilterModifyAndDelete>>& list) const {
  list = pf_modify_and_delete_list_;
}
//------------------------------------------------------------------------------
std::optional<std::vector<PacketFilterModifyAndDelete>>
QosRule::GetPacketFilterModifyAndDeleteList() const {
  return pf_modify_and_delete_list_;
}

//------------------------------------------------------------------------------
void QosRule::SetPacketFilterCreateAndModifyAndReplaceList(
    const std::vector<PacketFilterCreateAndModifyAndReplace>& list) {
  pf_create_and_modify_and_replace_list_ =
      std::make_optional<std::vector<PacketFilterCreateAndModifyAndReplace>>(
          list);
}

//------------------------------------------------------------------------------
void QosRule::GetPacketFilterCreateAndModifyAndReplaceList(
    std::optional<std::vector<PacketFilterCreateAndModifyAndReplace>>& list)
    const {
  list = pf_create_and_modify_and_replace_list_;
}

std::optional<std::vector<PacketFilterCreateAndModifyAndReplace>>
QosRule::GetPacketFilterCreateAndModifyAndReplaceList() const {
  return pf_create_and_modify_and_replace_list_;
}

//------------------------------------------------------------------------------
void QosRule::SetPrecedence(uint8_t precedence) {
  precedence_ = precedence;
}

//------------------------------------------------------------------------------
void QosRule::GetPrecedence(uint8_t& precedence) const {
  precedence = precedence_;
}
//------------------------------------------------------------------------------
uint8_t QosRule::GetPrecedence() const {
  return precedence_;
}

//------------------------------------------------------------------------------
void QosRule::SetSegregation(bool segregation) {
  segregation_ = segregation;
}

//------------------------------------------------------------------------------
void QosRule::GetSegregation(bool& segregation) const {
  segregation = segregation_;
}

//------------------------------------------------------------------------------
bool QosRule::GetSegregation() const {
  return segregation_;
}

//------------------------------------------------------------------------------
void QosRule::SetQfi(uint8_t qfi) {
  qfi_ = qfi & 0x3f;  // 10 bits
}

//------------------------------------------------------------------------------
void QosRule::GetQfi(uint8_t& qfi) const {
  qfi = qfi_;
}

//------------------------------------------------------------------------------
uint8_t QosRule::GetQfi() const {
  return qfi_;
}

//------------------------------------------------------------------------------
int QosRule::Encode(uint8_t* buf, int len) const {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoding QosRule");

  int ie_len = dnn_.GetIeLength();
  ie_len += ta_list_.GetIeLength();

  if (len < ie_len) {  // Length of the content + IEI/Len
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error(
            "Size of the buffer is not enough to store this IE (IE len %d)",
            ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size    = 0;
  int encoded_ie_size = 0;
  // TODO:

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoded QosRule, len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int QosRule::Decode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoding QosRule");
  int decoded_size = 0;
  // TODO:

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoded QosRule (len %d)", decoded_size);
  return decoded_size;
}
