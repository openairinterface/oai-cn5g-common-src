/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _FIVE_G_PRO_SE_AUTHORIZED_H_
#define _FIVE_G_PRO_SE_AUTHORIZED_H_

extern "C" {
#include "Ngap_FiveG-ProSeAuthorized.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-FiveGProSeAuthorized (9.3.1.233, Rel-17).
 *
 * SECURITY NOTE: encode() sources data exclusively from UDM-provided ProSe
 * context (ue_context->getProseContext(), gated by proseContextIsSet()).
 * Encoding from gNB input is NEVER permitted. Wiring to AMF UE context is
 * deferred to Stage 8; this wrapper carries the raw struct only.
 */
class FiveGProSeAuthorized {
 public:
  FiveGProSeAuthorized()  = default;
  ~FiveGProSeAuthorized() = default;

  bool encode(Ngap_FiveG_ProSeAuthorized_t& value) const;
  bool decode(const Ngap_FiveG_ProSeAuthorized_t& value);

  const Ngap_FiveG_ProSeAuthorized_t& get() const { return m_Value; }
  void set(const Ngap_FiveG_ProSeAuthorized_t& v) { m_Value = v; }

 private:
  Ngap_FiveG_ProSeAuthorized_t m_Value{};
};

}  // namespace oai::ngap
#endif
