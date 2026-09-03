/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "TargetNssaiInformation.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool TargetNssaiInformation::encode(
    Ngap_TargetNSSAIInformation_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool TargetNssaiInformation::decode(
    const Ngap_TargetNSSAIInformation_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
