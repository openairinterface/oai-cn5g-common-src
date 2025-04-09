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

#include "QosFlowListWithDataForwarding.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
QosFlowListWithDataForwarding::QosFlowListWithDataForwarding() {}

//------------------------------------------------------------------------------
QosFlowListWithDataForwarding::~QosFlowListWithDataForwarding() {}

//------------------------------------------------------------------------------
void QosFlowListWithDataForwarding::set(
    const std::vector<QosFlowItemWithDataForwarding>& list) {
  uint8_t number_items =
      (list.size() > KMaxNoOfQosFlows) ? KMaxNoOfQosFlows : list.size();
  m_ItemList.insert(
      m_ItemList.begin(), list.begin(), list.begin() + number_items);
}

//------------------------------------------------------------------------------
void QosFlowListWithDataForwarding::get(
    std::vector<QosFlowItemWithDataForwarding>& list) const {
  list = m_ItemList;
}

//------------------------------------------------------------------------------
bool QosFlowListWithDataForwarding::decode(
    const Ngap_QosFlowListWithDataForwarding& list) {
  for (int i = 0; i < list.list.count; i++) {
    QosFlowItemWithDataForwarding item = {};
    if (!item.decode(*list.list.array[i])) return false;
    m_ItemList.push_back(item);
  }
  return true;
}
}  // namespace oai::ngap
