/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "QosFlowPerTnlInformationItem.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
QosFlowPerTnlInformationItem::QosFlowPerTnlInformationItem() {}

//------------------------------------------------------------------------------
QosFlowPerTnlInformationItem::~QosFlowPerTnlInformationItem() {}

//------------------------------------------------------------------------------
void QosFlowPerTnlInformationItem::set(
    const QosFlowPerTnlInformation& qosFlowPerTNLInformation) {
  m_QosFlowPerTnlInformation = qosFlowPerTNLInformation;
}

//------------------------------------------------------------------------------
void QosFlowPerTnlInformationItem::get(
    QosFlowPerTnlInformation& qosFlowPerTNLInformation) const {
  qosFlowPerTNLInformation = m_QosFlowPerTnlInformation;
}

//------------------------------------------------------------------------------
bool QosFlowPerTnlInformationItem::encode(
    Ngap_QosFlowPerTNLInformationItem_t& qosFlowPerTnlInformationItem) const {
  if (!qosFlowPerTnlInformationItem.qosFlowPerTNLInformation)
    qosFlowPerTnlInformationItem.qosFlowPerTNLInformation =
        (Ngap_QosFlowPerTNLInformation_t*) calloc(
            1, sizeof(Ngap_QosFlowPerTNLInformation_t));
  if (!m_QosFlowPerTnlInformation.encode(
          *qosFlowPerTnlInformationItem.qosFlowPerTNLInformation))
    return false;
  return true;
}

//------------------------------------------------------------------------------
bool QosFlowPerTnlInformationItem::decode(
    const Ngap_QosFlowPerTNLInformationItem_t& qosFlowPerTnlInformationItem) {
  if (!qosFlowPerTnlInformationItem.qosFlowPerTNLInformation) return false;
  if (!m_QosFlowPerTnlInformation.decode(
          *qosFlowPerTnlInformationItem.qosFlowPerTNLInformation))
    return false;

  return true;
}

}  // namespace oai::ngap
