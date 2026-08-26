/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "ManagementBasedMdtPlmnList.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool ManagementBasedMdtPlmnList::encode(Ngap_MDTPLMNList_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool ManagementBasedMdtPlmnList::decode(const Ngap_MDTPLMNList_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
