/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MULTICAST_SESSION_DEACTIVATION_REQUEST_TRANSFER_H_
#define _MULTICAST_SESSION_DEACTIVATION_REQUEST_TRANSFER_H_

#include <cstdint>

extern "C" {
#include "Ngap_MulticastSessionDeactivationRequestTransfer.h"
}

namespace oai::ngap {

/**
 * Thin APER wrapper for Ngap_MulticastSessionDeactivationRequestTransfer_t.
 * Full IE field access is Stage 8 territory.
 */
class MulticastSessionDeactivationRequestTransfer {
 public:
  MulticastSessionDeactivationRequestTransfer();
  virtual ~MulticastSessionDeactivationRequestTransfer();

  int encode(uint8_t* buf, int bufSize) const;
  bool decode(const uint8_t* buf, int bufSize);

  const Ngap_MulticastSessionDeactivationRequestTransfer_t* getIe() const {
    return m_Ie;
  }

 private:
  Ngap_MulticastSessionDeactivationRequestTransfer_t* m_Ie;
};

}  // namespace oai::ngap
#endif
