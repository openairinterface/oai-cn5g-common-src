/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#include "QosFlowAddOrModifyRequestList.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
QosFlowAddOrModifyRequestList::QosFlowAddOrModifyRequestList() {}

//------------------------------------------------------------------------------
QosFlowAddOrModifyRequestList::~QosFlowAddOrModifyRequestList() {}

//------------------------------------------------------------------------------
void QosFlowAddOrModifyRequestList::set(
    const std::vector<QosFlowAddOrModifyRequestItem>& list) {
  m_ItemList = list;
  return;
}

//------------------------------------------------------------------------------
void QosFlowAddOrModifyRequestList::get(
    std::vector<QosFlowAddOrModifyRequestItem>& list) const {
  list = m_ItemList;
}

//------------------------------------------------------------------------------
void QosFlowAddOrModifyRequestList::addItem(
    const QosFlowAddOrModifyRequestItem& item) {
  m_ItemList.push_back(item);
}

//------------------------------------------------------------------------------
bool QosFlowAddOrModifyRequestList::encode(
    Ngap_QosFlowAddOrModifyRequestList_t& list) const {
  for (auto l : m_ItemList) {
    Ngap_QosFlowAddOrModifyRequestItem_t* item =
        (Ngap_QosFlowAddOrModifyRequestItem_t*) calloc(
            1, sizeof(Ngap_QosFlowAddOrModifyRequestItem_t));
    if (!item) return false;
    if (!l.encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&list.list, item) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool QosFlowAddOrModifyRequestList::decode(
    const Ngap_QosFlowAddOrModifyRequestList_t& list) {
  m_ItemList.clear();
  for (int i = 0; i < list.list.count; i++) {
    QosFlowAddOrModifyRequestItem item = {};
    if (!item.decode(*list.list.array[i])) return false;
    m_ItemList.push_back(item);
  }
  return true;
}

}  // namespace oai::ngap
