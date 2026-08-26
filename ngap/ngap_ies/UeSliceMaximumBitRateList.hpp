/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _UE_SLICE_MAXIMUM_BIT_RATE_LIST_H_
#define _UE_SLICE_MAXIMUM_BIT_RATE_LIST_H_

extern "C" {
#include "Ngap_UESliceMaximumBitRateList.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-UESliceMaximumBitRateList (9.3.1.231, Rel-17).
 */
class UeSliceMaximumBitRateList {
 public:
  UeSliceMaximumBitRateList()  = default;
  ~UeSliceMaximumBitRateList() = default;

  bool encode(Ngap_UESliceMaximumBitRateList_t& value) const;
  bool decode(const Ngap_UESliceMaximumBitRateList_t& value);

  const Ngap_UESliceMaximumBitRateList_t& get() const { return m_Value; }
  void set(const Ngap_UESliceMaximumBitRateList_t& v) { m_Value = v; }

 private:
  Ngap_UESliceMaximumBitRateList_t m_Value{};
};

}  // namespace oai::ngap
#endif
