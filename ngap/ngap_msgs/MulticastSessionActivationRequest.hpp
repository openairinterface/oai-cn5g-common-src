/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MULTICAST_SESSION_ACTIVATION_REQUEST_H_
#define _MULTICAST_SESSION_ACTIVATION_REQUEST_H_

#include "MbsSessionId.hpp"
#include "MulticastSessionActivationRequestTransfer.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_MulticastSessionActivationRequest.h"
}

namespace oai::ngap {

class MulticastSessionActivationRequestMsg : public NgapMessage {
 public:
  MulticastSessionActivationRequestMsg();
  ~MulticastSessionActivationRequestMsg() = default;

  void initialize();

  void setMbsSessionId(const MbsSessionId& v);
  bool getMbsSessionId(MbsSessionId& v) const;

  void setMulticastSessionActivationRequestTransfer(
      const MulticastSessionActivationRequestTransfer& v);

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_MulticastSessionActivationRequest_t*
      m_MulticastSessionActivationRequestIes;

  MbsSessionId m_MbsSessionId;  // Mandatory
};

}  // namespace oai::ngap
#endif
