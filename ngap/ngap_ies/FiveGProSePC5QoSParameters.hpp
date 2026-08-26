/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _FIVE_G_PRO_SE_PC5_QOS_PARAMETERS_H_
#define _FIVE_G_PRO_SE_PC5_QOS_PARAMETERS_H_

extern "C" {
#include "Ngap_FiveG-ProSePC5QoSParameters.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-FiveGProSePC5QoSParameters (9.3.1.234, Rel-17).
 */
class FiveGProSePC5QoSParameters {
 public:
  FiveGProSePC5QoSParameters()  = default;
  ~FiveGProSePC5QoSParameters() = default;

  bool encode(Ngap_FiveG_ProSePC5QoSParameters_t& value) const;
  bool decode(const Ngap_FiveG_ProSePC5QoSParameters_t& value);

  const Ngap_FiveG_ProSePC5QoSParameters_t& get() const { return m_Value; }
  void set(const Ngap_FiveG_ProSePC5QoSParameters_t& v) { m_Value = v; }

 private:
  Ngap_FiveG_ProSePC5QoSParameters_t m_Value{};
};

}  // namespace oai::ngap
#endif
