/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MbsSessionId.hpp"

#include <cstring>

namespace oai::ngap {

//------------------------------------------------------------------------------
void MbsSessionId::setTmgi(const uint8_t* buf, size_t len) {
  m_Tmgi.assign(buf, buf + len);
}

//------------------------------------------------------------------------------
bool MbsSessionId::getTmgi(uint8_t*& buf, size_t& len) const {
  if (m_Tmgi.empty()) return false;
  buf = const_cast<uint8_t*>(m_Tmgi.data());
  len = m_Tmgi.size();
  return true;
}

//------------------------------------------------------------------------------
void MbsSessionId::setNid(const uint8_t* buf, size_t len, uint8_t bitsUnused) {
  m_NidBuf.assign(buf, buf + len);
  m_NidBitsUnused = bitsUnused;
  m_HasNid        = true;
}

//------------------------------------------------------------------------------
bool MbsSessionId::getNid(
    uint8_t*& buf, size_t& len, uint8_t& bitsUnused) const {
  if (!m_HasNid) return false;
  buf        = const_cast<uint8_t*>(m_NidBuf.data());
  len        = m_NidBuf.size();
  bitsUnused = m_NidBitsUnused;
  return true;
}

//------------------------------------------------------------------------------
bool MbsSessionId::encode(Ngap_MBS_SessionID_t& ie) const {
  if (m_Tmgi.empty()) return false;
  OCTET_STRING_fromBuf(
      &ie.tMGI, reinterpret_cast<const char*>(m_Tmgi.data()), m_Tmgi.size());

  if (m_HasNid) {
    ie.nID = (Ngap_NID_t*) calloc(1, sizeof(Ngap_NID_t));
    if (!ie.nID) return false;
    ie.nID->buf = (uint8_t*) calloc(m_NidBuf.size(), sizeof(uint8_t));
    if (!ie.nID->buf) return false;
    memcpy(ie.nID->buf, m_NidBuf.data(), m_NidBuf.size());
    ie.nID->size        = m_NidBuf.size();
    ie.nID->bits_unused = m_NidBitsUnused;
  }
  return true;
}

//------------------------------------------------------------------------------
bool MbsSessionId::decode(const Ngap_MBS_SessionID_t& ie) {
  if (!ie.tMGI.buf || ie.tMGI.size == 0) return false;
  m_Tmgi.assign(ie.tMGI.buf, ie.tMGI.buf + ie.tMGI.size);

  m_HasNid = false;
  if (ie.nID && ie.nID->buf && ie.nID->size > 0) {
    m_NidBuf.assign(ie.nID->buf, ie.nID->buf + ie.nID->size);
    m_NidBitsUnused = ie.nID->bits_unused;
    m_HasNid        = true;
  }
  return true;
}

}  // namespace oai::ngap
