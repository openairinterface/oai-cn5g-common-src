/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _BROADCAST_SESSION_RELEASE_REQUIRED_H_
#define _BROADCAST_SESSION_RELEASE_REQUIRED_H_

#include "Cause.hpp"
#include "MbsSessionId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_BroadcastSessionReleaseRequired.h"
}

namespace oai::ngap {

class BroadcastSessionReleaseRequiredMsg : public NgapMessage {
 public:
  BroadcastSessionReleaseRequiredMsg();
  ~BroadcastSessionReleaseRequiredMsg() = default;

  void initialize();

  bool getMbsSessionId(MbsSessionId& v) const;
  bool getCause(Cause& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_BroadcastSessionReleaseRequired_t* m_BroadcastSessionReleaseRequiredIes;

  MbsSessionId m_MbsSessionId;  // Mandatory
  Cause m_Cause;                 // Mandatory
};

}  // namespace oai::ngap
#endif
