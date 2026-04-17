/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "PagingPriority.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
PagingPriority::PagingPriority() {
  m_PagingPriority = Ngap_PagingPriority_priolevel1;
}

//------------------------------------------------------------------------------
PagingPriority::~PagingPriority() {}

//------------------------------------------------------------------------------
void PagingPriority::set(e_Ngap_PagingPriority priority) {
  m_PagingPriority = priority;
}

//------------------------------------------------------------------------------
void PagingPriority::setFromPpi(uint8_t ppi) {
  // TS 23.501 §5.6.3: PPI 0 = highest priority → priolevel1 (enum 0)
  // PPI 7 = lowest priority → priolevel8 (enum 7); direct mapping
  if (ppi > 7) ppi = 7;
  m_PagingPriority = static_cast<e_Ngap_PagingPriority>(ppi);
}

//------------------------------------------------------------------------------
e_Ngap_PagingPriority PagingPriority::get() const {
  return m_PagingPriority;
}

//------------------------------------------------------------------------------
bool PagingPriority::encode(Ngap_PagingPriority_t& pagingPriority) const {
  pagingPriority = m_PagingPriority;
  return true;
}

//------------------------------------------------------------------------------
bool PagingPriority::decode(const Ngap_PagingPriority_t& pagingPriority) {
  m_PagingPriority = static_cast<e_Ngap_PagingPriority>(pagingPriority);
  return true;
}

}  // namespace oai::ngap
