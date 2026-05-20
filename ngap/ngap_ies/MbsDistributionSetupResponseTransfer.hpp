/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MBS_DISTRIBUTION_SETUP_RESPONSE_TRANSFER_H_
#define _MBS_DISTRIBUTION_SETUP_RESPONSE_TRANSFER_H_

#include <cstdint>

extern "C" {
#include "Ngap_MBS-DistributionSetupResponseTransfer.h"
}

namespace oai::ngap {

/**
 * Thin APER wrapper for Ngap_MBS_DistributionSetupResponseTransfer_t.
 * Full IE field access is Stage 8 territory.
 */
class MbsDistributionSetupResponseTransfer {
 public:
  MbsDistributionSetupResponseTransfer();
  virtual ~MbsDistributionSetupResponseTransfer();

  int encode(uint8_t* buf, int bufSize) const;
  bool decode(const uint8_t* buf, int bufSize);

  const Ngap_MBS_DistributionSetupResponseTransfer_t* getIe() const {
    return m_Ie;
  }

 private:
  Ngap_MBS_DistributionSetupResponseTransfer_t* m_Ie;
};

}  // namespace oai::ngap
#endif
