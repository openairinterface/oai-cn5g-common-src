/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _BROADCAST_SESSION_RELEASE_REQUEST_H_
#define _BROADCAST_SESSION_RELEASE_REQUEST_H_

#include "Cause.hpp"
#include "MbsSessionId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_BroadcastSessionReleaseRequest.h"
}

namespace oai::ngap {

class BroadcastSessionReleaseRequestMsg : public NgapMessage {
 public:
  BroadcastSessionReleaseRequestMsg();
  ~BroadcastSessionReleaseRequestMsg() = default;

  void initialize();

  void setMbsSessionId(const MbsSessionId& v);
  bool getMbsSessionId(MbsSessionId& v) const;

  void setCause(const Cause& v);
  bool getCause(Cause& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_BroadcastSessionReleaseRequest_t* m_BroadcastSessionReleaseRequestIes;

  MbsSessionId m_MbsSessionId;  // Mandatory
  Cause m_Cause;                 // Mandatory
};

}  // namespace oai::ngap
#endif
