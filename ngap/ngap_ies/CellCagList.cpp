/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "CellCagList.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
CellCagList::CellCagList() {}

//------------------------------------------------------------------------------
CellCagList::~CellCagList() {}

//------------------------------------------------------------------------------
void CellCagList::set(const std::vector<CagId>& list) {
  uint8_t number_items =
      (list.size() > kMaxNoOfCAGsPerCell) ? kMaxNoOfCAGsPerCell : list.size();
  m_ItemList.insert(
      m_ItemList.begin(), list.begin(), list.begin() + number_items);
}

//------------------------------------------------------------------------------
void CellCagList::get(std::vector<CagId>& list) const {
  list = m_ItemList;
}

//------------------------------------------------------------------------------
bool CellCagList::encode(Ngap_CellCAGList_t& cellCagList) const {
  for (int i = 0; i < m_ItemList.size(); i++) {
    Ngap_CAG_ID_t* item = (Ngap_CAG_ID_t*) calloc(1, sizeof(Ngap_CAG_ID_t));
    if (!item) return false;
    if (!m_ItemList[i].encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&cellCagList.list, item) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool CellCagList::decode(const Ngap_CellCAGList_t& cellCagList) {
  for (int i = 0; i < cellCagList.list.count; i++) {
    CagId item = {};
    if (!item.decode(*cellCagList.list.array[i])) return false;
    m_ItemList.push_back(item);
  }
  return true;
}

}  // namespace oai::ngap
