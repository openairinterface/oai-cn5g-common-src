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

#ifndef _PDU_SESSION_RESOURCE_MODIFY_INDICATION_TRANSFER_H_
#define _PDU_SESSION_RESOURCE_MODIFY_INDICATION_TRANSFER_H_

#include "NgapIesStruct.hpp"
#include "QosFlowPerTnlInformation.hpp"
#include "QosFlowPerTnlInformationList.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceModifyIndicationTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace oai::ngap {
class PduSessionResourceModifyIndicationTransfer {
 public:
  PduSessionResourceModifyIndicationTransfer();
  virtual ~PduSessionResourceModifyIndicationTransfer();

  void setDlQosFlowPerTnlInformation(
      const QosFlowPerTnlInformation& dlQosFlowPerTnlInformation);
  void getDlQosFlowPerTnlInformation(
      QosFlowPerTnlInformation& dlQosFlowPerTnlInformation) const;

  void setAdditionalDlQosFlowPerTnlInformation(
      const std::vector<QosFlowPerTnlInformationItem>& list);
  void setAdditionalDlQosFlowPerTnlInformation(
      const QosFlowPerTnlInformationList& list);
  void getAdditionalDlQosFlowPerTnlInformation(
      std::optional<QosFlowPerTnlInformationList>& list) const;

  int encode(uint8_t* buf, int bufSize);
  bool decode(uint8_t* buf, int bufSize);

 private:
  Ngap_PDUSessionResourceModifyIndicationTransfer_t* m_Ie;

  // DL QoS Flow per TNL Information (Mandatory)
  QosFlowPerTnlInformation m_DlQosFlowPerTnlInformation;
  // Additional DL QoS Flow per TNL Information (Optional)
  std::optional<QosFlowPerTnlInformationList>
      m_AdditionalDlQosFlowPerTnlInformation;

  // Secondary RAT Usage Information (Optional)
  // Security Result (Optional)
  //  Redundant DL QoS Flow per TNL Information (Optional)
  // Global RAN Node ID of Secondary NG-RAN Node (Optional)
};
}  // namespace oai::ngap

#endif
