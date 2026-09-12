/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _PATH_SWITCH_REQUEST_ACK_MSG_H_
#define _PATH_SWITCH_REQUEST_ACK_MSG_H_

#include <optional>

#include "FiveGProSeAuthorized.hpp"
#include "FiveGProSePC5QoSParameters.hpp"
#include "FiveGProSeUePC5AggregateMaximumBitRate.hpp"
#include "IabAuthorized.hpp"
#include "ManagementBasedMdtPlmnList.hpp"
#include "ManagementBasedMdtPlmnModificationList.hpp"
#include "NgapUeMessage.hpp"
#include "TimeSynchronisationAssistanceInfo.hpp"

extern "C" {
#include "Ngap_PathSwitchRequestAcknowledge.h"
}

namespace oai::ngap {

class PathSwitchRequestAckMsg : public NgapUeMessage {
 public:
  PathSwitchRequestAckMsg();
  virtual ~PathSwitchRequestAckMsg();

  void initialize();

  void setAmfUeNgapId(const uint64_t& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setManagementBasedMdtPlmnList(const ManagementBasedMdtPlmnList& value);
  void setManagementBasedMdtPlmnModificationList(
      const ManagementBasedMdtPlmnModificationList& value);
  void setTimeSynchronisationAssistanceInfo(
      const TimeSynchronisationAssistanceInfo& value);
  void setFiveGProSeAuthorized(const FiveGProSeAuthorized& value);
  void setFiveGProSeUePC5AggregateMaximumBitRate(
      const FiveGProSeUePC5AggregateMaximumBitRate& value);
  void setFiveGProSePC5QoSParameters(const FiveGProSePC5QoSParameters& value);
  void setIabAuthorized(const IabAuthorized& value);

 private:
  Ngap_PathSwitchRequestAcknowledge_t* m_PathSwitchRequestAckIes;
  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  // PDUSessionResourceSwitchedList (Mandatory) — TODO
  // SecurityContext (Mandatory) — TODO
  // AllowedNSSAI (Optional) — TODO
  // PDUSessionResourceReleasedListPSAck (Optional) — TODO
  // NewSecurityContextInd (Optional) — TODO
  // CoreNetworkAssistanceInformationForInactive (Optional) — TODO
  std::optional<ManagementBasedMdtPlmnList> m_ManagementBasedMdtPlmnList;
  std::optional<ManagementBasedMdtPlmnModificationList>
      m_ManagementBasedMdtPlmnModificationList;
  std::optional<TimeSynchronisationAssistanceInfo>
      m_TimeSynchronisationAssistanceInfo;
  std::optional<FiveGProSeAuthorized> m_FiveGProSeAuthorized;
  std::optional<FiveGProSeUePC5AggregateMaximumBitRate>
      m_FiveGProSeUePC5AggregateMaximumBitRate;
  std::optional<FiveGProSePC5QoSParameters> m_FiveGProSePC5QoSParameters;
  std::optional<IabAuthorized> m_IabAuthorized;
};

}  // namespace oai::ngap

#endif
