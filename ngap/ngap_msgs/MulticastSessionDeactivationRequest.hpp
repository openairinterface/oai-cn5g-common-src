/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MULTICAST_SESSION_DEACTIVATION_REQUEST_H_
#define _MULTICAST_SESSION_DEACTIVATION_REQUEST_H_

#include "MbsSessionId.hpp"
#include "MulticastSessionDeactivationRequestTransfer.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_MulticastSessionDeactivationRequest.h"
}

namespace oai::ngap {

class MulticastSessionDeactivationRequestMsg : public NgapMessage {
 public:
  MulticastSessionDeactivationRequestMsg();
  ~MulticastSessionDeactivationRequestMsg() = default;

  void initialize();

  void setMbsSessionId(const MbsSessionId& v);
  bool getMbsSessionId(MbsSessionId& v) const;

  void setMulticastSessionDeactivationRequestTransfer(
      const MulticastSessionDeactivationRequestTransfer& v);

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_MulticastSessionDeactivationRequest_t*
      m_MulticastSessionDeactivationRequestIes;

  MbsSessionId m_MbsSessionId;  // Mandatory
};

}  // namespace oai::ngap
#endif
