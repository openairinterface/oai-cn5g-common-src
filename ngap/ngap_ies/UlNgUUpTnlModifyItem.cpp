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

#include "UlNgUUpTnlModifyItem.hpp"

#include "utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
UlNgUUpTnlModifyItem::UlNgUUpTnlModifyItem() {}

//------------------------------------------------------------------------------
UlNgUUpTnlModifyItem::~UlNgUUpTnlModifyItem() {}

//------------------------------------------------------------------------------
void UlNgUUpTnlModifyItem::set(
    const UpTransportLayerInformation& ulNgUUpTnlInformation,
    const UpTransportLayerInformation& dlNgUUpTnlInformation) {
  m_UlNgUUpTnlInformation = ulNgUUpTnlInformation;
  m_DlNgUUpTnlInformation = dlNgUUpTnlInformation;
}
//------------------------------------------------------------------------------
void UlNgUUpTnlModifyItem::get(
    UpTransportLayerInformation& ulNgUUpTnlInformation,
    UpTransportLayerInformation& dlNgUUpTnlInformation) const {
  ulNgUUpTnlInformation = m_UlNgUUpTnlInformation;
  dlNgUUpTnlInformation = m_DlNgUUpTnlInformation;
}

//------------------------------------------------------------------------------
void UlNgUUpTnlModifyItem::setUlNgUUpTnlInformation(
    const UpTransportLayerInformation& ulNgUUpTnlInformation) {
  m_UlNgUUpTnlInformation = ulNgUUpTnlInformation;
}

//------------------------------------------------------------------------------
void UlNgUUpTnlModifyItem::getUlNgUUpTnlInformation(
    UpTransportLayerInformation& ulNgUUpTnlInformation) const {
  ulNgUUpTnlInformation = m_UlNgUUpTnlInformation;
}

//------------------------------------------------------------------------------
void UlNgUUpTnlModifyItem::setDlNgUUpTnlInformation(
    const UpTransportLayerInformation& dlNgUUpTnlInformation) {
  m_DlNgUUpTnlInformation = dlNgUUpTnlInformation;
}
//------------------------------------------------------------------------------
void UlNgUUpTnlModifyItem::getDlNgUUpTnlInformation(
    UpTransportLayerInformation& dlNgUUpTnlInformation) const {
  dlNgUUpTnlInformation = m_DlNgUUpTnlInformation;
}

//------------------------------------------------------------------------------
bool UlNgUUpTnlModifyItem::encode(
    Ngap_UL_NGU_UP_TNLModifyItem_t& ulNgUUpTnlModifyItem) const {
  if (!m_UlNgUUpTnlInformation.encode(
          ulNgUUpTnlModifyItem.uL_NGU_UP_TNLInformation)) {
    return false;
  }
  if (!m_DlNgUUpTnlInformation.encode(
          ulNgUUpTnlModifyItem.dL_NGU_UP_TNLInformation)) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UlNgUUpTnlModifyItem::decode(
    const Ngap_UL_NGU_UP_TNLModifyItem_t& ulNgUUpTnlModifyItem) {
  if (!m_UlNgUUpTnlInformation.decode(
          ulNgUUpTnlModifyItem.uL_NGU_UP_TNLInformation))
    false;
  if (!m_DlNgUUpTnlInformation.decode(
          ulNgUUpTnlModifyItem.dL_NGU_UP_TNLInformation))
    return false;

  return true;
}

}  // namespace oai::ngap
