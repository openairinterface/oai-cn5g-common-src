/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MBS_DISTRIBUTION_SETUP_REQUEST_TRANSFER_H_
#define _MBS_DISTRIBUTION_SETUP_REQUEST_TRANSFER_H_

#include <cstdint>

extern "C" {
#include "Ngap_MBS-DistributionSetupRequestTransfer.h"
}

namespace oai::ngap {

/**
 * Thin APER wrapper for Ngap_MBS_DistributionSetupRequestTransfer_t.
 * Full IE field access is Stage 8 territory.
 */
class MbsDistributionSetupRequestTransfer {
 public:
  MbsDistributionSetupRequestTransfer();
  virtual ~MbsDistributionSetupRequestTransfer();

  int encode(uint8_t* buf, int bufSize) const;
  bool decode(const uint8_t* buf, int bufSize);

  const Ngap_MBS_DistributionSetupRequestTransfer_t* getIe() const {
    return m_Ie;
  }

 private:
  Ngap_MBS_DistributionSetupRequestTransfer_t* m_Ie;
};

}  // namespace oai::ngap
#endif
