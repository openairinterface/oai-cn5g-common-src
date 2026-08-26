/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "DrbStatusUl18.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
DrbStatusUl18::DrbStatusUl18() {}

//------------------------------------------------------------------------------
DrbStatusUl18::~DrbStatusUl18() {}

//------------------------------------------------------------------------------
void DrbStatusUl18::get(CountValueForPdcpSn18& countValue) const {
  countValue = m_PdcpValue;
}

//------------------------------------------------------------------------------
void DrbStatusUl18::set(const CountValueForPdcpSn18& countValue) {
  m_PdcpValue = countValue;
}

//------------------------------------------------------------------------------
bool DrbStatusUl18::encode(Ngap_DRBStatusUL18_t& ul18) const {
  ul18.uL_COUNTValue = (Ngap_COUNTValueForPDCP_SN18_t*) calloc(
      1, sizeof(Ngap_COUNTValueForPDCP_SN18_t));
  if (!ul18.uL_COUNTValue) return false;
  if (!m_PdcpValue.encode(*ul18.uL_COUNTValue)) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool DrbStatusUl18::decode(const Ngap_DRBStatusUL18_t& ul18) {
  if (!ul18.uL_COUNTValue) return false;
  if (!m_PdcpValue.decode(*ul18.uL_COUNTValue)) {
    return false;
  }
  return true;
}
}  // namespace oai::ngap
