/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MBS_SESSION_SETUP_OR_MODIFICATION_REQUEST_TRANSFER_H_
#define _MBS_SESSION_SETUP_OR_MODIFICATION_REQUEST_TRANSFER_H_

#include <cstdint>

extern "C" {
#include "Ngap_MBSSessionSetupOrModRequestTransfer.h"
}

namespace oai::ngap {

/**
 * Thin APER wrapper for Ngap_MBSSessionSetupOrModRequestTransfer_t.
 * Full IE field access is Stage 8 territory.
 */
class MbsSessionSetupOrModificationRequestTransfer {
 public:
  MbsSessionSetupOrModificationRequestTransfer();
  virtual ~MbsSessionSetupOrModificationRequestTransfer();

  int encode(uint8_t* buf, int bufSize) const;
  bool decode(const uint8_t* buf, int bufSize);

  const Ngap_MBSSessionSetupOrModRequestTransfer_t* getIe() const {
    return m_Ie;
  }

 private:
  Ngap_MBSSessionSetupOrModRequestTransfer_t* m_Ie;
};

}  // namespace oai::ngap
#endif
