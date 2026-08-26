/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "NgapUeRadioCapabilityId.hpp"

#include <cstring>

namespace oai::ngap {

//------------------------------------------------------------------------------
bool NgapUeRadioCapabilityId::encode(Ngap_UERadioCapabilityID_t& value) const {
  if (m_Data.empty()) return false;
  uint8_t* buf = static_cast<uint8_t*>(calloc(m_Data.size(), sizeof(uint8_t)));
  if (!buf) return false;
  std::memcpy(buf, m_Data.data(), m_Data.size());
  value.buf  = buf;
  value.size = m_Data.size();
  return true;
}

//------------------------------------------------------------------------------
bool NgapUeRadioCapabilityId::decode(const Ngap_UERadioCapabilityID_t& value) {
  if (!value.buf || value.size == 0) return false;
  m_Data.assign(value.buf, value.buf + value.size);
  return true;
}

//------------------------------------------------------------------------------
void NgapUeRadioCapabilityId::set(const std::vector<uint8_t>& data) {
  m_Data = data;
}

//------------------------------------------------------------------------------
void NgapUeRadioCapabilityId::get(std::vector<uint8_t>& data) const {
  data = m_Data;
}

}  // namespace oai::ngap
