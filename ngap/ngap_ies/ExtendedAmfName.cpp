/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "ExtendedAmfName.hpp"

#include "ngap_utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool ExtendedAmfName::encode(Ngap_Extended_AMFName_t& value) const {
  if (m_VisibleString.empty()) return false;

  value.aMFNameVisibleString = (Ngap_AMFNameVisibleString_t*) calloc(
      1, sizeof(Ngap_AMFNameVisibleString_t));
  if (!value.aMFNameVisibleString) return false;

  ngap_utils::string_2_octet_string(
      m_VisibleString, *value.aMFNameVisibleString);
  if (!value.aMFNameVisibleString->buf) {
    free(value.aMFNameVisibleString);
    value.aMFNameVisibleString = nullptr;
    return false;
  }
  // TODO: AMFNameUTF8String
  return true;
}

//------------------------------------------------------------------------------
bool ExtendedAmfName::decode(const Ngap_Extended_AMFName_t& value) {
  // TODO: AMFNameUTF8String
  if (!value.aMFNameVisibleString) return false;
  if (!ngap_utils::check_octet_string(*value.aMFNameVisibleString))
    return false;
  if (value.aMFNameVisibleString->size > EXTENDED_AMF_NAME_SIZE_MAX)
    return false;

  ngap_utils::octet_string_2_string(
      *value.aMFNameVisibleString, m_VisibleString);
  return true;
}

//------------------------------------------------------------------------------
bool ExtendedAmfName::set(const std::string& name) {
  if (name.empty() or (name.size() > EXTENDED_AMF_NAME_SIZE_MAX)) return false;
  for (const char c : name) {
    const auto value = static_cast<unsigned char>(c);
    if ((value < 32) or (value > 126)) return false;
  }
  m_VisibleString = name;
  return true;
}

//------------------------------------------------------------------------------
void ExtendedAmfName::get(std::string& name) const {
  name = m_VisibleString;
}

}  // namespace oai::ngap
