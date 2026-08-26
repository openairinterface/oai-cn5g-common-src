/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "FiveGProSeUePC5AggregateMaximumBitRate.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool FiveGProSeUePC5AggregateMaximumBitRate::encode(
    Ngap_NRUESidelinkAggregateMaximumBitrate_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool FiveGProSeUePC5AggregateMaximumBitRate::decode(
    const Ngap_NRUESidelinkAggregateMaximumBitrate_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
