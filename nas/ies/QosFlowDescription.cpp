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

#include "QosFlowDescription.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger_base.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
QosFlowDescription::QosFlowDescription() : e_bit_(false), length_(0) {}

//------------------------------------------------------------------------------
QosFlowDescription::~QosFlowDescription() {}

//------------------------------------------------------------------------------
uint16_t QosFlowDescription::GetLength() const {
  return length_;
}

//------------------------------------------------------------------------------
void QosFlowDescription::SetLength() {
  // Calculate the actual length
  length_ = kQosFlowDescriptionMinimumLength;
  if (parameters_list_.has_value()) {
    for (auto p : parameters_list_.value()) {
      length_ +=
          p.length +
          2;  // 2: 1 for parameter id, 1 for length of parameter contents
    }
  }
}

//------------------------------------------------------------------------------
void QosFlowDescription::SetQfi(uint8_t qfi) {
  qfi_ = qfi & 0x3f;  // 6 bits
  SetLength();
}

//------------------------------------------------------------------------------
void QosFlowDescription::GetQfi(uint8_t& qfi) const {
  qfi = qfi_;
}

//------------------------------------------------------------------------------
uint8_t QosFlowDescription::GetQfi() const {
  return qfi_;
}

//------------------------------------------------------------------------------
void QosFlowDescription::SetOperationCode(uint8_t code) {
  operation_code_ = code & 0x07;  // 3 bits
}

//------------------------------------------------------------------------------
void QosFlowDescription::GetOperationCode(uint8_t& code) const {
  code = operation_code_;
}

//------------------------------------------------------------------------------
uint8_t QosFlowDescription::GetOperationCode() const {
  return operation_code_;
}

//------------------------------------------------------------------------------
void QosFlowDescription::SetEBit(bool e_bit) {
  e_bit_ = e_bit;
}
//------------------------------------------------------------------------------
void QosFlowDescription::GetEBit(bool& e_bit) const {
  e_bit = e_bit_;
}

//------------------------------------------------------------------------------
bool QosFlowDescription::GetEBit() const {
  return e_bit_;
}

//------------------------------------------------------------------------------
void QosFlowDescription::GetNumberOfParameters(uint8_t& no_parameters) const {
  if (parameters_list_.has_value()) {
    no_parameters = parameters_list_.value().size();
  }
}
//------------------------------------------------------------------------------
uint8_t QosFlowDescription::GetNumberOfParameters() const {
  if (parameters_list_.has_value())
    return parameters_list_.value().size();
  else
    return 0;
}

//------------------------------------------------------------------------------
void QosFlowDescription::SetParametersList(
    const std::vector<QosFlowDescriptionsParameter>& list) {
  parameters_list_ =
      std::make_optional<std::vector<QosFlowDescriptionsParameter>>(list);
  SetLength();
}

//------------------------------------------------------------------------------
void QosFlowDescription::GetParametersList(
    std::optional<std::vector<QosFlowDescriptionsParameter>>& list) const {
  list = parameters_list_;
}
//------------------------------------------------------------------------------
std::optional<std::vector<QosFlowDescriptionsParameter>>
QosFlowDescription::GetParametersList() const {
  return parameters_list_;
}

//------------------------------------------------------------------------------
int QosFlowDescription::Encode(uint8_t* buf, int len) const {
  oai::logger::logger_common::nas().debug("Encoding QosFlowDescription");

  int encoded_size = 0;

  // Validate the buffer's length and Encode IEI/Length (later)
  if (len < length_) {
    oai::logger::logger_common::nas().error(
        "Buffer length is less than the length of this IE (%d "
        "octet)",
        length_);
    return KEncodeDecodeError;
  }

  // Octet 4: spare (2 bits) + QFI (6 bits)
  uint8_t octet4 = qfi_ & 0x3f;
  ENCODE_U8(buf + encoded_size, octet4, encoded_size);

  // Octet 5: Operation code (2 bits) + spare (4 bits)
  uint8_t octet5 = (operation_code_ & 0x07) << 5;
  ENCODE_U8(buf + encoded_size, octet5, encoded_size);

  uint8_t number_of_parameters = 0;
  if (parameters_list_.has_value())
    number_of_parameters = parameters_list_.value().size();

  // Octet 6: spare + e (1 bit) + number of parameters (6 bits)
  uint8_t octet6 = (e_bit_ << 7) | (number_of_parameters & 0x3f);
  ENCODE_U8(buf + encoded_size, octet6, encoded_size);

  if (!parameters_list_.has_value()) return encoded_size;

  // Parameter lists
  for (auto p : parameters_list_.value()) {
    // Parameter identifier
    ENCODE_U8(buf + encoded_size, p.parameter_id, encoded_size);
    // Length of parameter contents
    ENCODE_U8(buf + encoded_size, p.length, encoded_size);
    // Parameter contents
    int encoded_content_size =
        encode_bstring(p.content, (buf + encoded_size), len - encoded_size);
    encoded_size += encoded_content_size;
  }

  oai::logger::logger_common::nas().debug(
      "Encoded QosFlowDescription, len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int QosFlowDescription::Decode(const uint8_t* const buf, int len) {
  oai::logger::logger_common::nas().debug("Decoding QosFlowDescription");
  if (len < kQosFlowDescriptionMinimumLength) {
    oai::logger::logger_common::nas().error(
        "Buffer length is less than the minimum length of this IE (%d "
        "octet)",
        kQosFlowDescriptionMinimumLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;

  // Octet 4: spare (2 bits) + QFI (6 bits)
  uint8_t octet4 = {};
  DECODE_U8(buf + decoded_size, octet4, decoded_size);
  qfi_ = octet4 & 0x3f;

  // Octet 5: Operation code (3 bits) + spare (4 bits)
  uint8_t octet5 = {};
  DECODE_U8(buf + decoded_size, octet5, decoded_size);
  operation_code_ = (octet5 & 0x80) >> 5;

  // Octet 6: spare + e (1 bit) + number of parameters (6 bits)
  uint8_t octet6 = {};
  DECODE_U8(buf + decoded_size, octet6, decoded_size);
  e_bit_                       = (octet6 >> 6) & 0x01;
  uint8_t number_of_parameters = octet6 & 0x3f;  // 6 bits

  // Parameters list
  std::vector<QosFlowDescriptionsParameter> parameters_list;
  for (int i = 0; i < number_of_parameters; i++) {
    QosFlowDescriptionsParameter parameter = {};

    // Parameter identifier
    DECODE_U8(buf + decoded_size, parameter.parameter_id, decoded_size);

    // Length of parameter contents
    DECODE_U8(buf + decoded_size, parameter.length, decoded_size);
    // Parameter contents
    uint8_t decoded_bstring_size = decode_bstring(
        &parameter.content, parameter.length, (buf + decoded_size),
        len - decoded_size);
    if (decoded_bstring_size > 0) decoded_size += parameter.length;
    parameters_list.push_back(parameter);
  }

  parameters_list_ =
      std::make_optional<std::vector<QosFlowDescriptionsParameter>>(
          parameters_list);

  oai::logger::logger_common::nas().debug(
      "Decoded QosFlowDescription (len %d)", decoded_size);
  return decoded_size;
}
