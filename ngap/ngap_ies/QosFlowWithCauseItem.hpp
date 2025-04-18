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

#ifndef _QOS_FLOW_ITEM_WITH_CAUSE_H_
#define _QOS_FLOW_ITEM_WITH_CAUSE_H_

#include "QosFlowIdentifier.hpp"
#include "Cause.hpp"

extern "C" {
#include "Ngap_QosFlowWithCauseItem.h"
}

namespace oai::ngap {

class QosFlowWithCauseItem {
 public:
  QosFlowWithCauseItem();
  virtual ~QosFlowWithCauseItem();

  void set(const QosFlowIdentifier& qosFlowIdentifier, const Cause& cause);
  bool get(QosFlowIdentifier& qosFlowIdentifier, Cause& cause) const;

  void setQosFlowIdentifier(const QosFlowIdentifier& qosFlowIdentifier);
  bool getQosFlowIdentifier(QosFlowIdentifier& qosFlowIdentifier) const;

  void setCause(const Cause& cause);
  bool getCause(Cause& cause) const;

  bool encode(Ngap_QosFlowWithCauseItem_t&) const;
  bool decode(const Ngap_QosFlowWithCauseItem_t&);

 private:
  QosFlowIdentifier m_QosFlowIdentifier;  // Mandatory
  Cause m_Cause;                          // Mandatory
};

}  // namespace oai::ngap

#endif
