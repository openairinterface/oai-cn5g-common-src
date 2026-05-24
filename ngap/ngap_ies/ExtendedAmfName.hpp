/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _EXTENDED_AMF_NAME_H_
#define _EXTENDED_AMF_NAME_H_

#include <string>
#include <cstring>

extern "C" {
#include "Ngap_Extended-AMFName.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-ExtendedAMFName (9.3.3.51, Rel-17).
 *
 * SECURITY NOTE: source from AMF configuration only — never echo from gNB.
 */
class ExtendedAmfName {
 public:
  ExtendedAmfName()  = default;
  ~ExtendedAmfName() = default;

  bool encode(Ngap_Extended_AMFName_t& value) const;
  bool decode(const Ngap_Extended_AMFName_t& value);

  const Ngap_Extended_AMFName_t& get() const { return m_Value; }
  void set(const Ngap_Extended_AMFName_t& v) { m_Value = v; }

  /**
   * Set from a plain string using the VisibleString alternative (§9.3.3.51).
   * Stores pointers into members; the caller must ensure the ExtendedAmfName
   * lifetime exceeds the encode call.
   */
  bool set(const std::string& name);

 private:
  Ngap_Extended_AMFName_t m_Value{};
  Ngap_AMFNameVisibleString_t m_VisibleString{};
  std::string m_VisibleStringBuf;  // backing store for set(const std::string&)
};

}  // namespace oai::ngap
#endif
