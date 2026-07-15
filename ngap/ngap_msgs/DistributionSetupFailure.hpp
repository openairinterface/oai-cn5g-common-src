/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _DISTRIBUTION_SETUP_FAILURE_H_
#define _DISTRIBUTION_SETUP_FAILURE_H_

#include <optional>

#include "Cause.hpp"
#include "CriticalityDiagnostics.hpp"
#include "MbsAreaSessionId.hpp"
#include "MbsDistributionSetupUnsuccessfulTransfer.hpp"
#include "MbsSessionId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_DistributionSetupFailure.h"
}

namespace oai::ngap {

class DistributionSetupFailureMsg : public NgapMessage {
 public:
  DistributionSetupFailureMsg();
  ~DistributionSetupFailureMsg() = default;

  void initialize();

  void setMbsSessionId(const MbsSessionId& v);
  bool getMbsSessionId(MbsSessionId& v) const;

  void setMbsAreaSessionId(const MbsAreaSessionId& v);
  bool getMbsAreaSessionId(MbsAreaSessionId& v) const;

  void setMbsDistributionSetupUnsuccessfulTransfer(
      const MbsDistributionSetupUnsuccessfulTransfer& v);

  void setCause(const Cause& v);
  bool getCause(Cause& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_DistributionSetupFailure_t* m_DistributionSetupFailureIes;

  MbsSessionId m_MbsSessionId;                              // Mandatory
  std::optional<MbsAreaSessionId> m_MbsAreaSessionId;       // Optional
  Cause m_Cause;                                            // Mandatory
  std::optional<CriticalityDiagnostics> m_CriticalityDiag;  // Optional
};

}  // namespace oai::ngap
#endif
