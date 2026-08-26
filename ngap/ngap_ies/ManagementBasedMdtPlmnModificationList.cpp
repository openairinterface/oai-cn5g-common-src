/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "ManagementBasedMdtPlmnModificationList.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool ManagementBasedMdtPlmnModificationList::encode(
    Ngap_MDTPLMNModificationList_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool ManagementBasedMdtPlmnModificationList::decode(
    const Ngap_MDTPLMNModificationList_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
