/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MANAGEMENT_BASED_MDT_PLMN_LIST_H_
#define _MANAGEMENT_BASED_MDT_PLMN_LIST_H_

extern "C" {
#include "Ngap_MDTPLMNList.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-ManagementBasedMDTPLMNList (9.3.1.168).
 */
class ManagementBasedMdtPlmnList {
 public:
  ManagementBasedMdtPlmnList()  = default;
  ~ManagementBasedMdtPlmnList() = default;

  bool encode(Ngap_MDTPLMNList_t& value) const;
  bool decode(const Ngap_MDTPLMNList_t& value);

  const Ngap_MDTPLMNList_t& get() const { return m_Value; }
  void set(const Ngap_MDTPLMNList_t& v) { m_Value = v; }

 private:
  Ngap_MDTPLMNList_t m_Value{};
};

}  // namespace oai::ngap
#endif
