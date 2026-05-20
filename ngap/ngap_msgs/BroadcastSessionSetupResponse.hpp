/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _BROADCAST_SESSION_SETUP_RESPONSE_H_
#define _BROADCAST_SESSION_SETUP_RESPONSE_H_

#include <optional>

#include "CriticalityDiagnostics.hpp"
#include "MbsSessionId.hpp"
#include "MbsSessionSetupResponseTransfer.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_BroadcastSessionSetupResponse.h"
}

namespace oai::ngap {

class BroadcastSessionSetupResponseMsg : public NgapMessage {
 public:
  BroadcastSessionSetupResponseMsg();
  ~BroadcastSessionSetupResponseMsg() = default;

  void initialize();

  bool getMbsSessionId(MbsSessionId& v) const;
  bool getCriticalityDiagnostics(CriticalityDiagnostics& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_BroadcastSessionSetupResponse_t* m_BroadcastSessionSetupResponseIes;

  MbsSessionId m_MbsSessionId;                              // Mandatory
  std::optional<CriticalityDiagnostics> m_CriticalityDiag;  // Optional
};

}  // namespace oai::ngap
#endif
