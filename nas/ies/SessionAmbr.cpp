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

#include "SessionAmbr.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.hpp"
#include "logger_base.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
SessionAmbr::SessionAmbr() : Type4NasIe() {
  SetLengthIndicator(kSessionAmbrContentLength);
}

//------------------------------------------------------------------------------
SessionAmbr::SessionAmbr(uint8_t iei) : Type4NasIe(kIeiSessionAmbr) {
  SetLengthIndicator(kSessionAmbrContentLength);
}

//------------------------------------------------------------------------------
SessionAmbr::SessionAmbr(
    uint8_t iei, uint8_t unit_for_downlink, uint16_t session_ambr_for_downlink,
    uint8_t unit_for_uplink, uint16_t session_ambr_for_uplink)
    : Type4NasIe(kIeiSessionAmbr) {
  unit_for_downlink_         = unit_for_downlink;
  session_ambr_for_downlink_ = session_ambr_for_downlink;
  unit_for_uplink_           = unit_for_uplink;
  session_ambr_for_uplink_   = session_ambr_for_uplink;
  SetLengthIndicator(kSessionAmbrContentLength);
}

//------------------------------------------------------------------------------
SessionAmbr::SessionAmbr(
    uint8_t unit_for_downlink, uint16_t session_ambr_for_downlink,
    uint8_t unit_for_uplink, uint16_t session_ambr_for_uplink)
    : Type4NasIe() {
  unit_for_downlink_         = unit_for_downlink;
  session_ambr_for_downlink_ = session_ambr_for_downlink;
  unit_for_uplink_           = unit_for_uplink;
  session_ambr_for_uplink_   = session_ambr_for_uplink;
  SetLengthIndicator(kSessionAmbrContentLength);
}

//------------------------------------------------------------------------------
SessionAmbr::~SessionAmbr() {}

//------------------------------------------------------------------------------
void SessionAmbr::SetUnitForDownlink(uint8_t unit_for_downlink) {
  unit_for_downlink_ = unit_for_downlink;
}

//------------------------------------------------------------------------------
uint8_t SessionAmbr::GetUnitForDownlink() const {
  return unit_for_downlink_;
}

//------------------------------------------------------------------------------
void SessionAmbr::SetSessionAmbrForDownlink(
    uint16_t session_ambr_for_downlink) {
  session_ambr_for_downlink_ = session_ambr_for_downlink;
}

//------------------------------------------------------------------------------
uint16_t SessionAmbr::GetSessionAmbrForDownlink() const {
  return session_ambr_for_downlink_;
}

//------------------------------------------------------------------------------
void SessionAmbr::SetUnitForUplink(uint8_t unit_for_uplink) {
  unit_for_uplink_ = unit_for_uplink;
}

//------------------------------------------------------------------------------
uint8_t SessionAmbr::GetUnitForUplink() const {
  return unit_for_uplink_;
}

//------------------------------------------------------------------------------
void SessionAmbr::SetSessionAmbrForUplink(uint16_t session_ambr_for_uplink) {
  session_ambr_for_uplink_ = session_ambr_for_uplink;
}

//------------------------------------------------------------------------------
uint16_t SessionAmbr::GetSessionAmbrForUplink() const {
  return session_ambr_for_uplink_;
}

//------------------------------------------------------------------------------
int SessionAmbr::Encode(uint8_t* buf, int len) const {
  oai::logger::logger_common::nas().debug("Encoding %s", GetIeName().c_str());
  int ie_len = GetIeLength();

  int encoded_size = 0;
  // Validate the buffer's length and Encode IEI/Length
  int encoded_header_size = Type4NasIe::Encode(buf + encoded_size, len);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  // Unit for Session-AMBR for downlink
  ENCODE_U8(buf + encoded_size, unit_for_downlink_, encoded_size);
  // Session-AMBR for downlink
  ENCODE_U16(buf + encoded_size, session_ambr_for_downlink_, encoded_size);

  // Unit for Session-AMBR for uplink
  ENCODE_U8(buf + encoded_size, unit_for_uplink_, encoded_size);
  // Session-AMBR for uplink
  ENCODE_U16(buf + encoded_size, session_ambr_for_uplink_, encoded_size);

  oai::logger::logger_common::nas().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int SessionAmbr::Decode(const uint8_t* const buf, int len, bool is_iei) {
  if (len < kSessionAmbrLength) {
    oai::logger::logger_common::nas().error(
        "Buffer length is less than the minimum length of this IE (%d "
        "octet)",
        kSessionAmbrLength);
    return KEncodeDecodeError;
  }

  uint8_t decoded_size = 0;
  oai::logger::logger_common::nas().debug("Decoding %s", GetIeName().c_str());

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  // Unit for Session-AMBR for downlink
  DECODE_U8(buf + decoded_size, unit_for_downlink_, decoded_size);
  // Session-AMBR for downlink
  DECODE_U16(buf + decoded_size, session_ambr_for_downlink_, decoded_size);

  // Unit for Session-AMBR for uplink
  DECODE_U8(buf + decoded_size, unit_for_uplink_, decoded_size);
  // Session-AMBR for uplink
  DECODE_U16(buf + decoded_size, session_ambr_for_uplink_, decoded_size);

  oai::logger::logger_common::nas().debug(
      "Decoded %s, Session-AMBR for Downlink: 0x%x, Unit: 0x%x",
      GetIeName().c_str(), session_ambr_for_downlink_, unit_for_downlink_);
  oai::logger::logger_common::nas().debug(
      "Decoded %s, Session-AMBR for Uplink: 0x%x, Unit: 0x%x",
      GetIeName().c_str(), session_ambr_for_uplink_, unit_for_uplink_);

  oai::logger::logger_common::nas().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
