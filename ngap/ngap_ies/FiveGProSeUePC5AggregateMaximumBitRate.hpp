/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _FIVE_G_PRO_SE_UE_PC5_AGGREGATE_MAXIMUM_BIT_RATE_H_
#define _FIVE_G_PRO_SE_UE_PC5_AGGREGATE_MAXIMUM_BIT_RATE_H_

extern "C" {
#include "Ngap_NRUESidelinkAggregateMaximumBitrate.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-FiveGProSeUePC5AggregateMaximumBitRate (type: 9.3.1.148).
 * Uses NRUESidelinkAggregateMaximumBitrate as the underlying ASN.1 type
 * (per TS 38.413 v17.10.0 IE definition and ProtocolIE-Field.h).
 */
class FiveGProSeUePC5AggregateMaximumBitRate {
 public:
  FiveGProSeUePC5AggregateMaximumBitRate()  = default;
  ~FiveGProSeUePC5AggregateMaximumBitRate() = default;

  bool encode(Ngap_NRUESidelinkAggregateMaximumBitrate_t& value) const;
  bool decode(const Ngap_NRUESidelinkAggregateMaximumBitrate_t& value);

  const Ngap_NRUESidelinkAggregateMaximumBitrate_t& get() const {
    return m_Value;
  }
  void set(const Ngap_NRUESidelinkAggregateMaximumBitrate_t& v) {
    m_Value = v;
  }

 private:
  Ngap_NRUESidelinkAggregateMaximumBitrate_t m_Value{};
};

}  // namespace oai::ngap
#endif
