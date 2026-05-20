/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _BROADCAST_SESSION_MODIFICATION_REQUEST_H_
#define _BROADCAST_SESSION_MODIFICATION_REQUEST_H_

#include <optional>

#include "MbsServiceArea.hpp"
#include "MbsSessionId.hpp"
#include "MbsSessionSetupOrModificationRequestTransfer.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_BroadcastSessionModificationRequest.h"
}

namespace oai::ngap {

class BroadcastSessionModificationRequestMsg : public NgapMessage {
 public:
  BroadcastSessionModificationRequestMsg();
  ~BroadcastSessionModificationRequestMsg() = default;

  void initialize();

  void setMbsSessionId(const MbsSessionId& v);
  bool getMbsSessionId(MbsSessionId& v) const;

  void setMbsServiceArea(const MbsServiceArea& v);
  bool getMbsServiceArea(MbsServiceArea& v) const;

  void setMbsSessionModificationRequestTransfer(
      const MbsSessionSetupOrModificationRequestTransfer& v);

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_BroadcastSessionModificationRequest_t*
      m_BroadcastSessionModificationRequestIes;

  MbsSessionId m_MbsSessionId;                     // Mandatory
  std::optional<MbsServiceArea> m_MbsServiceArea;  // Optional
};

}  // namespace oai::ngap
#endif
