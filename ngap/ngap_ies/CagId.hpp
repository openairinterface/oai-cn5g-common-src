/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _CAG_ID_H_
#define _CAG_ID_H_

extern "C" {
#include "Ngap_CAG-ID.h"
}

namespace oai::ngap {

class CagId {
 public:
  CagId();
  virtual ~CagId();

  uint64_t get() const;
  void set(const uint64_t& cagId);

  bool encode(Ngap_CAG_ID_t& cagId) const;
  bool decode(const Ngap_CAG_ID_t& cagId);

 private:
  uint64_t m_CagId;  // Bitstring (size 44)
};
}  // namespace oai::ngap

#endif
