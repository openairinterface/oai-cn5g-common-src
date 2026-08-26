/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MANAGEMENT_BASED_MDT_PLMN_MODIFICATION_LIST_H_
#define _MANAGEMENT_BASED_MDT_PLMN_MODIFICATION_LIST_H_

extern "C" {
#include "Ngap_MDTPLMNModificationList.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-ManagementBasedMDTPLMNModificationList (9.3.1.243, Rel-17).
 */
class ManagementBasedMdtPlmnModificationList {
 public:
  ManagementBasedMdtPlmnModificationList()  = default;
  ~ManagementBasedMdtPlmnModificationList() = default;

  bool encode(Ngap_MDTPLMNModificationList_t& value) const;
  bool decode(const Ngap_MDTPLMNModificationList_t& value);

  const Ngap_MDTPLMNModificationList_t& get() const { return m_Value; }
  void set(const Ngap_MDTPLMNModificationList_t& v) { m_Value = v; }

 private:
  Ngap_MDTPLMNModificationList_t m_Value{};
};

}  // namespace oai::ngap
#endif
