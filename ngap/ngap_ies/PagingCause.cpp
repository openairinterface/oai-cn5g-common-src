/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "PagingCause.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool PagingCause::encode(Ngap_PagingCause_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool PagingCause::decode(const Ngap_PagingCause_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
