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

#ifndef _PATH_SWITCH_REQUEST_ACKNOWLEDGE_TRANSFER_H_
#define _PATH_SWITCH_REQUEST_ACKNOWLEDGE_TRANSFER_H_

#include <memory>
#include <vector>

#include "NgapIesStruct.hpp"
#include "QosFlowParametersList.hpp"
#include "UpTransportLayerInformation.hpp"

extern "C" {
#include "Ngap_PathSwitchRequestAcknowledgeTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace oai::ngap {

class PathSwitchRequestAcknowledgeTransfer {
 public:
  PathSwitchRequestAcknowledgeTransfer();
  virtual ~PathSwitchRequestAcknowledgeTransfer(){};

  void setUlNgUUpTnlInformation(
      const UpTransportLayerInformation& ulNgUUpTnlInformation);
  void getUlNgUUpTnlInformation(
      std::optional<UpTransportLayerInformation>& ulNgUUpTnlInformation) const;

  void setQosFlowParametersList(const std::vector<QosFlowParametersItem> list);
  void setQosFlowParametersList(const QosFlowParametersList& list);
  void getQosFlowParametersList(
      std::optional<QosFlowParametersList>& list) const;

  int encode(uint8_t* buf, int bufSize);
  bool decode(uint8_t* buf, int bufSize);

 private:
  Ngap_PathSwitchRequestAcknowledgeTransfer_t* m_Ie;

  // UL NG-U UP TNL Information (Optional)
  std::optional<UpTransportLayerInformation> m_UlNgUUpTnlInformation;

  // TODO: Security Indication
  // TODO: Additional NG-U UP TNL Information
  // TODO: Redundant UL NG-U UP TNL Information
  // TODO: Additional Redundant NG-U UP TNL Information

  // QoS Flow Parameters List (Optional 0..1)
  std::optional<QosFlowParametersList> m_QosFlowParametersList;
};

}  // namespace oai::ngap
#endif
