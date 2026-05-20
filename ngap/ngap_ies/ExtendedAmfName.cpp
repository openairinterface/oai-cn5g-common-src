/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "ExtendedAmfName.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool ExtendedAmfName::encode(Ngap_Extended_AMFName_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool ExtendedAmfName::decode(const Ngap_Extended_AMFName_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
