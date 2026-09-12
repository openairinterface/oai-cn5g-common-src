/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MbsAreaSessionId.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool MbsAreaSessionId::encode(Ngap_MBS_AreaSessionID_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool MbsAreaSessionId::decode(const Ngap_MBS_AreaSessionID_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
