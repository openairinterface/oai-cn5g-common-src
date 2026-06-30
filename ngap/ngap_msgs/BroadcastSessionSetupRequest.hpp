/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _BROADCAST_SESSION_SETUP_REQUEST_H_
#define _BROADCAST_SESSION_SETUP_REQUEST_H_

#include <optional>

#include "MbsServiceArea.hpp"
#include "MbsSessionId.hpp"
#include "MbsSessionSetupOrModificationRequestTransfer.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_BroadcastSessionSetupRequest.h"
}

namespace oai::ngap {

class BroadcastSessionSetupRequestMsg : public NgapMessage {
 public:
  BroadcastSessionSetupRequestMsg();
  ~BroadcastSessionSetupRequestMsg() = default;

  void initialize();

  void setMbsSessionId(const MbsSessionId& v);
  bool getMbsSessionId(MbsSessionId& v) const;

  void setMbsServiceArea(const MbsServiceArea& v);
  bool getMbsServiceArea(MbsServiceArea& v) const;

  void setMbsSessionSetupOrModificationRequestTransfer(
      const MbsSessionSetupOrModificationRequestTransfer& v);
  bool getMbsSessionSetupOrModificationRequestTransfer(
      MbsSessionSetupOrModificationRequestTransfer& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_BroadcastSessionSetupRequest_t* m_BroadcastSessionSetupRequestIes;

  MbsSessionId m_MbsSessionId;                     // Mandatory
  std::optional<MbsServiceArea> m_MbsServiceArea;  // Optional
  // Transfer is carried as OCTET_STRING at message layer
};

}  // namespace oai::ngap
#endif
