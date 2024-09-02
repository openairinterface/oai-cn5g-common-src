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

#include "PduSessionEstablishmentRequest.hpp"

#include "NasHelper.hpp"
#include "conversions.hpp"
#include "utils.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
PduSessionEstablishmentRequest::PduSessionEstablishmentRequest()
    : ie_header_(
          k5gsSessionManagementMessages, kPduSessionEstablishmentRequest) {}

//------------------------------------------------------------------------------
PduSessionEstablishmentRequest::~PduSessionEstablishmentRequest() {}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRequest::SetPduSessionIdentity(
    uint8_t pdu_session_id) {
  ie_header_.SetPduSessionIdentity(pdu_session_id);
}

//------------------------------------------------------------------------------
uint8_t PduSessionEstablishmentRequest::GetPduSessionIdentity() const {
  return ie_header_.GetPduSessionIdentity();
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRequest::SetProcedureTransactionIdentity(
    uint16_t procedure_transaction_id) {
  ie_header_.SetProcedureTransactionIdentity(procedure_transaction_id);
}

//------------------------------------------------------------------------------
uint16_t PduSessionEstablishmentRequest::GetProcedureTransactionIdentity()
    const {
  return ie_header_.GetProcedureTransactionIdentity();
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRequest::SetIntegrityProtectionMaximumDataRate(
    const IntegrityProtectionMaximumDataRate& rate) {
  ie_integrity_protection_maximum_data_rate_ = rate;
}

//------------------------------------------------------------------------------
IntegrityProtectionMaximumDataRate
PduSessionEstablishmentRequest::GetIntegrityProtectionMaximumDataRate() const {
  return ie_integrity_protection_maximum_data_rate_;
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRequest::SetPduSessionType(
    const PduSessionType& type) {
  ie_pdu_session_type_ = std::make_optional<PduSessionType>(type);
}

//------------------------------------------------------------------------------
std::optional<PduSessionType>
PduSessionEstablishmentRequest::GetPduSessionType() const {
  return ie_pdu_session_type_;
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRequest::SetSscMode(const SscMode ssc_mode) {
  ie_ssc_mode_ = std::make_optional<SscMode>(ssc_mode);
}

//------------------------------------------------------------------------------
std::optional<SscMode> PduSessionEstablishmentRequest::GetSscMode() const {
  return ie_ssc_mode_;
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRequest::Set5gsmCapability(
    const _5gsmCapability& _5gsm_capability) {
  ie_5gsm_capability_ = std::make_optional<_5gsmCapability>(_5gsm_capability);
}

//------------------------------------------------------------------------------
std::optional<_5gsmCapability>
PduSessionEstablishmentRequest::Get5gsmCapability() const {
  return ie_5gsm_capability_;
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRequest::SetMaximumNumberOfSupportedPacketFilters(
    const MaximumNumberOfSupportedPacketFilters& filters) {
  ie_maximum_number_of_supported_packet_filters_ =
      std::make_optional<MaximumNumberOfSupportedPacketFilters>(filters);
}

//------------------------------------------------------------------------------
std::optional<MaximumNumberOfSupportedPacketFilters>
PduSessionEstablishmentRequest::GetMaximumNumberOfSupportedPacketFilters() {
  return ie_maximum_number_of_supported_packet_filters_;
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRequest::SetAlwaysOnPduSessionRequested(
    const AlwaysOnPduSessionRequested& apsr) {
  ie_always_on_pdu_session_requested_ =
      std::make_optional<AlwaysOnPduSessionRequested>(apsr);
}

//------------------------------------------------------------------------------
std::optional<AlwaysOnPduSessionRequested>
PduSessionEstablishmentRequest::GetAlwaysOnPduSessionRequested() const {
  return ie_always_on_pdu_session_requested_;
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRequest::SetPduDnRequestContainer(
    const PduDnRequestContainer& container) {
  ie_pdu_dn_request_container_ =
      std::make_optional<PduDnRequestContainer>(container);
}

//------------------------------------------------------------------------------
std::optional<PduDnRequestContainer>
PduSessionEstablishmentRequest::GetPduDnRequestContainer() const {
  return ie_pdu_dn_request_container_;
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRequest::SetExtendedProtocolConfigurationOptions(
    const ExtendedProtocolConfigurationOptions& options) {
  ie_extended_protocol_configuration_options_ =
      std::make_optional<ExtendedProtocolConfigurationOptions>(options);
}
//------------------------------------------------------------------------------
std::optional<ExtendedProtocolConfigurationOptions>
PduSessionEstablishmentRequest::GetExtendedProtocolConfigurationOptions() {
  return ie_extended_protocol_configuration_options_;
}

//------------------------------------------------------------------------------
void PduSessionEstablishmentRequest::SetIpHeaderCompressionConfiguration(
    const IpHeaderCompressionConfiguration& configuration) {
  ie_ip_header_compression_configuration_ =
      std::make_optional<IpHeaderCompressionConfiguration>(configuration);
}

//------------------------------------------------------------------------------
std::optional<IpHeaderCompressionConfiguration>
PduSessionEstablishmentRequest::GetIpHeaderCompressionConfiguration() const {
  return ie_ip_header_compression_configuration_;
}
