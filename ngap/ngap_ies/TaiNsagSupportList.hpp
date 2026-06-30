/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _TAI_NSAG_SUPPORT_LIST_H_
#define _TAI_NSAG_SUPPORT_LIST_H_

extern "C" {
#include "Ngap_TAINSAGSupportList.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-TAINSAGSupportList (9.3.1.238, Rel-17).
 *
 * Nested inside BroadcastPlmnItem of SupportedTA/NgSetupRequest.
 * SECURITY NOTE: sourced from AMF configuration (gnb_context), not UE input.
 */
class TaiNsagSupportList {
 public:
  TaiNsagSupportList()  = default;
  ~TaiNsagSupportList() = default;

  bool encode(Ngap_TAINSAGSupportList_t& value) const;
  bool decode(const Ngap_TAINSAGSupportList_t& value);

  const Ngap_TAINSAGSupportList_t& get() const { return m_Value; }
  void set(const Ngap_TAINSAGSupportList_t& v) { m_Value = v; }

 private:
  Ngap_TAINSAGSupportList_t m_Value{};
};

}  // namespace oai::ngap
#endif
