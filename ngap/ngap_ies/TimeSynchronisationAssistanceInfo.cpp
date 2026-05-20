/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "TimeSynchronisationAssistanceInfo.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool TimeSynchronisationAssistanceInfo::encode(
    Ngap_TimeSyncAssistanceInfo_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool TimeSynchronisationAssistanceInfo::decode(
    const Ngap_TimeSyncAssistanceInfo_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
