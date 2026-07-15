/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MBS_SESSION_RELEASE_RESPONSE_TRANSFER_H_
#define _MBS_SESSION_RELEASE_RESPONSE_TRANSFER_H_

#include <cstdint>

extern "C" {
#include "Ngap_MBSSessionReleaseResponseTransfer.h"
}

namespace oai::ngap {

/**
 * Thin APER wrapper for Ngap_MBSSessionReleaseResponseTransfer_t.
 * Full IE field access is Stage 8 territory.
 */
class MbsSessionReleaseResponseTransfer {
 public:
  MbsSessionReleaseResponseTransfer();
  virtual ~MbsSessionReleaseResponseTransfer();

  int encode(uint8_t* buf, int bufSize) const;
  bool decode(const uint8_t* buf, int bufSize);

  const Ngap_MBSSessionReleaseResponseTransfer_t* getIe() const { return m_Ie; }

 private:
  Ngap_MBSSessionReleaseResponseTransfer_t* m_Ie;
};

}  // namespace oai::ngap
#endif
