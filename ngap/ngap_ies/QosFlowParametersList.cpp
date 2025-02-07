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

#include "QosFlowParametersList.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
QosFlowParametersList::QosFlowParametersList() {}

//------------------------------------------------------------------------------
QosFlowParametersList::~QosFlowParametersList() {}

//------------------------------------------------------------------------------
void QosFlowParametersList::set(
    const std::vector<QosFlowParametersItem>& list) {
  m_ItemList = list;
  return;
}

//------------------------------------------------------------------------------
void QosFlowParametersList::get(
    std::vector<QosFlowParametersItem>& list) const {
  list = m_ItemList;
}

//------------------------------------------------------------------------------
void QosFlowParametersList::addItem(const QosFlowParametersItem& item) {
  m_ItemList.push_back(item);
}

//------------------------------------------------------------------------------
bool QosFlowParametersList::encode(Ngap_QosFlowParametersList_t& list) const {
  for (auto l : m_ItemList) {
    Ngap_QosFlowParametersItem_t* item = (Ngap_QosFlowParametersItem_t*) calloc(
        1, sizeof(Ngap_QosFlowParametersItem_t));
    if (!item) return false;
    if (!l.encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&list.list, item) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool QosFlowParametersList::decode(const Ngap_QosFlowParametersList_t& list) {
  m_ItemList.clear();
  for (int i = 0; i < list.list.count; i++) {
    QosFlowParametersItem item = {};
    if (!item.decode(*list.list.array[i])) return false;
    m_ItemList.push_back(item);
  }
  return true;
}

}  // namespace oai::ngap
