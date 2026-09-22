/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _CELL_CAG_LIST_H_
#define _CELL_CAG_LIST_H_

#include <vector>

#include "CagId.hpp"

extern "C" {
#include "Ngap_CellCAGList.h"
}

namespace oai::ngap {

class CellCagList {
 public:
  CellCagList();
  virtual ~CellCagList();

  void set(const std::vector<CagId>& list);
  void get(std::vector<CagId>& list) const;

  bool encode(Ngap_CellCAGList_t& cellCagList) const;
  bool decode(const Ngap_CellCAGList_t& cellCagList);

 private:
  std::vector<CagId> m_ItemList;
  constexpr static uint8_t kMaxNoOfCAGsPerCell = 12;
};
}  // namespace oai::ngap
#endif
