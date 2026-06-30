/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _TIME_SYNCHRONISATION_ASSISTANCE_INFO_H_
#define _TIME_SYNCHRONISATION_ASSISTANCE_INFO_H_

extern "C" {
#include "Ngap_TimeSyncAssistanceInfo.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-TimeSynchronisationAssistanceInfo (9.3.1.220, Rel-17).
 * Underlying ASN.1 type: Ngap_TimeSyncAssistanceInfo_t.
 */
class TimeSynchronisationAssistanceInfo {
 public:
  TimeSynchronisationAssistanceInfo()  = default;
  ~TimeSynchronisationAssistanceInfo() = default;

  bool encode(Ngap_TimeSyncAssistanceInfo_t& value) const;
  bool decode(const Ngap_TimeSyncAssistanceInfo_t& value);

  const Ngap_TimeSyncAssistanceInfo_t& get() const { return m_Value; }
  void set(const Ngap_TimeSyncAssistanceInfo_t& v) { m_Value = v; }

 private:
  Ngap_TimeSyncAssistanceInfo_t m_Value{};
};

}  // namespace oai::ngap
#endif
