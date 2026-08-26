/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "IabAuthorized.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool IabAuthorized::encode(Ngap_IAB_Authorized_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool IabAuthorized::decode(const Ngap_IAB_Authorized_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
