/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _ERROR_INDICATION_H_
#define _ERROR_INDICATION_H_

#include <optional>

#include "CriticalityDiagnostics.hpp"
#include "Cause.hpp"
#include "FiveGSTmsi.hpp"
#include "NgapUeMessage.hpp"

extern "C" {
#include "Ngap_ErrorIndication.h"
#include "Ngap_NGAP-PDU.h"
}

namespace oai::ngap {

class ErrorIndication : public NgapUeMessage {
 public:
  ErrorIndication();
  virtual ~ErrorIndication();

  void initialize();

  void setAmfUeNgapId(const uint64_t& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setCause(const Cause& cause);
  void setCause(const Ngap_Cause_PR& causePresent, const long& value);
  void getCause(std::optional<Cause>& cause) const;

  void setCriticalityDiagnostics(
      const CriticalityDiagnostics& criticalityDiagnostics);
  void getCriticalityDiagnostics(
      std::optional<CriticalityDiagnostics>& criticalityDiagnostics) const;

  void setFiveGSTmsi(const FiveGSTmsi& fiveGSTmsi);
  void getFiveGSTmsi(std::optional<FiveGSTmsi>& fiveGSTmsi) const;

 private:
  Ngap_ErrorIndication_t* m_ErrorIndicationIes;

  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  std::optional<Cause> m_Cause;
  std::optional<CriticalityDiagnostics> m_CriticalityDiagnostics;
  std::optional<FiveGSTmsi> m_FiveGSTmsi;
};

}  // namespace oai::ngap
#endif
