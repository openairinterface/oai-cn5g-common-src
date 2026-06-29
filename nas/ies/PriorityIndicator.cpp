/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "PriorityIndicator.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.hpp"
#include "logger_base.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
PriorityIndicator::PriorityIndicator() : Type1NasIeFormatTv() {}

//------------------------------------------------------------------------------
PriorityIndicator::PriorityIndicator(uint8_t mpsi) : Type1NasIeFormatTv() {
  SetMpsi(mpsi);
}

//------------------------------------------------------------------------------
PriorityIndicator::PriorityIndicator(uint8_t iei, uint8_t mpsi)
    : Type1NasIeFormatTv(iei) {
  SetMpsi(mpsi);
}

//------------------------------------------------------------------------------
void PriorityIndicator::SetMpsi(uint8_t mpsi) {
  // Lower nibble: bits 4-2 are spare (0), bit 1 is MPSI.
  SetValue(mpsi & 0x01);
}

//------------------------------------------------------------------------------
uint8_t PriorityIndicator::GetMpsi() const {
  return GetValue() & 0x01;
}
