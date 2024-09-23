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
void QosRule::SetLength() {
  // Calculate the actual length
  length_ = kQosRuleMinimumLength;
  for (int i = 0; i < number_of_packet_filters_; i++) {
    if (rule_operation_code_ ==
        kQosRuleRuleOperationCodeModifyExistingQosRuleAndDeletePacketFilters) {
      length_++;  // 1 octet for each packet filter
    } else if (
        (rule_operation_code_ == kQosRuleRuleOperationCodeCreateNewQosRule) or
        (rule_operation_code_ ==
         kQosRuleRuleOperationCodeModifyExistingQosRuleAndAddPacketFilters) or
        (rule_operation_code_ ==
         kQosRuleRuleOperationCodeModifyExistingQosRuleAndReplaceAllPacketFilters)) {
      if (pf_create_and_modify_and_replace_list_.has_value()) {
        for (auto p : pf_create_and_modify_and_replace_list_.value()) {
          length_ +=
              1;  // octet 8- packet filter direction + packet filter identifier
          length_ += 1;                 // length of packet filter
          length_ += p.content.length;  // packet filter content
        }
      }
    }
  }
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
  SetLength();
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
  SetLength();
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
  precedence_ = std::make_optional<uint8_t>(precedence);
  SetLength();
}

//------------------------------------------------------------------------------
void QosRule::GetPrecedence(std::optional<uint8_t>& precedence) const {
  precedence = precedence_;
}
//------------------------------------------------------------------------------
std::optional<uint8_t> QosRule::GetPrecedence() const {
  return precedence_;
}

//------------------------------------------------------------------------------
void QosRule::SetSegregation(bool segregation) {
  segregation_ = segregation;
  SetLength();
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
  SetLength();
}

//------------------------------------------------------------------------------
void QosRule::GetQfi(std::optional<uint8_t>& qfi) const {
  qfi = qfi_;
}

//------------------------------------------------------------------------------
std::optional<uint8_t> QosRule::GetQfi() const {
  return qfi_;
}

//------------------------------------------------------------------------------
int QosRule::Encode(uint8_t* buf, int len) const {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoding QosRule");

  int encoded_size = 0;

  // Validate the buffer's length and Encode IEI/Length (later)
  if (len < length_) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error(
            "Buffer length is less than the length of this IE (%d "
            "octet)",
            length_);
    return KEncodeDecodeError;
  }

  // QoS Rule Identifier
  ENCODE_U8(buf + encoded_size, qos_rule_id_, encoded_size);

  // Length
  ENCODE_U16(buf + encoded_size, GetLength(), encoded_size);

  // Octet 7 - Rule operation code, DQR bit, Number of packet filters
  uint8_t octet_7 = ((rule_operation_code_ & 0x07) << 5) | (dqr_bit_ << 4) |
                    (number_of_packet_filters_ & 0x0f);
  ENCODE_U8(buf + encoded_size, octet_7, encoded_size);

  // Paket filter list
  for (int i = 0; i < number_of_packet_filters_; i++) {
    if (rule_operation_code_ ==
        kQosRuleRuleOperationCodeModifyExistingQosRuleAndDeletePacketFilters) {
      if (pf_modify_and_delete_list_.has_value()) {
        for (auto p : pf_modify_and_delete_list_.value()) {
          ENCODE_U8(
              buf + encoded_size, (p.packet_filter_id & 0x0f), encoded_size);
        }
      }

    } else if (
        (rule_operation_code_ == kQosRuleRuleOperationCodeCreateNewQosRule) or
        (rule_operation_code_ ==
         kQosRuleRuleOperationCodeModifyExistingQosRuleAndAddPacketFilters) or
        (rule_operation_code_ ==
         kQosRuleRuleOperationCodeModifyExistingQosRuleAndReplaceAllPacketFilters)) {
      if (pf_create_and_modify_and_replace_list_.has_value()) {
        for (auto p : pf_create_and_modify_and_replace_list_.value()) {
          uint8_t octet_8 =
              ((p.packet_filter_direction & 0x03) << 4) |
              (p.packet_filter_id & 0x0f);  // octet 8- packet filter direction
                                            // + packet filter identifier
          ENCODE_U8(buf + encoded_size, octet_8, encoded_size);
          ENCODE_U8(
              buf + encoded_size, p.content.length,
              encoded_size);  // length of packet filter
          int encoded_content_size = encode_bstring(
              p.content.content, (buf + encoded_size),
              len - encoded_size);  // packet filter content
          encoded_size += encoded_content_size;
        }
      }
    }
  }

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoded QosRule, len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int QosRule::Decode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoding QosRule");
  if (len < kQosRuleMinimumLength) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error(
            "Buffer length is less than the minimum length of this IE (%d "
            "octet)",
            kQosRuleMinimumLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;

  // QoS Rule Identifier
  DECODE_U8(buf + decoded_size, qos_rule_id_, decoded_size);

  // Length
  DECODE_U16(buf + decoded_size, length_, decoded_size);

  // Octet 7 - Rule operation code, DQR bit, Number of packet filters
  uint8_t octet_7 = {};
  DECODE_U8(buf + decoded_size, octet_7, decoded_size);
  rule_operation_code_      = octet_7 >> 5;
  dqr_bit_                  = (octet_7 >> 4) & 0x01;
  number_of_packet_filters_ = octet_7 & 0x0f;

  // Packet filter list

  if (rule_operation_code_ ==
      kQosRuleRuleOperationCodeModifyExistingQosRuleAndDeletePacketFilters) {
    std::vector<PacketFilterModifyAndDelete> pf_modify_and_delete_list;
    for (int i = 0; i < number_of_packet_filters_; i++) {
      PacketFilterModifyAndDelete pf = {};
      DECODE_U8(buf + decoded_size, pf.packet_filter_id, decoded_size);
      pf_modify_and_delete_list.push_back(pf);
    }
    pf_modify_and_delete_list_ =
        std::make_optional<std::vector<PacketFilterModifyAndDelete>>(
            pf_modify_and_delete_list);
  }
  if ((rule_operation_code_ == kQosRuleRuleOperationCodeCreateNewQosRule) or
      (rule_operation_code_ ==
       kQosRuleRuleOperationCodeModifyExistingQosRuleAndAddPacketFilters) or
      (rule_operation_code_ ==
       kQosRuleRuleOperationCodeModifyExistingQosRuleAndReplaceAllPacketFilters)) {
    std::vector<PacketFilterCreateAndModifyAndReplace>
        pf_create_and_modify_and_replace_list;
    for (int i = 0; i < number_of_packet_filters_; i++) {
      PacketFilterCreateAndModifyAndReplace pf = {};
      uint8_t octet_8 = {};  // octet 8- packet filter direction
                             // + packet filter identifier
      DECODE_U8(buf + decoded_size, octet_8, decoded_size);
      pf.packet_filter_direction = (octet_8 >> 4) & 0x03;
      pf.packet_filter_id        = octet_8 & 0x0f;

      DECODE_U8(
          buf + decoded_size, pf.content.length,
          decoded_size);  // length of packet filter

      // Content
      decode_bstring(
          &pf.content.content, pf.content.length, (buf + decoded_size),
          len - decoded_size);
      decoded_size += pf.content.length;
      pf_create_and_modify_and_replace_list.push_back(pf);
    }
    pf_create_and_modify_and_replace_list_ =
        std::make_optional<std::vector<PacketFilterCreateAndModifyAndReplace>>(
            pf_create_and_modify_and_replace_list);
  }

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoded QosRule (len %d)", decoded_size);
  return decoded_size;
}
