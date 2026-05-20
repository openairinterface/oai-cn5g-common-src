/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _UE_CONTEXT_MODIFICATION_RESPONSE_MSG_H_
#define _UE_CONTEXT_MODIFICATION_RESPONSE_MSG_H_

#include <optional>

#include "NgapUeMessage.hpp"

extern "C" {
#include "Ngap_UEContextModificationResponse.h"
}

namespace oai::ngap {

class UeContextModificationResponseMsg : public NgapUeMessage {
 public:
  UeContextModificationResponseMsg();
  virtual ~UeContextModificationResponseMsg();

  void initialize();

  void setAmfUeNgapId(const uint64_t& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_UEContextModificationResponse_t* m_UeContextModificationResponseIes;
  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  // RRCState (Optional) — TODO
  // UserLocationInformation (Optional) — TODO
  // CriticalityDiagnostics (Optional) — TODO
};

}  // namespace oai::ngap

#endif
