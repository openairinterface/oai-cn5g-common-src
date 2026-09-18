/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MBS_SESSION_SETUP_OR_MOD_FAILURE_TRANSFER_H_
#define _MBS_SESSION_SETUP_OR_MOD_FAILURE_TRANSFER_H_

#include <cstdint>

extern "C" {
#include "Ngap_MBSSessionSetupOrModFailureTransfer.h"
}

namespace oai::ngap {

/**
 * Thin APER wrapper for Ngap_MBSSessionSetupOrModFailureTransfer_t.
 * Full IE field access is Stage 8 territory.
 */
class MbsSessionSetupOrModFailureTransfer {
 public:
  MbsSessionSetupOrModFailureTransfer();
  virtual ~MbsSessionSetupOrModFailureTransfer();

  int encode(uint8_t* buf, int bufSize) const;
  bool decode(const uint8_t* buf, int bufSize);

  const Ngap_MBSSessionSetupOrModFailureTransfer_t* getIe() const {
    return m_Ie;
  }

 private:
  Ngap_MBSSessionSetupOrModFailureTransfer_t* m_Ie;
};

}  // namespace oai::ngap
#endif
