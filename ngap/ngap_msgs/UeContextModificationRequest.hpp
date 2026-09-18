/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _UE_CONTEXT_MODIFICATION_REQUEST_MSG_H_
#define _UE_CONTEXT_MODIFICATION_REQUEST_MSG_H_

#include <optional>

#include "CoreNetworkAssistanceInformationForInactive.hpp"
#include "FiveGProSeAuthorized.hpp"
#include "FiveGProSePC5QoSParameters.hpp"
#include "FiveGProSeUePC5AggregateMaximumBitRate.hpp"
#include "IndexToRfsp.hpp"
#include "ManagementBasedMdtPlmnModificationList.hpp"
#include "NgapUeMessage.hpp"
#include "QmcDeactivation.hpp"
#include "RanPagingPriority.hpp"
#include "SecurityKey.hpp"
#include "TimeSynchronisationAssistanceInfo.hpp"
#include "UeAggregateMaxBitRate.hpp"
#include "UeSecurityCapabilities.hpp"
#include "UeSliceMaximumBitRateList.hpp"

extern "C" {
#include "Ngap_EmergencyFallbackIndicator.h"
#include "Ngap_UEContextModificationRequest.h"
}

namespace oai::ngap {

class UeContextModificationRequestMsg : public NgapUeMessage {
 public:
  UeContextModificationRequestMsg();
  virtual ~UeContextModificationRequestMsg();

  void initialize();

  void setAmfUeNgapId(const uint64_t& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool setNewAmfUeNgapId(const uint64_t& id);
  bool setRanPagingPriority(const uint32_t& priority);
  bool setSecurityKey(uint8_t* buffer, const size_t& size = 256);
  bool setIndexToRfsp(const uint32_t& index);
  bool setUeAggregateMaximumBitRate(const uint64_t& dl, const uint64_t& ul);
  bool setUeSecurityCapabilities(
      uint16_t nrEncryptionAlgs, uint16_t nrIntegrityProtectionAlgs,
      uint16_t eutraEncryptionAlgs, uint16_t eutraIntegrityProtectionAlgs);
  bool setCoreNetworkAssistanceInformation(
      const CoreNetworkAssistanceInformationForInactive& info);
  bool setEmergencyFallbackIndicator(
      const Ngap_EmergencyFallbackIndicator_t& indicator);
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setTimeSynchronisationAssistanceInfo(
      const TimeSynchronisationAssistanceInfo& value);
  void setFiveGProSeAuthorized(const FiveGProSeAuthorized& value);
  void setFiveGProSeUePC5AggregateMaximumBitRate(
      const FiveGProSeUePC5AggregateMaximumBitRate& value);
  void setFiveGProSePC5QoSParameters(const FiveGProSePC5QoSParameters& value);
  void setUeSliceMaximumBitRateList(const UeSliceMaximumBitRateList& value);
  void setQmcDeactivation(const QmcDeactivation& value);
  void setManagementBasedMdtPlmnModificationList(
      const ManagementBasedMdtPlmnModificationList& value);

 private:
  Ngap_UEContextModificationRequest_t* m_UeContextModificationRequestIes;
  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  // NewAMF-UE-NGAP-ID (Optional)
  // RANPagingPriority (Optional)
  // SecurityKey (Optional)
  // IndexToRFSP (Optional)
  // UEAggregateMaximumBitRate (Optional)
  // UESecurityCapabilities (Optional)
  // CoreNetworkAssistanceInformationForInactive (Optional)
  // EmergencyFallbackIndicator (Optional)
  std::optional<TimeSynchronisationAssistanceInfo>
      m_TimeSynchronisationAssistanceInfo;
  std::optional<FiveGProSeAuthorized> m_FiveGProSeAuthorized;
  std::optional<FiveGProSeUePC5AggregateMaximumBitRate>
      m_FiveGProSeUePC5AggregateMaximumBitRate;
  std::optional<FiveGProSePC5QoSParameters> m_FiveGProSePC5QoSParameters;
  std::optional<UeSliceMaximumBitRateList> m_UeSliceMaximumBitRateList;
  std::optional<QmcDeactivation> m_QmcDeactivation;
  std::optional<ManagementBasedMdtPlmnModificationList>
      m_ManagementBasedMdtPlmnModificationList;
};

}  // namespace oai::ngap

#endif
