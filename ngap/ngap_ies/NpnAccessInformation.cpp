/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "NpnAccessInformation.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool NpnAccessInformation::encode(
    Ngap_NPN_AccessInformation_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool NpnAccessInformation::decode(
    const Ngap_NPN_AccessInformation_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
