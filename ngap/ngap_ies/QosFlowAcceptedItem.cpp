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

#include "QosFlowAcceptedItem.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
QosFlowAcceptedItem::QosFlowAcceptedItem() {}

//------------------------------------------------------------------------------
QosFlowAcceptedItem::~QosFlowAcceptedItem() {}

//------------------------------------------------------------------------------
void QosFlowAcceptedItem::setQosFlowIdentifier(
    const QosFlowIdentifier& qosFlowIdentifier) {
  m_QosFlowIdentifier = qosFlowIdentifier;
}

//------------------------------------------------------------------------------
void QosFlowAcceptedItem::getQosFlowIdentifier(
    QosFlowIdentifier& qosFlowIdentifier) const {
  qosFlowIdentifier = m_QosFlowIdentifier;
}

/*
//------------------------------------------------------------------------------
void QosFlowAcceptedItem::setCurrentQoSParametersSetIndex(uint32_t&
          currentQoSParametersSetIndex) {
        m_CurrentQoSParametersSetIndex =
std::make_optional<uint32_t>(currentQoSParametersSetIndex);
}
//------------------------------------------------------------------------------
void
QosFlowAcceptedItem::getCurrentQoSParametersSetIndex(std::optional<uint32_t>&
                  currentQoSParametersSetIndex) const{
        m_CurrentQoSParametersSetIndex = currentQoSParametersSetIndex;
}
*/

//------------------------------------------------------------------------------
bool QosFlowAcceptedItem::encode(
    Ngap_QosFlowAcceptedItem_t& QosFlowAcceptedItem) const {
  if (!m_QosFlowIdentifier.encode(QosFlowAcceptedItem.qosFlowIdentifier))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowAcceptedItem::decode(
    const Ngap_QosFlowAcceptedItem_t& QosFlowAcceptedItem) {
  if (!m_QosFlowIdentifier.decode(QosFlowAcceptedItem.qosFlowIdentifier))
    return false;

  return true;
}
}  // namespace oai::ngap
