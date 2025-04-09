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

#include "QosFlowWithCauseItem.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
QosFlowWithCauseItem::QosFlowWithCauseItem() {}

//------------------------------------------------------------------------------
QosFlowWithCauseItem::~QosFlowWithCauseItem() {}

//------------------------------------------------------------------------------
void QosFlowWithCauseItem::set(
    const QosFlowIdentifier& qosFlowIdentifier, const Cause& cause) {
  m_QosFlowIdentifier = qosFlowIdentifier;
  m_Cause             = cause;
}

//------------------------------------------------------------------------------
bool QosFlowWithCauseItem::get(
    QosFlowIdentifier& qosFlowIdentifier, Cause& cause) const {
  qosFlowIdentifier = m_QosFlowIdentifier;
  cause             = m_Cause;

  return true;
}

//------------------------------------------------------------------------------
void QosFlowWithCauseItem::setQosFlowIdentifier(
    const QosFlowIdentifier& qosFlowIdentifier) {
  m_QosFlowIdentifier = qosFlowIdentifier;
}

//------------------------------------------------------------------------------
bool QosFlowWithCauseItem::getQosFlowIdentifier(
    QosFlowIdentifier& qosFlowIdentifier) const {
  qosFlowIdentifier = m_QosFlowIdentifier;
  return true;
}

//------------------------------------------------------------------------------
void QosFlowWithCauseItem::setCause(const Cause& cause) {
  m_Cause = cause;
}

//------------------------------------------------------------------------------
bool QosFlowWithCauseItem::getCause(Cause& cause) const {
  cause = m_Cause;

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowWithCauseItem::encode(
    Ngap_QosFlowWithCauseItem_t& QosFlowWithCauseItem) const {
  if (!m_QosFlowIdentifier.encode(QosFlowWithCauseItem.qosFlowIdentifier))
    return false;
  if (!m_Cause.encode(QosFlowWithCauseItem.cause)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowWithCauseItem::decode(
    const Ngap_QosFlowWithCauseItem_t& QosFlowWithCauseItem) {
  if (!m_QosFlowIdentifier.decode(QosFlowWithCauseItem.qosFlowIdentifier))
    return false;
  if (!m_Cause.decode(QosFlowWithCauseItem.cause)) return false;

  return true;
}
}  // namespace oai::ngap
