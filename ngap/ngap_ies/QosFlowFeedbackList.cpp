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

#include "QosFlowFeedbackList.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
QosFlowFeedbackList::QosFlowFeedbackList() {}

//------------------------------------------------------------------------------
QosFlowFeedbackList::~QosFlowFeedbackList() {}

//------------------------------------------------------------------------------
void QosFlowFeedbackList::set(const std::vector<QosFlowFeedbackItem>& list) {
  m_ItemList = list;
  return;
}

//------------------------------------------------------------------------------
void QosFlowFeedbackList::get(std::vector<QosFlowFeedbackItem>& list) const {
  list = m_ItemList;
}

//------------------------------------------------------------------------------
void QosFlowFeedbackList::addItem(const QosFlowFeedbackItem& item) {
  m_ItemList.push_back(item);
}

//------------------------------------------------------------------------------
bool QosFlowFeedbackList::encode(Ngap_QosFlowFeedbackList_t& list) const {
  for (auto l : m_ItemList) {
    Ngap_QosFlowFeedbackItem_t* item = (Ngap_QosFlowFeedbackItem_t*) calloc(
        1, sizeof(Ngap_QosFlowFeedbackItem_t));
    if (!item) return false;
    if (!l.encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&list.list, item) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool QosFlowFeedbackList::decode(const Ngap_QosFlowFeedbackList_t& list) {
  m_ItemList.clear();
  for (int i = 0; i < list.list.count; i++) {
    QosFlowFeedbackItem item = {};
    if (!item.decode(*list.list.array[i])) return false;
    m_ItemList.push_back(item);
  }
  return true;
}

}  // namespace oai::ngap
