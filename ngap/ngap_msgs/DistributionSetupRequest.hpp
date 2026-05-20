/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _DISTRIBUTION_SETUP_REQUEST_H_
#define _DISTRIBUTION_SETUP_REQUEST_H_

#include <optional>

#include "MbsAreaSessionId.hpp"
#include "MbsDistributionSetupRequestTransfer.hpp"
#include "MbsSessionId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_DistributionSetupRequest.h"
}

namespace oai::ngap {

class DistributionSetupRequestMsg : public NgapMessage {
 public:
  DistributionSetupRequestMsg();
  ~DistributionSetupRequestMsg() = default;

  void initialize();

  bool getMbsSessionId(MbsSessionId& v) const;
  bool getMbsAreaSessionId(MbsAreaSessionId& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_DistributionSetupRequest_t* m_DistributionSetupRequestIes;

  MbsSessionId m_MbsSessionId;                         // Mandatory
  std::optional<MbsAreaSessionId> m_MbsAreaSessionId;  // Optional
};

}  // namespace oai::ngap
#endif
