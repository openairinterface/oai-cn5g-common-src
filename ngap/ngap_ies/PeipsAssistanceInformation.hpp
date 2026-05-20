/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _PEIPS_ASSISTANCE_INFORMATION_H_
#define _PEIPS_ASSISTANCE_INFORMATION_H_

extern "C" {
#include "Ngap_PEIPSassistanceInformation.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-PEIPSAssistanceInformation (9.3.1.232, Rel-17).
 */
class PeipsAssistanceInformation {
 public:
  PeipsAssistanceInformation()  = default;
  ~PeipsAssistanceInformation() = default;

  bool encode(Ngap_PEIPSassistanceInformation_t& value) const;
  bool decode(const Ngap_PEIPSassistanceInformation_t& value);

  const Ngap_PEIPSassistanceInformation_t& get() const { return m_Value; }
  void set(const Ngap_PEIPSassistanceInformation_t& v) { m_Value = v; }

 private:
  Ngap_PEIPSassistanceInformation_t m_Value{};
};

}  // namespace oai::ngap
#endif
