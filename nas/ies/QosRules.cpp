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

#include "QosRules.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger_base.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
QosRules::QosRules() : Type6NasIe(), qos_rules_() {
  SetLengthIndicator(kQosRulesContentMinimumLength);
}

//------------------------------------------------------------------------------
QosRules::QosRules(uint8_t iei) : Type6NasIe(iei) {
  SetLengthIndicator(kQosRulesContentMinimumLength);
}

//------------------------------------------------------------------------------
QosRules::QosRules(const std::vector<QosRule>& qos_rules) : Type6NasIe() {
  uint32_t length = 0;  // not include 3 first octets: 1 for IE , 2 for length,
  for (auto qos : qos_rules) {
    length += qos.GetLength();
  }
  SetLengthIndicator(
      (length > kQosRulesContentMinimumLength) ? length :
                                                 kQosRulesContentMinimumLength);
}

//------------------------------------------------------------------------------
QosRules::QosRules(uint8_t iei, const std::vector<QosRule>& qos_rules)
    : Type6NasIe(iei) {
  uint32_t length = 0;  // not include 3 first octets: 1 for IE , 2 for length,
  for (auto qos : qos_rules) {
    length += qos.GetLength() +
              3;  // 1 for QoS rule identifier and 2 for length of QoS rule
  }
  SetLengthIndicator(
      (length > kQosRulesContentMinimumLength) ? length :
                                                 kQosRulesContentMinimumLength);
}

//------------------------------------------------------------------------------
QosRules::~QosRules() {}

//------------------------------------------------------------------------------
void QosRules::Set(const std::vector<QosRule>& qos_rules) {
  if (qos_rules.size() == 0) return;

  uint32_t length = 0;  // not include 3 first octets: 1 for IE , 2 for length,
  for (auto qos : qos_rules) {
    length += qos.GetLength() +
              3;  // 1 for QoS rule identifier and 2 for length of QoS rule
  }

  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
void QosRules::Get(std::vector<QosRule>& qos_rules) const {
  qos_rules.assign(qos_rules_.begin(), qos_rules_.end());
  return;
}

//------------------------------------------------------------------------------
void QosRules::AddQosRule(const QosRule& rule) {
  qos_rules_.push_back(rule);
}

//------------------------------------------------------------------------------
int QosRules::Encode(uint8_t* buf, int len, uint8_t type) const {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoding %s", GetIeName().c_str());

  int encoded_size = 0;
  // Validate the buffer's length and Encode IEI/Length
  int len_pos = 0;
  int encoded_header_size =
      Type6NasIe::Encode(buf + encoded_size, len, len_pos);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;
  for (int i = 0; i < qos_rules_.size(); i++) {
    uint16_t encoded_qos_rule_size =
        qos_rules[i].Encode(buf + encoded_size, len - encoded_size);
  }

  // Encode length
  int encoded_len_ie = 0;
  ENCODE_U16(buf + len_pos, encoded_size - GetHeaderLength(), encoded_len_ie);
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int QosRules::Decode(
    const uint8_t* const buf, int len, bool is_iei, uint8_t type) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoding %s", GetIeName().c_str());
  int decoded_size = 0;

  // IEI and Length
  uint16_t ie_len         = 0;
  int decoded_header_size = Type6NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;
  ie_len = GetLengthIndicator();

  if (type != kMultiplePayloads) {  // not multiple payloads
    uint8_t octet       = 0;
    bstring content_tmp = {};
    content_tmp         = blk2bstr(buf + decoded_size, ie_len);
    content_            = std::optional<bstring>(content_tmp);
    decoded_size += ie_len;
    return decoded_size;
  }

  // Multiple payloads
  uint8_t num_entries  = {};
  uint8_t num_optional = {};
  std::vector<QosRulesEntry> CONTENT_tmp;

  QosRulesEntry QosRulesentry = {};
  DECODE_U8(buf + decoded_size, num_entries, decoded_size);

  while (num_entries) {
    uint8_t length_entry = 0;
    DECODE_U8(buf + decoded_size, length_entry, decoded_size);
    uint8_t octet = 0;
    // Number of optional IEs and Payload container type
    DECODE_U8(buf + decoded_size, octet, decoded_size);
    QosRulesentry.QosRulesType = octet & 0x0f;
    num_optional               = (octet & 0xf0) >> 4;

    IE_t value;
    while (num_optional) {
      DECODE_U8(buf + decoded_size, value.ie_type, decoded_size);
      DECODE_U8(buf + decoded_size, value.ie_len, decoded_size);

      decode_bstring(
          &value.ie_value, value.ie_len, (buf + decoded_size),
          len - decoded_size);
      decoded_size += value.ie_len;
      QosRulesentry.optionalIE.insert(QosRulesentry.optionalIE.end(), value);
      num_optional--;
    }
    CONTENT_tmp.insert(CONTENT_tmp.end(), QosRulesentry);
    num_entries--;
  }
  contents_ = std::optional<std::vector<QosRulesEntry>>(contents_);
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoded %s (len %d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
