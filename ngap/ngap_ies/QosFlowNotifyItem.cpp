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

#include "QosFlowNotifyItem.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
QosFlowNotifyItem::QosFlowNotifyItem() {}

//------------------------------------------------------------------------------
QosFlowNotifyItem::~QosFlowNotifyItem() {}

//------------------------------------------------------------------------------
void QosFlowNotifyItem::setQosFlowIdentifier(
    const QosFlowIdentifier& qosFlowIdentifier) {
  m_QosFlowIdentifier = qosFlowIdentifier;
}

//------------------------------------------------------------------------------
void QosFlowNotifyItem::getQosFlowIdentifier(
    QosFlowIdentifier& qosFlowIdentifier) const {
  qosFlowIdentifier = m_QosFlowIdentifier;
}

//------------------------------------------------------------------------------
void QosFlowNotifyItem::setNotificationCause(
    const NotificationCause& notificationCause) {
  m_NotificationCause = notificationCause;
}

//------------------------------------------------------------------------------
void QosFlowNotifyItem::getNotificationCause(
    NotificationCause& notificationCause) const {
  notificationCause = m_NotificationCause;
}

/*
//------------------------------------------------------------------------------
void QosFlowNotifyItem::setCurrentQoSParametersSetIndex(uint32_t&
          currentQoSParametersSetIndex) {
        m_CurrentQoSParametersSetIndex =
std::make_optional<uint32_t>(currentQoSParametersSetIndex);
}
//------------------------------------------------------------------------------
void
QosFlowNotifyItem::getCurrentQoSParametersSetIndex(std::optional<uint32_t>&
                  currentQoSParametersSetIndex) const{
        m_CurrentQoSParametersSetIndex = currentQoSParametersSetIndex;
}
*/

//------------------------------------------------------------------------------
bool QosFlowNotifyItem::encode(
    Ngap_QosFlowNotifyItem_t& QosFlowNotifyItem) const {
  if (!m_QosFlowIdentifier.encode(QosFlowNotifyItem.qosFlowIdentifier))
    return false;

  if (!m_NotificationCause.encode(QosFlowNotifyItem.notificationCause))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowNotifyItem::decode(
    const Ngap_QosFlowNotifyItem_t& QosFlowNotifyItem) {
  if (!m_QosFlowIdentifier.decode(QosFlowNotifyItem.qosFlowIdentifier))
    return false;

  if (!m_NotificationCause.decode(QosFlowNotifyItem.notificationCause))
    return false;

  return true;
}
}  // namespace oai::ngap
