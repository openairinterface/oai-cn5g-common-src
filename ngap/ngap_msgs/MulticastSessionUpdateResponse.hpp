/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MULTICAST_SESSION_UPDATE_RESPONSE_H_
#define _MULTICAST_SESSION_UPDATE_RESPONSE_H_

#include <optional>

#include "CriticalityDiagnostics.hpp"
#include "MbsAreaSessionId.hpp"
#include "MbsSessionId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_MulticastSessionUpdateResponse.h"
}

namespace oai::ngap {

class MulticastSessionUpdateResponseMsg : public NgapMessage {
 public:
  MulticastSessionUpdateResponseMsg();
  ~MulticastSessionUpdateResponseMsg() = default;

  void initialize();

  bool getMbsSessionId(MbsSessionId& v) const;
  bool getMbsAreaSessionId(MbsAreaSessionId& v) const;
  bool getCriticalityDiagnostics(CriticalityDiagnostics& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_MulticastSessionUpdateResponse_t* m_MulticastSessionUpdateResponseIes;

  MbsSessionId m_MbsSessionId;                              // Mandatory
  std::optional<MbsAreaSessionId> m_MbsAreaSessionId;       // Optional
  std::optional<CriticalityDiagnostics> m_CriticalityDiag;  // Optional
};

}  // namespace oai::ngap
#endif
