/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "QmcDeactivation.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool QmcDeactivation::encode(Ngap_QMCDeactivation_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool QmcDeactivation::decode(const Ngap_QMCDeactivation_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
