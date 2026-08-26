/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "QmcConfigInfo.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool QmcConfigInfo::encode(Ngap_QMCConfigInfo_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool QmcConfigInfo::decode(const Ngap_QMCConfigInfo_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
