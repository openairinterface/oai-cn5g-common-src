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
  cagId.buf         = (uint8_t*) calloc(1, sizeof(uint32_t) + sizeof(uint8_t));
  if (!cagId.buf) return false;
  cagId.buf[5] = m_CagId & 0x00000000ff;
  cagId.buf[4] = (m_CagId & 0x000000ff00) >> 8;
  cagId.buf[3] = (m_CagId & 0x0000ff0000) >> 16;
  cagId.buf[2] = (m_CagId & 0x00ff000000) >> 24;
  cagId.buf[1] = (m_CagId & 0xff00000000) >> 32;
  cagId.buf[0] = (m_CagId & 0xff00000000) >> 40;
  return true;
}

//------------------------------------------------------------------------------
bool CagId::decode(const Ngap_CAG_ID_t& cagId) {
  if (!cagId.buf) return false;
  if (cagId.size < 6) return false;

  m_CagId = cagId.buf[0];
  m_CagId = m_CagId << 40;
  m_CagId |= cagId.buf[1] << 32;
  m_CagId |= cagId.buf[2] << 24;
  m_CagId |= cagId.buf[3] << 16;
  m_CagId |= cagId.buf[4] << 8;
  m_CagId |= cagId.buf[5];

  m_CagId = m_CagId >> 4;  // 44 bits, so number of unused bits = 4
  return true;
}

//------------------------------------------------------------------------------
uint64_t CagId::get() const {
  return (m_CagId & 0x0ffffffffff);  // Get 44 LSB
}
}  // namespace oai::ngap
