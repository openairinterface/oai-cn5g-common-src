/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "UeSliceMaximumBitRateList.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool UeSliceMaximumBitRateList::encode(
    Ngap_UESliceMaximumBitRateList_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool UeSliceMaximumBitRateList::decode(
    const Ngap_UESliceMaximumBitRateList_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
