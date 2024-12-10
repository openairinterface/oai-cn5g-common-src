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

#ifndef _PDU_SESSION_ESTABLISHMENT_REJECT_TEST_H_
#define _PDU_SESSION_ESTABLISHMENT_REJECT_TEST_H_

#include "Nas5gsmHeader.hpp"
#include "NasIeHeader.hpp"
#include "NasMessage.hpp"
using namespace oai::nas;

namespace oai::nas {

class PduSessionEstablishmentRejectTest : public NasMessage {
 public:
  PduSessionEstablishmentRejectTest();
  virtual ~PduSessionEstablishmentRejectTest() = default;

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  uint32_t GetLength() const;

  void Set5gsmCause(const _5gsmCause& _5gsm_cause);
  void Get5gsmCause(_5gsmCause& _5gsm_cause) const;

  void SetBackOffTimerValue(const GprsTimer3& back_off_timer_value);
  void GetBackOffTimerValue(
      std::optional<GprsTimer3>& back_off_timer_value) const;

 private:
  std::optional<GprsTimer3> ie_back_off_timer_value_;  // Optional
  std::optional<PduSessionType> ie_pdu_session_type_;  // Optional
  std::optional<SscMode> ie_ssc_mode_;                 // Optional
  std::optional<_5gsmCapability> ie_5gsm_capability_;  // Optional

  std::optional<MaximumNumberOfSupportedPacketFilters>
      ie_maximum_number_of_supported_packet_filters_;  // Optional
  std::optional<AlwaysOnPduSessionRequested>
      ie_always_on_pdu_session_requested_;  // Optional
  std::optional<PduDnRequestContainer>
      ie_pdu_dn_request_container_;  // Optional
  std::optional<ExtendedProtocolConfigurationOptions>
      ie_extended_protocol_configuration_options_;  // Optional
  std::optional<IpHeaderCompressionConfiguration>
      ie_ip_header_compression_configuration_;  // Optional

  _5gsmCause ie_5gsm_cause_;  // Mandatory
  IntegrityProtectionMaximumDataRate
      ie_integrity_protection_maximum_data_rate_;  // Mandatory
  Nas5gsmHeader ie_header_;                        // Mandatory
};

}  // namespace oai::nas

#endif
