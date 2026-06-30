/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _QMC_DEACTIVATION_H_
#define _QMC_DEACTIVATION_H_

extern "C" {
#include "Ngap_QMCDeactivation.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-QMCDeactivation (Rel-17).
 */
class QmcDeactivation {
 public:
  QmcDeactivation()  = default;
  ~QmcDeactivation() = default;

  bool encode(Ngap_QMCDeactivation_t& value) const;
  bool decode(const Ngap_QMCDeactivation_t& value);

  const Ngap_QMCDeactivation_t& get() const { return m_Value; }
  void set(const Ngap_QMCDeactivation_t& v) { m_Value = v; }

 private:
  Ngap_QMCDeactivation_t m_Value{};
};

}  // namespace oai::ngap
#endif
