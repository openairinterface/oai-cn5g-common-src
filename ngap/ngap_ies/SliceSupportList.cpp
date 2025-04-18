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

#include "SliceSupportList.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
SliceSupportList::SliceSupportList() {}

//------------------------------------------------------------------------------
SliceSupportList::~SliceSupportList() {}

//------------------------------------------------------------------------------
bool SliceSupportList::encode(Ngap_SliceSupportList_t& SliceSupportList) const {
  for (std::vector<SNssai>::const_iterator it = m_SliceSupportItems.begin();
       it < m_SliceSupportItems.end(); ++it) {
    Ngap_SliceSupportItem_t* ta =
        (Ngap_SliceSupportItem_t*) calloc(1, sizeof(Ngap_SliceSupportItem_t));
    if (!it->encode(ta->s_NSSAI)) return false;
    if (ASN_SEQUENCE_ADD(&SliceSupportList.list, ta) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool SliceSupportList::decode(const Ngap_SliceSupportList_t& SliceSupportList) {
  for (int i = 0; i < SliceSupportList.list.count; i++) {
    SNssai item = {};
    if (!item.decode(SliceSupportList.list.array[i]->s_NSSAI)) return false;
    m_SliceSupportItems.push_back(item);
  }

  return true;
}

//------------------------------------------------------------------------------
void SliceSupportList::setSliceSupportItems(const std::vector<SNssai>& items) {
  m_SliceSupportItems = items;
}

//------------------------------------------------------------------------------
void SliceSupportList::getSliceSupportItems(std::vector<SNssai>& items) const {
  items = m_SliceSupportItems;
}

}  // namespace oai::ngap
