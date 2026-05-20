/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _BROADCAST_SESSION_RELEASE_RESPONSE_H_
#define _BROADCAST_SESSION_RELEASE_RESPONSE_H_

#include <optional>

#include "CriticalityDiagnostics.hpp"
#include "MbsSessionId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_BroadcastSessionReleaseResponse.h"
}

namespace oai::ngap {

class BroadcastSessionReleaseResponseMsg : public NgapMessage {
 public:
  BroadcastSessionReleaseResponseMsg();
  ~BroadcastSessionReleaseResponseMsg() = default;

  void initialize();

  bool getMbsSessionId(MbsSessionId& v) const;
  bool getCriticalityDiagnostics(CriticalityDiagnostics& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_BroadcastSessionReleaseResponse_t* m_BroadcastSessionReleaseResponseIes;

  MbsSessionId m_MbsSessionId;                              // Mandatory
  std::optional<CriticalityDiagnostics> m_CriticalityDiag;  // Optional
};

}  // namespace oai::ngap
#endif
