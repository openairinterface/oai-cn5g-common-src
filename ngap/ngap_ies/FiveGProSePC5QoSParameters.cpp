/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "FiveGProSePC5QoSParameters.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool FiveGProSePC5QoSParameters::encode(
    Ngap_FiveG_ProSePC5QoSParameters_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool FiveGProSePC5QoSParameters::decode(
    const Ngap_FiveG_ProSePC5QoSParameters_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
