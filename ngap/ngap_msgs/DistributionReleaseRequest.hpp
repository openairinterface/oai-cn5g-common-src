/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _DISTRIBUTION_RELEASE_REQUEST_H_
#define _DISTRIBUTION_RELEASE_REQUEST_H_

#include <optional>

#include "Cause.hpp"
#include "MbsAreaSessionId.hpp"
#include "MbsDistributionReleaseRequestTransfer.hpp"
#include "MbsSessionId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_DistributionReleaseRequest.h"
}

namespace oai::ngap {

class DistributionReleaseRequestMsg : public NgapMessage {
 public:
  DistributionReleaseRequestMsg();
  ~DistributionReleaseRequestMsg() = default;

  void initialize();

  bool getMbsSessionId(MbsSessionId& v) const;
  bool getMbsAreaSessionId(MbsAreaSessionId& v) const;
  bool getCause(Cause& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_DistributionReleaseRequest_t* m_DistributionReleaseRequestIes;

  MbsSessionId m_MbsSessionId;                         // Mandatory
  std::optional<MbsAreaSessionId> m_MbsAreaSessionId;  // Optional
  Cause m_Cause;                                       // Mandatory
};

}  // namespace oai::ngap
#endif
