/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "NrPagingEDrxInformation.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool NrPagingEDrxInformation::encode(
    Ngap_NR_PagingeDRXInformation_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool NrPagingEDrxInformation::decode(
    const Ngap_NR_PagingeDRXInformation_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
