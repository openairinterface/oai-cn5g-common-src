/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _DISTRIBUTION_RELEASE_RESPONSE_H_
#define _DISTRIBUTION_RELEASE_RESPONSE_H_

#include <optional>

#include "CriticalityDiagnostics.hpp"
#include "MbsAreaSessionId.hpp"
#include "MbsSessionId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_DistributionReleaseResponse.h"
}

namespace oai::ngap {

class DistributionReleaseResponseMsg : public NgapMessage {
 public:
  DistributionReleaseResponseMsg();
  ~DistributionReleaseResponseMsg() = default;

  void initialize();

  void setMbsSessionId(const MbsSessionId& v);
  bool getMbsSessionId(MbsSessionId& v) const;

  void setMbsAreaSessionId(const MbsAreaSessionId& v);
  bool getMbsAreaSessionId(MbsAreaSessionId& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_DistributionReleaseResponse_t* m_DistributionReleaseResponseIes;

  MbsSessionId m_MbsSessionId;                              // Mandatory
  std::optional<MbsAreaSessionId> m_MbsAreaSessionId;       // Optional
  std::optional<CriticalityDiagnostics> m_CriticalityDiag;  // Optional
};

}  // namespace oai::ngap
#endif
