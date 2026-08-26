/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "IabSupported.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool IabSupported::encode(Ngap_IAB_Supported_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool IabSupported::decode(const Ngap_IAB_Supported_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
