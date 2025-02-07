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

#include "QosFlowToBeForwardedList.hpp"

#include "logger_base.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
QosFlowToBeForwardedList::QosFlowToBeForwardedList() {}

//------------------------------------------------------------------------------
void QosFlowToBeForwardedList::set(
    const std::vector<QosFlowToBeForwardedItem>& list) {
  uint8_t number_items =
      (list.size() > kMaxNoOfQosFlows) ? kMaxNoOfQosFlows : list.size();
  m_ItemList.insert(
      m_ItemList.begin(), list.begin(), list.begin() + number_items);
}

//------------------------------------------------------------------------------
void QosFlowToBeForwardedList::get(
    std::vector<QosFlowToBeForwardedItem>& list) const {
  list = m_ItemList;
}

//------------------------------------------------------------------------------
bool QosFlowToBeForwardedList::encode(
    Ngap_QosFlowToBeForwardedList*& list) const {
  list = (Ngap_QosFlowToBeForwardedList_t*) calloc(
      1, sizeof(Ngap_QosFlowToBeForwardedList_t));
  for (int i = 0; i < m_ItemList.size(); i++) {
    Ngap_QosFlowToBeForwardedItem_t* response =
        (Ngap_QosFlowToBeForwardedItem_t*) calloc(
            1, sizeof(Ngap_QosFlowToBeForwardedItem_t));
    if (!response) return false;
    if (!m_ItemList[i].encode(*response)) {
      oai::logger::logger_common::ngap().debug(
          "Encode QosFlowToBeForwardedItem error");
      return false;
    }
    oai::logger::logger_common::ngap().debug(
        "QFI %d", response->qosFlowIdentifier);

    if (ASN_SEQUENCE_ADD(&list->list, response) != 0) {
      oai::logger::logger_common::ngap().debug(
          "Encode QosFlowToBeForwardedList error");
      return false;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool QosFlowToBeForwardedList::decode(
    const Ngap_QosFlowToBeForwardedList& list) {
  for (int i = 0; i < list.list.count; i++) {
    QosFlowToBeForwardedItem item = {};
    if (!item.decode(*list.list.array[i])) return false;
    m_ItemList.push_back(item);
  }
  return true;
}

}  // namespace oai::ngap
