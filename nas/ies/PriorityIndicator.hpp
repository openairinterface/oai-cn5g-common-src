/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _PRIORITY_INDICATOR_H_
#define _PRIORITY_INDICATOR_H_

#include "Type1NasIeFormatTv.hpp"

constexpr uint8_t kPriorityIndicatorIei = 0x0E;  // upper nibble value
constexpr auto kPriorityIndicatorIeName = "Priority Indicator";

namespace oai::nas {

class PriorityIndicator : public Type1NasIeFormatTv {
 public:
  PriorityIndicator();
  explicit PriorityIndicator(uint8_t mpsi);
  PriorityIndicator(uint8_t iei, uint8_t mpsi);
  ~PriorityIndicator() override = default;

  static std::string GetIeName() { return kPriorityIndicatorIeName; }

  // Set MPSI (MPS Indicator) bit: 0 = no MPS, 1 = MPS.
  void SetMpsi(uint8_t mpsi);
  uint8_t GetMpsi() const;
};

}  // namespace oai::nas

#endif
