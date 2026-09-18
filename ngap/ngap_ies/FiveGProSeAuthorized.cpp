/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "FiveGProSeAuthorized.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool FiveGProSeAuthorized::encode(Ngap_FiveG_ProSeAuthorized_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool FiveGProSeAuthorized::decode(const Ngap_FiveG_ProSeAuthorized_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
