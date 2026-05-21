/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "ExtendedAmfName.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool ExtendedAmfName::encode(Ngap_Extended_AMFName_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool ExtendedAmfName::decode(const Ngap_Extended_AMFName_t& value) {
  m_Value = value;
  return true;
}

//------------------------------------------------------------------------------
// Populate from a plain string using the VisibleString alternative (9.3.3.51).
// The string is stored in m_VisibleStringBuf to back the C pointer.
bool ExtendedAmfName::set(const std::string& name) {
  if (name.empty()) return false;
  m_VisibleStringBuf = name;
  // Allocate and assign the VisibleString field; free any previous allocation.
  if (!m_Value.aMFNameVisibleString) {
    m_Value.aMFNameVisibleString =
        static_cast<Ngap_AMFNameVisibleString_t*>(
            calloc(1, sizeof(Ngap_AMFNameVisibleString_t)));
    if (!m_Value.aMFNameVisibleString) return false;
  }
  m_Value.aMFNameVisibleString->buf =
      reinterpret_cast<uint8_t*>(
          const_cast<char*>(m_VisibleStringBuf.c_str()));
  m_Value.aMFNameVisibleString->size = m_VisibleStringBuf.size();
  return true;
}

}  // namespace oai::ngap
