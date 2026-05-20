/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _DISTRIBUTION_SETUP_RESPONSE_H_
#define _DISTRIBUTION_SETUP_RESPONSE_H_

#include <optional>

#include "CriticalityDiagnostics.hpp"
#include "MbsAreaSessionId.hpp"
#include "MbsDistributionSetupResponseTransfer.hpp"
#include "MbsSessionId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_DistributionSetupResponse.h"
}

namespace oai::ngap {

class DistributionSetupResponseMsg : public NgapMessage {
 public:
  DistributionSetupResponseMsg();
  ~DistributionSetupResponseMsg() = default;

  void initialize();

  void setMbsSessionId(const MbsSessionId& v);
  bool getMbsSessionId(MbsSessionId& v) const;

  void setMbsAreaSessionId(const MbsAreaSessionId& v);
  bool getMbsAreaSessionId(MbsAreaSessionId& v) const;

  void setMbsDistributionSetupResponseTransfer(
      const MbsDistributionSetupResponseTransfer& v);

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_DistributionSetupResponse_t* m_DistributionSetupResponseIes;

  MbsSessionId m_MbsSessionId;                              // Mandatory
  std::optional<MbsAreaSessionId> m_MbsAreaSessionId;       // Optional
  std::optional<CriticalityDiagnostics> m_CriticalityDiag;  // Optional
};

}  // namespace oai::ngap
#endif
