/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _IAB_SUPPORTED_H_
#define _IAB_SUPPORTED_H_

extern "C" {
#include "Ngap_IAB-Supported.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-IABSupported (Rel-17).
 * Security: must be populated from AMF configuration only (never from gNB
 * input). Wiring to AMF config is deferred to Stage 8.
 */
class IabSupported {
 public:
  IabSupported()  = default;
  ~IabSupported() = default;

  bool encode(Ngap_IAB_Supported_t& value) const;
  bool decode(const Ngap_IAB_Supported_t& value);

  Ngap_IAB_Supported_t get() const { return m_Value; }
  void set(Ngap_IAB_Supported_t v) { m_Value = v; }

 private:
  Ngap_IAB_Supported_t m_Value{};
};

}  // namespace oai::ngap
#endif
