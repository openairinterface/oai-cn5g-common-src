/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _UE_CONTEXT_MODIFICATION_FAILURE_MSG_H_
#define _UE_CONTEXT_MODIFICATION_FAILURE_MSG_H_

#include <optional>

#include "Cause.hpp"
#include "NgapUeMessage.hpp"

extern "C" {
#include "Ngap_UEContextModificationFailure.h"
}

namespace oai::ngap {

class UeContextModificationFailureMsg : public NgapUeMessage {
 public:
  UeContextModificationFailureMsg();
  virtual ~UeContextModificationFailureMsg();

  void initialize();

  void setAmfUeNgapId(const uint64_t& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  bool getCause(Cause& cause) const;

 private:
  Ngap_UEContextModificationFailure_t* m_UeContextModificationFailureIes;
  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  Cause m_Cause;  // Mandatory
  // CriticalityDiagnostics (Optional) — TODO
};

}  // namespace oai::ngap

#endif
