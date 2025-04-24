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

#include "PlmnSupportItem.hpp"

extern "C" {
#include "Ngap_SliceSupportItem.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
PlmnSupportItem::PlmnSupportItem() {}

//------------------------------------------------------------------------------
PlmnSupportItem::~PlmnSupportItem() {}

//------------------------------------------------------------------------------
void PlmnSupportItem::set(
    const PlmnId& plmnId, const std::vector<SNssai>& sNssais) {
  m_PlmnId = plmnId;
  m_SliceSupportList.setSliceSupportItems(sNssais);
}

//------------------------------------------------------------------------------
void PlmnSupportItem::get(PlmnId& plmnId, std::vector<SNssai>& sNssais) const {
  plmnId = m_PlmnId;
  m_SliceSupportList.getSliceSupportItems(sNssais);
}

//------------------------------------------------------------------------------
void PlmnSupportItem::setPlmn(const PlmnId& plmnId) {
  m_PlmnId = plmnId;
}

//------------------------------------------------------------------------------
void PlmnSupportItem::getPlmn(PlmnId& plmnId) const {
  plmnId = m_PlmnId;
}

//------------------------------------------------------------------------------
void PlmnSupportItem::setSliceSupportList(
    const SliceSupportList& sliceSupportList) {
  m_SliceSupportList = sliceSupportList;
}

//------------------------------------------------------------------------------
void PlmnSupportItem::getSliceSupportList(
    SliceSupportList& sliceSupportList) const {
  sliceSupportList = m_SliceSupportList;
}

//------------------------------------------------------------------------------
bool PlmnSupportItem::encode(Ngap_PLMNSupportItem_t& plmnSupportItem) const {
  if (!m_PlmnId.encode(plmnSupportItem.pLMNIdentity)) return false;
  if (!m_SliceSupportList.encode(plmnSupportItem.sliceSupportList))
    return false;
  return true;
}

//------------------------------------------------------------------------------
bool PlmnSupportItem::decode(const Ngap_PLMNSupportItem_t& plmnSupportItem) {
  if (!m_PlmnId.decode(plmnSupportItem.pLMNIdentity)) return false;
  if (!m_SliceSupportList.decode(plmnSupportItem.sliceSupportList))
    return false;
  return true;
}
}  // namespace oai::ngap
