/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MULTICAST_SESSION_DEACTIVATION_RESPONSE_H_
#define _MULTICAST_SESSION_DEACTIVATION_RESPONSE_H_

#include <optional>

#include "CriticalityDiagnostics.hpp"
#include "MbsSessionId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_MulticastSessionDeactivationResponse.h"
}

namespace oai::ngap {

class MulticastSessionDeactivationResponseMsg : public NgapMessage {
 public:
  MulticastSessionDeactivationResponseMsg();
  ~MulticastSessionDeactivationResponseMsg() = default;

  void initialize();

  bool getMbsSessionId(MbsSessionId& v) const;
  bool getCriticalityDiagnostics(CriticalityDiagnostics& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_MulticastSessionDeactivationResponse_t*
      m_MulticastSessionDeactivationResponseIes;

  MbsSessionId m_MbsSessionId;                              // Mandatory
  std::optional<CriticalityDiagnostics> m_CriticalityDiag;  // Optional
};

}  // namespace oai::ngap
#endif
