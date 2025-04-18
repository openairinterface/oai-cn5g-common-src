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

#include "QosFlowItemWithDataForwarding.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
QosFlowItemWithDataForwarding::QosFlowItemWithDataForwarding() {
  m_DataForwardingAccepted = std::nullopt;
}

//------------------------------------------------------------------------------
QosFlowItemWithDataForwarding::~QosFlowItemWithDataForwarding() {}

//------------------------------------------------------------------------------
void QosFlowItemWithDataForwarding::set(
    const QosFlowIdentifier& qosFlowIdentifier,
    const std::optional<long>& dataForwardingAccepted) {
  m_QosFlowIdentifier      = qosFlowIdentifier;
  m_DataForwardingAccepted = dataForwardingAccepted;
}

//------------------------------------------------------------------------------
void QosFlowItemWithDataForwarding::setQosFlowIdentifier(
    const QosFlowIdentifier& qosFlowIdentifier) {
  m_QosFlowIdentifier = qosFlowIdentifier;
}

//------------------------------------------------------------------------------
void QosFlowItemWithDataForwarding::getQosFlowIdentifier(
    QosFlowIdentifier& qosFlowIdentifier) const {
  qosFlowIdentifier = m_QosFlowIdentifier;
}

//------------------------------------------------------------------------------
void QosFlowItemWithDataForwarding::setDataForwardingAccepted(
    long dataForwardingAccepted) {
  m_DataForwardingAccepted = std::make_optional<long>(dataForwardingAccepted);
}

//------------------------------------------------------------------------------
void QosFlowItemWithDataForwarding::getDataForwardingAccepted(
    std::optional<long>& dataForwardingAccepted) const {
  dataForwardingAccepted = m_DataForwardingAccepted;
}
//------------------------------------------------------------------------------
bool QosFlowItemWithDataForwarding::encode(
    Ngap_QosFlowItemWithDataForwarding_t& item) const {
  if (!m_QosFlowIdentifier.encode(item.qosFlowIdentifier)) return false;
  if (m_DataForwardingAccepted.has_value()) {
    item.dataForwardingAccepted  = (long*) calloc(1, sizeof(long));
    *item.dataForwardingAccepted = m_DataForwardingAccepted.value();
  }

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowItemWithDataForwarding::decode(
    const Ngap_QosFlowItemWithDataForwarding_t& item) {
  if (!m_QosFlowIdentifier.decode(item.qosFlowIdentifier)) {
    return false;
  }
  if (item.dataForwardingAccepted)
    m_DataForwardingAccepted =
        std::make_optional<long>(*item.dataForwardingAccepted);
  return true;
}
}  // namespace oai::ngap
