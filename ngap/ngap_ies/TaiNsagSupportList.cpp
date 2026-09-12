/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "TaiNsagSupportList.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool TaiNsagSupportList::encode(Ngap_TAINSAGSupportList_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool TaiNsagSupportList::decode(const Ngap_TAINSAGSupportList_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
