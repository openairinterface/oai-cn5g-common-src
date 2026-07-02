/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef CONFIGURATION_UPDATE_COMMAND_H_
#define CONFIGURATION_UPDATE_COMMAND_H_

#include "NasIeHeader.hpp"
#include "NasMmPlainHeader.hpp"

namespace oai::nas {
using namespace oai::nas;

class ConfigurationUpdateCommand : public Nas5gmmMessage {
 public:
  ConfigurationUpdateCommand();
  ~ConfigurationUpdateCommand();

  int Encode(uint8_t* buf, int len) override;
  int Decode(uint8_t* buf, int len) override;

  uint32_t GetLength() const override;

  void SetHeader(uint8_t security_header_type);
  void GetSecurityHeaderType(uint8_t security_header_type);
  bool VerifyHeader();

  void SetConfigurationUpdateIndication(
      const ConfigurationUpdateIndication& configuration_update_indication);
  void GetConfigurationUpdateIndication(
      std::optional<ConfigurationUpdateIndication>&
          configuration_update_indication);

  void Set5gGuti(
      const std::string& mcc, const std::string& mnc, uint8_t amf_region_id,
      uint16_t amf_set_id, uint8_t amf_pointer, uint32_t tmsi);
  // TODO: Get

  void SetFullNameForNetwork(const NetworkName& name);
  void SetFullNameForNetwork(const std::string& text_string);
  void GetFullNameForNetwork(std::optional<NetworkName>& name) const;

  void SetShortNameForNetwork(const NetworkName& name);
  void SetShortNameForNetwork(const std::string& text_string);
  void GetShortNameForNetwork(NetworkName& name) const;

  // Release 17.10: NSSRG information (IEI 0x70, TLV-E)
  void SetNssrgInformation(const NssrgInformation& nssrg);
  std::optional<NssrgInformation> GetNssrgInformation() const;

  // Release 17.10: NSAG information (IEI 0x73, TLV-E)
  void SetNsagInformation(const NsagInformation& nsag);
  std::optional<NsagInformation> GetNsagInformation() const;

  // Release 17.10: Priority indicator (IEI 0xE-, Type 1 TV)
  void SetPriorityIndicator(uint8_t mpsi);
  std::optional<PriorityIndicator> GetPriorityIndicator() const;

  // Core A&MM IEs (mirror RegistrationAccept setter signatures)
  void SetTaiList(const std::vector<p_tai_t>& tai_list);
  void SetAllowedNssai(const std::vector<struct SNSSAI_s>& nssai);
  void SetConfiguredNssai(const std::vector<struct SNSSAI_s>& nssai);
  // Rejected NSSAI uses the CUC-specific IEI (0x11), NOT the RA IEI
  void SetRejectedNssai(const std::vector<RejectedSNssai>& nssai);
  void SetServiceAreaList(const std::vector<service_area_list_ie_t>& list);
  void SetLadnInformation(const LadnInformation& ladn_information);
  void SetMicoIndication(bool sprti, bool raai);
  void SetNetworkSlicingIndication(bool dcni, bool nssci);
  // 5GS Registration Result: optional TLV with IEI 0x44 (NOT the RA bare field)
  void Set5gsRegistrationResult(
      bool emergency, bool nssaa, bool sms, uint8_t value);

 private:
  NasMmPlainHeader ie_header_;  // Mandatory
  // Configuration update indication
  std::optional<ConfigurationUpdateIndication>
      ie_configuration_update_indication_;        // Optional
  std::optional<_5gsMobileIdentity> ie_5g_guti_;  // Optional
  std::optional<_5gsTrackingAreaIdList>
      ie_tai_list_;                        // TAI list, IEI 0x54, TLV
  std::optional<Nssai> ie_allowed_nssai_;  // Allowed NSSAI, IEI 0x15, TLV
  std::optional<ServiceAreaList> ie_service_area_list_;  // IEI 0x27, TLV
  // Full name for network (Optional)
  std::optional<NetworkName> ie_full_name_for_network_;  // Optional
  // Short name for network
  std::optional<NetworkName> ie_short_name_for_network_;  // Optional
  // TODO: Local time zone (Optional)
  // TODO: Universal time and local time zone (Optional)
  // TODO: Network daylight saving time (Optional)
  std::optional<LadnInformation> ie_ladn_information_;  // IEI 0x79, TLV-E
  std::optional<MicoIndication> ie_mico_indication_;    // IEI 0xB-, Type 1 TV
  std::optional<NetworkSlicingIndication>
      ie_network_slicing_indication_;         // IEI 0x9-, Type 1 TV
  std::optional<Nssai> ie_configured_nssai_;  // Configured NSSAI, IEI 0x31, TLV
  std::optional<RejectedNssai> ie_rejected_nssai_;  // IEI 0x11 (CUC), TLV
  // TODO: Operator-defined access category definitions (Optional)
  // TODO: SMS indication (Optional)
  // TODO: T3447 value (Optional)
  // TODO: CAG information list (Rel 16.4.1) (Optional)
  // TODO: UE radio capability ID (Rel 16.4.1) (Optional)
  // TODO: UE radio capability ID deletion indication (Rel 16.4.1) (Optional)
  std::optional<_5gsRegistrationResult>
      ie_5gs_registration_result_;  // IEI 0x44, optional TLV
  // TODO: Truncated 5G-S-TMSI configuration (Rel 16.4.1) (Optional)
  // TODO: Additional configuration indication (Rel 16.14.0) (Optional)
  // Release 17.10 IEs
  std::optional<NssrgInformation> ie_nssrg_information_;  // IEI 0x70, TLV-E
  std::optional<NsagInformation> ie_nsag_information_;    // IEI 0x73, TLV-E
  std::optional<PriorityIndicator>
      ie_priority_indicator_;  // IEI 0xE-, Type 1 TV
};

}  // namespace oai::nas

#endif
