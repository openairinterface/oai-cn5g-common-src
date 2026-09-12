/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _TARGET_NSSAI_INFORMATION_H_
#define _TARGET_NSSAI_INFORMATION_H_

extern "C" {
#include "Ngap_TargetNSSAIInformation.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-TargetNSSAIInformation (9.3.1.229, Rel-17).
 */
class TargetNssaiInformation {
 public:
  TargetNssaiInformation()  = default;
  ~TargetNssaiInformation() = default;

  bool encode(Ngap_TargetNSSAIInformation_t& value) const;
  bool decode(const Ngap_TargetNSSAIInformation_t& value);

  const Ngap_TargetNSSAIInformation_t& get() const { return m_Value; }
  void set(const Ngap_TargetNSSAIInformation_t& v) { m_Value = v; }

 private:
  Ngap_TargetNSSAIInformation_t m_Value{};
};

}  // namespace oai::ngap
#endif
