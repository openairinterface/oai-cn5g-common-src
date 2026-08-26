/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "PeipsAssistanceInformation.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool PeipsAssistanceInformation::encode(
    Ngap_PEIPSassistanceInformation_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool PeipsAssistanceInformation::decode(
    const Ngap_PEIPSassistanceInformation_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
