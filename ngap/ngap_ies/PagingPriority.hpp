/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _PAGING_PRIORITY_H_
#define _PAGING_PRIORITY_H_

#include <cstdint>

extern "C" {
#include "Ngap_PagingPriority.h"
}

namespace oai::ngap {

class PagingPriority {
 public:
  PagingPriority();
  virtual ~PagingPriority();

  void set(e_Ngap_PagingPriority priority);
  // PPI 0 (highest, TS 23.501 §5.6.3) → priolevel1; PPI 7 → priolevel8
  void setFromPpi(uint8_t ppi);
  e_Ngap_PagingPriority get() const;

  bool encode(Ngap_PagingPriority_t&) const;
  bool decode(const Ngap_PagingPriority_t&);

 private:
  e_Ngap_PagingPriority m_PagingPriority;
};

}  // namespace oai::ngap

#endif
