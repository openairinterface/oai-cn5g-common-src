/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "CagId.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
CagId::CagId() {
  m_CagId = 0;
}

//------------------------------------------------------------------------------
CagId::~CagId() {}

//------------------------------------------------------------------------------
void CagId::set(const uint64_t& cagId) {
  m_CagId = cagId;
}

//------------------------------------------------------------------------------
bool CagId::encode(Ngap_CAG_ID_t& cagId) const {
  // Cag ID is defined as BIT STRING (SIZE(44))
  cagId.bits_unused = 4;
  cagId.size        = 6;
  cagId.buf         = (uint8_t*) calloc(cagId.size, sizeof(uint8_t));
  if (!cagId.buf) return false;

  // 44 bits, left-aligned in 6 octets: 4 unused bits at the end
  const uint64_t value = (m_CagId & 0x00000fffffffffff) << 4;
  cagId.buf[5]         = value & 0x0000000000ff;
  cagId.buf[4]         = (value & 0x00000000ff00) >> 8;
  cagId.buf[3]         = (value & 0x000000ff0000) >> 16;
  cagId.buf[2]         = (value & 0x0000ff000000) >> 24;
  cagId.buf[1]         = (value & 0x00ff00000000) >> 32;
  cagId.buf[0]         = (value & 0xff0000000000) >> 40;
  return true;
}

//------------------------------------------------------------------------------
bool CagId::decode(const Ngap_CAG_ID_t& cagId) {
  if (!cagId.buf) return false;
  if (cagId.size < 6) return false;

  m_CagId = static_cast<uint64_t>(cagId.buf[0]) << 40;
  m_CagId |= static_cast<uint64_t>(cagId.buf[1]) << 32;
  m_CagId |= static_cast<uint64_t>(cagId.buf[2]) << 24;
  m_CagId |= static_cast<uint64_t>(cagId.buf[3]) << 16;
  m_CagId |= static_cast<uint64_t>(cagId.buf[4]) << 8;
  m_CagId |= static_cast<uint64_t>(cagId.buf[5]);

  m_CagId = m_CagId >> 4;  // 44 bits, so number of unused bits = 4
  return true;
}

//------------------------------------------------------------------------------
uint64_t CagId::get() const {
  return (m_CagId & 0x00000fffffffffff);  // Get 44 LSB
}
}  // namespace oai::ngap
