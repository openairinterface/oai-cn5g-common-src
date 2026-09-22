/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _QMC_CONFIG_INFO_H_
#define _QMC_CONFIG_INFO_H_

extern "C" {
#include "Ngap_QMCConfigInfo.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-QMCConfigInfo (9.3.1.223, Rel-17).
 */
class QmcConfigInfo {
 public:
  QmcConfigInfo()  = default;
  ~QmcConfigInfo() = default;

  bool encode(Ngap_QMCConfigInfo_t& value) const;
  bool decode(const Ngap_QMCConfigInfo_t& value);

  const Ngap_QMCConfigInfo_t& get() const { return m_Value; }
  void set(const Ngap_QMCConfigInfo_t& v) { m_Value = v; }

 private:
  Ngap_QMCConfigInfo_t m_Value{};
};

}  // namespace oai::ngap
#endif
