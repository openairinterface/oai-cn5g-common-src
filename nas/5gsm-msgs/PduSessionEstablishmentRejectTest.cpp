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

#include "PduSessionEstablishmentRejectTest.hpp"

#include "NasHelper.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
PduSessionEstablishmentRejectTest::PduSessionEstablishmentRejectTest()
    : NasMessage() {
  // ie_5gsm_cause_ ={};

  ie_back_off_timer_value_                       = std::nullopt;
  ie_pdu_session_type_                           = std::nullopt;
  ie_ssc_mode_                                   = std::nullopt;
  ie_5gsm_capability_                            = std::nullopt;
  ie_maximum_number_of_supported_packet_filters_ = std::nullopt;
  ie_always_on_pdu_session_requested_            = std::nullopt;
  ie_pdu_dn_request_container_                   = std::nullopt;
  ie_extended_protocol_configuration_options_    = std::nullopt;
  ie_ip_header_compression_configuration_        = std::nullopt;

  ie_5gsm_cause_                             = {};
  ie_integrity_protection_maximum_data_rate_ = {};
  // ie_header_ = {};
}

//------------------------------------------------------------------------------
// PduSessionEstablishmentRejectTest::~PduSessionEstablishmentRejectTest() {}

//------------------------------------------------------------------------------
uint32_t PduSessionEstablishmentRejectTest::GetLength() const {
  uint32_t msg_len = 0;
  // msg_len += ie_header_.GetLength();
  // msg_len += ie_5gsm_cause_.GetIeLength();

  //  if (ie_back_off_timer_value_.has_value())
  //    msg_len += ie_back_off_timer_value_.value().GetIeLength();

  return msg_len;
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRejectTest::Set5gsmCause(
    const _5gsmCause& _5gsm_cause) {
  // ie_5gsm_cause_ = _5gsm_cause;
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRejectTest::Get5gsmCause(
    _5gsmCause& _5gsm_cause) const {
  //_5gsm_cause = ie_5gsm_cause_;
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRejectTest::SetBackOffTimerValue(
    const GprsTimer3& back_off_timer_value) {
  // ie_back_off_timer_value_ =
  //    std::make_optional<GprsTimer3>(back_off_timer_value);
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRejectTest::GetBackOffTimerValue(
    std::optional<GprsTimer3>& back_off_timer_value) const {
  // back_off_timer_value = ie_back_off_timer_value_;
}

//------------------------------------------------------------------------------
int PduSessionEstablishmentRejectTest::Encode(uint8_t* buf, int len) {
  oai::logger::logger_common::nas().debug(
      "Encoding PduSessionEstablishmentRejectTest message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;
  // Header
  /* if ((encoded_ie_size = ie_header_.Encode(buf, len)) == KEncodeDecodeError)
   { oai::logger::logger_common::nas().error("Encoding NAS Header error");
     return KEncodeDecodeError;
   }
   */
  encoded_size += encoded_ie_size;
  /*
  // 5GSM cause
  if ((encoded_ie_size = NasHelper::Encode(
           ie_5gsm_cause_, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // Back-off timer value
  if ((encoded_ie_size = NasHelper::Encode(
           ie_back_off_timer_value_, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }
*/

  oai::logger::logger_common::nas().debug(
      "Encoded PduSessionEstablishmentRejectTest message len (%d)",
      encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int PduSessionEstablishmentRejectTest::Decode(uint8_t* buf, int len) {
  oai::logger::logger_common::nas().debug(
      "Decoding PduSessionEstablishmentRejectTest message");
  int decoded_size    = 0;
  int decoded_ie_size = 0;

  // Header
  // decoded_ie_size = ie_header_.Decode(buf, len);
  if (decoded_ie_size == KEncodeDecodeError) {
    oai::logger::logger_common::nas().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_ie_size;
  /*
    //
    if ((decoded_ie_size =
             NasHelper::Decode(ie_5gsm_cause_, buf, len, decoded_size, false))
    == KEncodeDecodeError) { return KEncodeDecodeError;
    }
    */
  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf, octet, decoded_size, len);
  oai::logger::logger_common::nas().debug("First option IEI (0x%x)", octet);
  bool flag = false;
  while ((octet != 0x0)) {
    switch ((octet & 0xf0) >> 4) {
      default: {
        flag = true;
      }
    }

    switch (octet) {
        /*    case kIeiGprsTimer3BackOffTimer: {
              oai::logger::logger_common::nas().debug(
                  "Decoding IEI 0x%x", kIeiGprsTimer3BackOffTimer);
              if ((decoded_ie_size = NasHelper::Decode(
                       ie_back_off_timer_value_, kIeiGprsTimer3BackOffTimer,
           buf, len, decoded_size, true)) == KEncodeDecodeError) { return
           KEncodeDecodeError;
              }
              DECODE_U8_VALUE(buf, octet, decoded_size, len);
              oai::logger::logger_common::nas().debug("Next IEI (0x%x)", octet);
            } break;
      */

      default: {
        // TODO:
        if (flag) {
          oai::logger::logger_common::nas().warn(
              "Unknown IEI 0x%x, stop decoding...", octet);
          // Stop decoding
          octet = 0x00;
        }
      } break;
    }
  }

  oai::logger::logger_common::nas().debug(
      "Decoded PduSessionEstablishmentRejectTest message len (%d)",
      decoded_size);
  return decoded_size;
}
