/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MULTICAST_SESSION_UPDATE_REQUEST_H_
#define _MULTICAST_SESSION_UPDATE_REQUEST_H_

#include <optional>

#include "MbsAreaSessionId.hpp"
#include "MbsSessionId.hpp"
#include "MulticastSessionUpdateRequestTransfer.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_MulticastSessionUpdateRequest.h"
}

namespace oai::ngap {

class MulticastSessionUpdateRequestMsg : public NgapMessage {
 public:
  MulticastSessionUpdateRequestMsg();
  ~MulticastSessionUpdateRequestMsg() = default;

  void initialize();

  void setMbsSessionId(const MbsSessionId& v);
  bool getMbsSessionId(MbsSessionId& v) const;

  void setMbsAreaSessionId(const MbsAreaSessionId& v);
  bool getMbsAreaSessionId(MbsAreaSessionId& v) const;

  void setMulticastSessionUpdateRequestTransfer(
      const MulticastSessionUpdateRequestTransfer& v);

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_MulticastSessionUpdateRequest_t* m_MulticastSessionUpdateRequestIes;

  MbsSessionId m_MbsSessionId;                         // Mandatory
  std::optional<MbsAreaSessionId> m_MbsAreaSessionId;  // Optional
};

}  // namespace oai::ngap
#endif
