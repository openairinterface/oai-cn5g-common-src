/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MBS_DISTRIBUTION_RELEASE_REQUEST_TRANSFER_H_
#define _MBS_DISTRIBUTION_RELEASE_REQUEST_TRANSFER_H_

#include <cstdint>

extern "C" {
#include "Ngap_MBS-DistributionReleaseRequestTransfer.h"
}

namespace oai::ngap {

/**
 * Thin APER wrapper for Ngap_MBS_DistributionReleaseRequestTransfer_t.
 * Full IE field access is Stage 8 territory.
 */
class MbsDistributionReleaseRequestTransfer {
 public:
  MbsDistributionReleaseRequestTransfer();
  virtual ~MbsDistributionReleaseRequestTransfer();

  int encode(uint8_t* buf, int bufSize) const;
  bool decode(const uint8_t* buf, int bufSize);

  const Ngap_MBS_DistributionReleaseRequestTransfer_t* getIe() const {
    return m_Ie;
  }

 private:
  Ngap_MBS_DistributionReleaseRequestTransfer_t* m_Ie;
};

}  // namespace oai::ngap
#endif
