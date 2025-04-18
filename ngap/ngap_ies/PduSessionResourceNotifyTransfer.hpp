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

#ifndef _PDU_SESSION_RESOURCE_NOTIFY_TRANSFER_H_
#define _PDU_SESSION_RESOURCE_NOTIFY_TRANSFER_H_

#include <memory>
#include <optional>
#include <vector>

#include "QosFlowFeedbackList.hpp"
#include "QosFlowListWithCause.hpp"
#include "QosFlowNotifyList.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceNotifyTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace oai::ngap {

class PduSessionResourceNotifyTransfer {
 public:
  PduSessionResourceNotifyTransfer();
  virtual ~PduSessionResourceNotifyTransfer(){};

  void setQosFlowNotifyList(const std::vector<QosFlowNotifyItem> list);
  void setQosFlowNotifyList(const QosFlowNotifyList& list);
  void getQosFlowNotifyList(std::optional<QosFlowNotifyList>& list) const;

  void setQosFlowReleasedList(const std::vector<QosFlowWithCauseItem> list);
  void setQosFlowReleasedList(const QosFlowListWithCause& list);
  void getQosFlowReleasedList(std::optional<QosFlowListWithCause>& list) const;

  void setQosFlowFeedbackList(const std::vector<QosFlowFeedbackItem> list);
  void setQosFlowFeedbackList(const QosFlowFeedbackList& list);
  void getQosFlowFeedbackList(std::optional<QosFlowFeedbackList>& list) const;

  int encode(uint8_t* buf, int bufSize);
  bool decode(uint8_t* buf, int bufSize);

 private:
  Ngap_PDUSessionResourceNotifyTransfer_t* m_Ie;

  // QoS Flow Notify List (Optional 0..)
  std::optional<QosFlowNotifyList> m_QosFlowNotifyList;
  // QoS Flow Released List (Optional)
  std::optional<QosFlowListWithCause> m_QosFlowReleasedList;
  // TODO: Secondary RAT Usage Information
  // QoS Flow Feedback List
  std::optional<QosFlowFeedbackList> m_QosFlowFeedbackList;
};

}  // namespace oai::ngap
#endif
