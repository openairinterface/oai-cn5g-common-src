/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "RedCapIndication.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool RedCapIndication::encode(Ngap_RedCapIndication_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool RedCapIndication::decode(const Ngap_RedCapIndication_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
