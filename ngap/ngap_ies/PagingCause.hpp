/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _PAGING_CAUSE_H_
#define _PAGING_CAUSE_H_

extern "C" {
#include "Ngap_PagingCause.h"
}

namespace oai::ngap {

class PagingCause {
 public:
  PagingCause()  = default;
  ~PagingCause() = default;

  bool encode(Ngap_PagingCause_t& value) const;
  bool decode(const Ngap_PagingCause_t& value);

  Ngap_PagingCause_t get() const { return m_Value; }
  void set(Ngap_PagingCause_t v) { m_Value = v; }

 private:
  Ngap_PagingCause_t m_Value{};
};

}  // namespace oai::ngap
#endif
