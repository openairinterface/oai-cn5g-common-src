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

#include "QosFlowAddOrModifyRequestItem.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
QosFlowAddOrModifyRequestItem::QosFlowAddOrModifyRequestItem() {}

//------------------------------------------------------------------------------
QosFlowAddOrModifyRequestItem::~QosFlowAddOrModifyRequestItem() {}

//------------------------------------------------------------------------------
void QosFlowAddOrModifyRequestItem::setQosFlowIdentifier(
    const QosFlowIdentifier& qosFlowIdentifier) {
  m_QosFlowIdentifier = qosFlowIdentifier;
}

//------------------------------------------------------------------------------
void QosFlowAddOrModifyRequestItem::getQosFlowIdentifier(
    QosFlowIdentifier& qosFlowIdentifier) const {
  qosFlowIdentifier = m_QosFlowIdentifier;
}

//------------------------------------------------------------------------------
void QosFlowAddOrModifyRequestItem::setQosFlowLevelQosParameters(
    const QosFlowLevelQosParameters& qosFlowLevelQosParameters) {
  m_QosFlowLevelQosParameters =
      std::make_optional<QosFlowLevelQosParameters>(qosFlowLevelQosParameters);
}

//------------------------------------------------------------------------------
void QosFlowAddOrModifyRequestItem::setQosFlowLevelQosParameters(
    const std::optional<QosFlowLevelQosParameters>& qosFlowLevelQosParameters) {
  m_QosFlowLevelQosParameters = qosFlowLevelQosParameters;
}

//------------------------------------------------------------------------------
void QosFlowAddOrModifyRequestItem::setQosFlowLevelQosParameters(
    std::optional<QosFlowLevelQosParameters>& qosFlowLevelQosParameters) const {
  qosFlowLevelQosParameters = m_QosFlowLevelQosParameters;
}

//------------------------------------------------------------------------------
bool QosFlowAddOrModifyRequestItem::encode(
    Ngap_QosFlowAddOrModifyRequestItem_t& QosFlowAddOrModifyRequestItem) const {
  if (!m_QosFlowIdentifier.encode(
          QosFlowAddOrModifyRequestItem.qosFlowIdentifier))
    return false;
  if (m_QosFlowLevelQosParameters.has_value()) {
    QosFlowAddOrModifyRequestItem.qosFlowLevelQosParameters =
        (Ngap_QosFlowLevelQosParameters_t*) calloc(
            1, sizeof(Ngap_QosFlowLevelQosParameters_t));
    if (!m_QosFlowLevelQosParameters.value().encode(
            *QosFlowAddOrModifyRequestItem.qosFlowLevelQosParameters))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowAddOrModifyRequestItem::decode(
    const Ngap_QosFlowAddOrModifyRequestItem_t& QosFlowAddOrModifyRequestItem) {
  if (!m_QosFlowIdentifier.decode(
          QosFlowAddOrModifyRequestItem.qosFlowIdentifier))
    return false;
  if (QosFlowAddOrModifyRequestItem.qosFlowLevelQosParameters) {
    QosFlowLevelQosParameters qosFlowLevelQosParameters = {};
    if (!qosFlowLevelQosParameters.decode(
            *QosFlowAddOrModifyRequestItem.qosFlowLevelQosParameters))
      return false;
    m_QosFlowLevelQosParameters = std::make_optional<QosFlowLevelQosParameters>(
        qosFlowLevelQosParameters);
  }

  return true;
}
}  // namespace oai::ngap
