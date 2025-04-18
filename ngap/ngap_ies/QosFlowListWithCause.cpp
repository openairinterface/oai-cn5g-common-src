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

#include "QosFlowListWithCause.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
QosFlowListWithCause::QosFlowListWithCause() {}

//------------------------------------------------------------------------------
QosFlowListWithCause::~QosFlowListWithCause() {}

//------------------------------------------------------------------------------
void QosFlowListWithCause::set(const std::vector<QosFlowWithCauseItem>& list) {
  m_ItemList = list;
}

//------------------------------------------------------------------------------
void QosFlowListWithCause::get(std::vector<QosFlowWithCauseItem>& list) const {
  list = m_ItemList;
}

//------------------------------------------------------------------------------
void QosFlowListWithCause::addItem(const QosFlowWithCauseItem& item) {
  m_ItemList.push_back(item);
}

//------------------------------------------------------------------------------
bool QosFlowListWithCause::encode(
    Ngap_QosFlowListWithCause_t& QosFlowListWithCause) const {
  for (auto l : m_ItemList) {
    Ngap_QosFlowWithCauseItem_t* item = (Ngap_QosFlowWithCauseItem_t*) calloc(
        1, sizeof(Ngap_QosFlowWithCauseItem_t));
    if (!item) return false;
    if (!l.encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&QosFlowListWithCause.list, item) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool QosFlowListWithCause::decode(
    const Ngap_QosFlowListWithCause_t& QosFlowListWithCause) {
  m_ItemList.clear();
  for (int i = 0; i < QosFlowListWithCause.list.count; i++) {
    QosFlowWithCauseItem item = {};
    if (!item.decode(*QosFlowListWithCause.list.array[i])) return false;
    m_ItemList.push_back(item);
  }
  return true;
}
}  // namespace oai::ngap
