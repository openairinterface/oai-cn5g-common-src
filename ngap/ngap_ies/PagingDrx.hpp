/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _PAGING_DRX_H_
#define _PAGING_DRX_H_

extern "C" {
#include "Ngap_PagingDRX.h"
}

namespace oai::ngap {

class PagingDrx {
 public:
  PagingDrx();
  virtual ~PagingDrx();

  void set(const e_Ngap_PagingDRX& pagingDrx);
  e_Ngap_PagingDRX get() const;

  bool encode(Ngap_PagingDRX_t&) const;
  bool decode(const Ngap_PagingDRX_t&);

 private:
  e_Ngap_PagingDRX m_PagingDrx;
};

}  // namespace oai::ngap

#endif
