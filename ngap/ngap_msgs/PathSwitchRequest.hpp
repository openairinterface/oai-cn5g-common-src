/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _PATH_SWITCH_REQUEST_MSG_H_
#define _PATH_SWITCH_REQUEST_MSG_H_

#include <optional>

#include "AmfUeNgapId.hpp"
#include "Cause.hpp"
#include "NgapUeMessage.hpp"
#include "RanUeNgapId.hpp"
#include "RedCapIndication.hpp"
#include "UserLocationInformation.hpp"

extern "C" {
#include "Ngap_PathSwitchRequest.h"
}

namespace oai::ngap {

class PathSwitchRequestMsg : public NgapUeMessage {
 public:
  PathSwitchRequestMsg();
  virtual ~PathSwitchRequestMsg();

  void initialize();

  void setAmfUeNgapId(const uint64_t& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  bool getUserLocationInformation(
      UserLocationInformation& userLocationInformation) const;

  bool getRedCapIndication(RedCapIndication& value) const;

 private:
  Ngap_PathSwitchRequest_t* m_PathSwitchRequestIes;
  // RAN_UE_NGAP_ID (Mandatory)
  // AMF_UE_NGAP_ID (Mandatory)
  UserLocationInformation m_UserLocationInformation;      // Mandatory
  std::optional<RedCapIndication> m_RedCapIndication;     // Rel-17
  // UESecurityCapabilities (Mandatory) — raw, not decoded at this stage
  // PDUSessionResourceToBeSwitchedDLList (Mandatory) — raw, not decoded
  // PDUSessionResourceFailedToSetupListPSReq (Optional)
  // RRCEstablishmentCause (Optional)
};

}  // namespace oai::ngap

#endif
