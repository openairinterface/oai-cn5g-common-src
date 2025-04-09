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

#ifndef _PDU_SESSION_RESOURCE_MODIFY_CONFIRM_TRANSFER_H_
#define _PDU_SESSION_RESOURCE_MODIFY_CONFIRM_TRANSFER_H_

#include <memory>
#include <vector>

#include "NgapIesStruct.hpp"
#include "QosFlowListWithCause.hpp"
#include "QosFlowModifyConfirmList.hpp"
#include "UpTransportLayerInformation.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceModifyConfirmTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace oai::ngap {

class PduSessionResourceModifyConfirmTransfer {
 public:
  PduSessionResourceModifyConfirmTransfer();
  virtual ~PduSessionResourceModifyConfirmTransfer(){};

  void setQosFlowModifyConfirmList(
      const std::vector<QosFlowModifyConfirmItem> list);
  void setQosFlowModifyConfirmList(const QosFlowModifyConfirmList& list);
  void getQosFlowModifyConfirmList(QosFlowModifyConfirmList& list) const;

  void setUlNgUUpTnlInformation(
      const UpTransportLayerInformation& ulNgUUpTnlInformation);
  void getUlNgUUpTnlInformation(
      UpTransportLayerInformation& ulNgUUpTnlInformation) const;

  void setQosFlowFailedToModifyList(
      const QosFlowListWithCause& qosFlowFailedToModifyList);
  void getQosFlowFailedToModifyList(
      std::optional<QosFlowListWithCause>& qosFlowFailedToModifyList) const;

  int encode(uint8_t* buf, int bufSize);
  bool decode(uint8_t* buf, int bufSize);

 private:
  Ngap_PDUSessionResourceModifyConfirmTransfer_t* m_Ie;

  // QoS Flow Modify Confirm List (Mandatory)
  QosFlowModifyConfirmList m_QosFlowModifyConfirmList;
  // UL NG-U UP TNL Information (Mandatory)
  UpTransportLayerInformation m_UlNgUUpTnlInformation;
  // TODO: Additional NG-U UP TNL Information (Optional)
  // QoS Flow Failed to Modify List (Optional)
  std::optional<QosFlowListWithCause> m_QosFlowFailedToModifyList;
  // Redundant UL NG-U UP TNL Information (Optional)
  // Additional Redundant NG-U UP TNL Information (Optional)
};

}  // namespace oai::ngap
#endif
