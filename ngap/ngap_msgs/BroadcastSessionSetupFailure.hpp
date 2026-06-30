/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _BROADCAST_SESSION_SETUP_FAILURE_H_
#define _BROADCAST_SESSION_SETUP_FAILURE_H_

#include <optional>

#include "Cause.hpp"
#include "CriticalityDiagnostics.hpp"
#include "MbsSessionId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_BroadcastSessionSetupFailure.h"
}

namespace oai::ngap {

class BroadcastSessionSetupFailureMsg : public NgapMessage {
 public:
  BroadcastSessionSetupFailureMsg();
  ~BroadcastSessionSetupFailureMsg() = default;

  void initialize();

  bool getMbsSessionId(MbsSessionId& v) const;
  bool getCause(Cause& v) const;
  bool getCriticalityDiagnostics(CriticalityDiagnostics& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_BroadcastSessionSetupFailure_t* m_BroadcastSessionSetupFailureIes;

  MbsSessionId m_MbsSessionId;                              // Mandatory
  Cause m_Cause;                                             // Mandatory
  std::optional<CriticalityDiagnostics> m_CriticalityDiag;  // Optional
};

}  // namespace oai::ngap
#endif
