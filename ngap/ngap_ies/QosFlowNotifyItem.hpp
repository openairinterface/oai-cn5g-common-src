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

#ifndef _QOS_FLOW_NOTIFY_ITEM_H_
#define _QOS_FLOW_NOTIFY_ITEM_H_

#include "NotificationCause.hpp"
#include "QosFlowIdentifier.hpp"

extern "C" {
#include "Ngap_QosFlowNotifyItem.h"
}

namespace oai::ngap {

class QosFlowNotifyItem {
 public:
  QosFlowNotifyItem();
  virtual ~QosFlowNotifyItem();

  void setQosFlowIdentifier(const QosFlowIdentifier& qosFlowIdentifier);
  void getQosFlowIdentifier(QosFlowIdentifier& qosFlowIdentifier) const;

  void setNotificationCause(const NotificationCause& notificationCause);
  void getNotificationCause(NotificationCause& notificationCause) const;

  /*
  void setCurrentQoSParametersSetIndex(uint32_t&
          currentQoSParametersSetIndex);
  void getCurrentQoSParametersSetIndex(std::optional<uint32_t>&
                  currentQoSParametersSetIndex) const;
*/
  bool encode(Ngap_QosFlowNotifyItem_t&) const;
  bool decode(const Ngap_QosFlowNotifyItem_t&);

 private:
  QosFlowIdentifier m_QosFlowIdentifier;  // Mandatory
  NotificationCause m_NotificationCause;
  // std::optional<uint32_t>
  //     m_CurrentQoSParametersSetIndex;  // Optional
};

}  // namespace oai::ngap

#endif
