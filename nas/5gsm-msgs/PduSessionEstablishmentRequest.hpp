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

#ifndef _PDU_SESSION_ESTABLISHMENT_REQUEST_H_
#define _PDU_SESSION_ESTABLISHMENT_REQUEST_H_

#include <bstrlib.h>

#include <vector>

#include "NasIeHeader.hpp"
#include "Nas5gsmHeader.hpp"

namespace oai::nas {

class PduSessionEstablishmentRequest : public Nas5gsmHeader {
 public:
  PduSessionEstablishmentRequest();
  ~PduSessionEstablishmentRequest();

  int Encode(uint8_t* buf, int len) override;
  int Decode(uint8_t* buf, int len) override;

  uint32_t GetLength() const override;

 private:
  // Mandatory
  Nas5gsmHeader ie_header_;
  IntegrityProtectionMaximumDataRate ie_integrity_protection_maximum_data_rate_;

  // Optional
  // PDU session type
  std::optional<PduSessionType> ie_pdu_session_type_;
  // SSC mode
  std::optional<SscMode> ie_ssc_mode_;
  // 5GSM capability
  std::optional<_5gsmCapability> ie_5gsm_capability_;
  // Maximum number of supported packet filters
  std::optional<MaximumNumberOfSupportedPacketFilters>
      ie_maximum_number_of_supported_packet_filters_;
  // Always-on PDU session requested
  std::optional<AlwaysOnPduSessionRequested>
      ie_always_on_pdu_session_requested_;
  // SM PDU DN request container
  std::optional<PduDnRequestContainer> ie_pdu_dn_request_container_;
  // Extended protocol configuration options
  std::optional<ExtendedProtocolConfigurationOptions>
      ie_extended_protocol_configuration_options_;
  // IP header compression configuration
  std::optional<IpHeaderCompressionConfiguration>
      ie_ip_header_compression_configuration_;
  // DS-TT Ethernet port MAC address
  // UE-DS-TT residence time
  // Port management information container
  // Ethernet header compression configuration
  // Suggested interface identifier
};

}  // namespace oai::nas

#endif
