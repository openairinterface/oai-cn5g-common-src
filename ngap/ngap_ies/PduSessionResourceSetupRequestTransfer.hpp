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

#ifndef _PDU_SESSION_RESOURCE_SETUP_REQUEST_TRANSFER_H_
#define _PDU_SESSION_RESOURCE_SETUP_REQUEST_TRANSFER_H_

#include <vector>

#include "DataForwardingNotPossible.hpp"
#include "NetworkInstance.hpp"
#include "NgapIesStruct.hpp"
#include "PduSessionAggregateMaximumBitRate.hpp"
#include "PduSessionType.hpp"
#include "QosFlowSetupRequestList.hpp"
#include "SecurityIndication.hpp"
#include "UpTransportLayerInformation.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceSetupRequestTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace oai::ngap {

class PduSessionResourceSetupRequestTransferIE {
 public:
  PduSessionResourceSetupRequestTransferIE();
  virtual ~PduSessionResourceSetupRequestTransferIE(){};

  void setPduSessionAggregateMaximumBitRate(
      const long& bitRateDl, const long& bitRateUl);
  void setPduSessionAggregateMaximumBitRate(
      const PduSessionAggregateMaximumBitRate& maxBitRate);
  void getPduSessionAggregateMaximumBitRate(
      std::optional<PduSessionAggregateMaximumBitRate>& maxBitRate) const;

  void setUlNgUUpTnlInformation(const GtpTunnel_t& upTnlInfo);
  void setUlNgUUpTnlInformation(const UpTransportLayerInformation& upTnlInfo);
  bool getUlNgUUpTnlInformation(GtpTunnel_t& upTnlInfo) const;
  // void
  // setAdditionalUlNgUUpTnlInformation(std::vector<GtpTunnel>list);
  // bool
  // getAdditionalUlNgUUpTnlInformation(std::vector<GtpTunnel>&list);

  void setDataForwardingNotPossible();
  bool getDataForwardingNotPossible() const;

  void setPduSessionType(e_Ngap_PDUSessionType type);
  bool getPduSessionType(long& type) const;

  void setSecurityIndication(
      e_Ngap_IntegrityProtectionIndication integrityProtectionIndication,
      e_Ngap_ConfidentialityProtectionIndication
          confidentialityProtectionIndication,
      e_Ngap_MaximumIntegrityProtectedDataRate maxIntProtDataRate);
  void setSecurityIndication(
      e_Ngap_IntegrityProtectionIndication integrityProtectionIndication,
      e_Ngap_ConfidentialityProtectionIndication
          confidentialityProtectionIndication);
  bool getSecurityIndication(
      long& integrityProtectionIndication,
      long& confidentialityProtectionIndication,
      long& maxIntProtDataRate) const;
  void getSecurityIndication(
      std::optional<SecurityIndication>& securityIndication) const;

  void setNetworkInstance(const long& value);
  bool getNetworkInstance(long& value) const;
  void getNetworkInstance(
      std::optional<NetworkInstance>& networkInstance) const;

  void setQosFlowSetupRequestList(std::vector<QosFlowSetupReq_t> list);
  bool getQosFlowSetupRequestList(std::vector<QosFlowSetupReq_t>& list) const;
  void setQosFlowSetupRequestList(const QosFlowSetupRequestList& list);
  void getQosFlowSetupRequestList(QosFlowSetupRequestList& list) const;

  int encode(uint8_t* buf, int buf_size);   // TODO: remove naked pointer
  bool decode(uint8_t* buf, int buf_size);  // TODO: remove naked pointer

 private:
  Ngap_PDUSessionResourceSetupRequestTransfer_t* m_Ie;

  // PDU Session Aggregate Maximum Bit Rate (optional)
  std::optional<PduSessionAggregateMaximumBitRate>
      m_PduSessionAggregateMaximumBitRateIe;
  // UL NG-U UP TNL Information (Mandatory)
  UpTransportLayerInformation m_UpTransportLayerInformation;
  // Additional UL NG-U UP TNL Information (Optional)
  std::vector<UpTransportLayerInformation> m_UpTransportLayerInformationList;
  // Data Forwarding Not Possible (Optional)
  std::optional<DataForwardingNotPossible> m_DataForwardingNotPossible;
  // PDU Session Type (Mandatory)
  PduSessionType m_PduSessionType;
  // Security Indication (Optional)
  std::optional<SecurityIndication> m_SecurityIndication;
  // Network Instance (Optional)
  std::optional<NetworkInstance> m_NetworkInstance;
  // QoS Flow Setup Request List (Mandatory 1..)
  QosFlowSetupRequestList m_QosFlowSetupRequestList;
  // TODO: Common Network Instance
  // TODO: Direct Forwarding Path Availability

  void addUlNgUUpTnlInformation();
  void addPduSessionAggregateMaximumBitRate();
  void addSecurityIndication();
  void addQosFlowSetupRequestList();
};

}  // namespace oai::ngap
#endif
