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

#ifndef _PDU_SESSION_RESOURCE_MODIFY_RESPONSE_TRANSFER_H_
#define _PDU_SESSION_RESOURCE_MODIFY_RESPONSE_TRANSFER_H_

#include <memory>
#include <vector>

#include "NgapIesStruct.hpp"
#include "QosFlowAddOrModifyResponseList.hpp"
#include "QosFlowListWithCause.hpp"
#include "UpTransportLayerInformation.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceModifyResponseTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace oai::ngap {

class PduSessionResourceModifyResponseTransfer {
 public:
  PduSessionResourceModifyResponseTransfer();
  virtual ~PduSessionResourceModifyResponseTransfer(){};

  void setDlNgUUpTnlInformation(
      const UpTransportLayerInformation& dlNgUUpTnlInformation);
  void getDlNgUUpTnlInformation(
      std::optional<UpTransportLayerInformation>& dlNgUUpTnlInformation) const;

  void setUlNgUUpTnlInformation(
      const UpTransportLayerInformation& ulNgUUpTnlInformation);
  void getUlNgUUpTnlInformation(
      std::optional<UpTransportLayerInformation>& ulNgUUpTnlInformation) const;

  void setQosFlowAddOrModifyResponseList(
      const std::vector<QosFlowAddOrModifyResponseItem> list);
  void setQosFlowAddOrModifyResponseList(
      const QosFlowAddOrModifyResponseList& list);
  void getQosFlowAddOrModifyRequestList(
      std::optional<QosFlowAddOrModifyResponseList>& list) const;

  void setQosFlowFailedToAddOrModifyList(
      const QosFlowListWithCause& qosFlowFailedToAddOrModifyList);
  void getQosFlowFailedToAddOrModifyList(
      std::optional<QosFlowListWithCause>& qosFlowFailedToAddOrModifyList)
      const;

  int encode(uint8_t* buf, int bufSize);
  bool decode(uint8_t* buf, int bufSize);

 private:
  Ngap_PDUSessionResourceModifyResponseTransfer_t* m_Ie;

  // DL NG-U UP TNL Information (Optional)
  std::optional<UpTransportLayerInformation> m_DlNgUUpTnlInformation;
  // UL NG-U UP TNL Information (Optional)
  std::optional<UpTransportLayerInformation> m_UlNgUUpTnlInformation;

  // QoS Flow Add or Modify Response List (Optional 0..)
  std::optional<QosFlowAddOrModifyResponseList>
      m_QosFlowAddOrModifyResponseList;
  // TODO: Additional DL QoS Flow per TNL Information
  // QoS Flow Failed to Add or Modify List
  std::optional<QosFlowListWithCause> m_QosFlowFailedToAddOrModifyList;

  // Additional NG-U UP TNL Information
  // Redundant DL NG-U UP TNL Information
  // Redundant UL NG-U UP TNL Information
  // Additional Redundant DL QoS Flow per TNL Information
  // Additional Redundant NG-U UP TNL Information
  // Secondary RAT Usage Information
};

}  // namespace oai::ngap
#endif
