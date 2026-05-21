/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MULTICAST_SESSION_UPDATE_FAILURE_H_
#define _MULTICAST_SESSION_UPDATE_FAILURE_H_

#include <optional>

#include "Cause.hpp"
#include "CriticalityDiagnostics.hpp"
#include "MbsAreaSessionId.hpp"
#include "MbsSessionId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_MulticastSessionUpdateFailure.h"
}

namespace oai::ngap {

class MulticastSessionUpdateFailureMsg : public NgapMessage {
 public:
  MulticastSessionUpdateFailureMsg();
  ~MulticastSessionUpdateFailureMsg() = default;

  void initialize();

  bool getMbsSessionId(MbsSessionId& v) const;
  bool getMbsAreaSessionId(MbsAreaSessionId& v) const;
  bool getCause(Cause& v) const;
  bool getCriticalityDiagnostics(CriticalityDiagnostics& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_MulticastSessionUpdateFailure_t* m_MulticastSessionUpdateFailureIes;

  MbsSessionId m_MbsSessionId;                              // Mandatory
  std::optional<MbsAreaSessionId> m_MbsAreaSessionId;       // Optional
  Cause m_Cause;                                             // Mandatory
  std::optional<CriticalityDiagnostics> m_CriticalityDiag;  // Optional
};

}  // namespace oai::ngap
#endif
