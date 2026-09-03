/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MULTICAST_SESSION_UPDATE_REQUEST_TRANSFER_H_
#define _MULTICAST_SESSION_UPDATE_REQUEST_TRANSFER_H_

#include <cstdint>

extern "C" {
#include "Ngap_MulticastSessionUpdateRequestTransfer.h"
}

namespace oai::ngap {

/**
 * Thin APER wrapper for Ngap_MulticastSessionUpdateRequestTransfer_t.
 * Full IE field access is Stage 8 territory.
 */
class MulticastSessionUpdateRequestTransfer {
 public:
  MulticastSessionUpdateRequestTransfer();
  virtual ~MulticastSessionUpdateRequestTransfer();

  int encode(uint8_t* buf, int bufSize) const;
  bool decode(const uint8_t* buf, int bufSize);

  const Ngap_MulticastSessionUpdateRequestTransfer_t* getIe() const {
    return m_Ie;
  }

 private:
  Ngap_MulticastSessionUpdateRequestTransfer_t* m_Ie;
};

}  // namespace oai::ngap
#endif
