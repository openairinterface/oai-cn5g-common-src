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

#ifndef _PATH_SWITCH_REQUEST_TRANSFER_H_
#define _PATH_SWITCH_REQUEST_TRANSFER_H_

#include <memory>
#include <vector>

#include "NgapIesStruct.hpp"
#include "QosFlowAcceptedList.hpp"
#include "QosFlowListWithCause.hpp"
#include "UpTransportLayerInformation.hpp"

extern "C" {
#include "Ngap_PathSwitchRequestTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace oai::ngap {

class PathSwitchRequestTransfer {
 public:
  PathSwitchRequestTransfer();
  virtual ~PathSwitchRequestTransfer(){};

  void setDlNgUUpTnlInformation(
      const UpTransportLayerInformation& dlNgUUpTnlInformation);
  void getDlNgUUpTnlInformation(
      UpTransportLayerInformation& dlNgUUpTnlInformation) const;

  void setQosFlowAcceptedList(const std::vector<QosFlowAcceptedItem> list);
  void setQosFlowAcceptedList(const QosFlowAcceptedList& list);
  void getQosFlowAcceptedList(QosFlowAcceptedList& list) const;

  int encode(uint8_t* buf, int bufSize);
  bool decode(uint8_t* buf, int bufSize);

 private:
  Ngap_PathSwitchRequestTransfer_t* m_Ie;

  // DL NG-U UP TNL Information (Mandatory)
  UpTransportLayerInformation m_DlNgUUpTnlInformation;
  // TODO: DL NG-U TNL Information Reused (Optional)
  // TODO: User Plane Security Information (Optional)

  // QoS Flow Accepted List (Mandatory)
  QosFlowAcceptedList m_QosFlowAcceptedList;

  // TODO: Additional DL QoS Flow per TNL Information
  // TODO: Redundant DL NG-U UP TNL Information
  // TODO: Redundant DL NG-U UP TNL Information Reused
  // TODO: Additional Redundant DL QoS Flow per TNL Information
  // TODO: Used RSN Information
  // TODO: Global RAN Node ID of Secondary NG-RAN Node
};

}  // namespace oai::ngap
#endif
