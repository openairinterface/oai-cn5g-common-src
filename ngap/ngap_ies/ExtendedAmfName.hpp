/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _EXTENDED_AMF_NAME_H_
#define _EXTENDED_AMF_NAME_H_

#include <cstdint>
#include <string>

extern "C" {
#include "Ngap_Extended-AMFName.h"
}

namespace oai::ngap {

// AMFNameVisibleString ::= VisibleString (SIZE(1..150,...))
constexpr uint8_t EXTENDED_AMF_NAME_SIZE_MAX = 150;

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

  /**
   * Set/get the VisibleString alternative (9.3.3.51).
   */
  bool set(const std::string& name);
  void get(std::string& name) const;

 private:
  std::string m_VisibleString;  // AMFNameVisibleString
};

}  // namespace oai::ngap
#endif
