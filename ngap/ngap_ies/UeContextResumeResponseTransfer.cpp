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

#include "UeContextResumeResponseTransfer.hpp"

#include "logger_base.hpp"
#include "ngap_utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
UeContextResumeResponseTransfer::UeContextResumeResponseTransfer() {
  m_UeContextResumeResponseTransferIe =
      (Ngap_UEContextResumeResponseTransfer_t*) calloc(
          1, sizeof(Ngap_UEContextResumeResponseTransfer_t));
}

//------------------------------------------------------------------------------
UeContextResumeResponseTransfer::~UeContextResumeResponseTransfer() {}

//------------------------------------------------------------------------------
void UeContextResumeResponseTransfer::setQosFlowFailedToResumeList(
    const std::vector<QosFlowWithCauseItem> list) {
  QosFlowListWithCause qosFlowFailedToResumeList = {};
  qosFlowFailedToResumeList.set(list);
  m_QosFlowFailedToResumeList =
      std::make_optional<QosFlowListWithCause>(qosFlowFailedToResumeList);
}

//------------------------------------------------------------------------------
void UeContextResumeResponseTransfer::setQosFlowFailedToResumeList(
    const QosFlowListWithCause& list) {
  m_QosFlowFailedToResumeList = std::make_optional<QosFlowListWithCause>(list);
}

//------------------------------------------------------------------------------
void UeContextResumeResponseTransfer::getQosFlowFailedToResumeList(
    std::optional<QosFlowListWithCause>& list) const {
  list = m_QosFlowFailedToResumeList;
}

//------------------------------------------------------------------------------
int UeContextResumeResponseTransfer::encode(uint8_t* buf, int bufSize) {
  ngap_utils::print_asn_msg(
      &asn_DEF_Ngap_UEContextResumeResponseTransfer,
      m_UeContextResumeResponseTransferIe);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_UEContextResumeResponseTransfer, NULL,
      m_UeContextResumeResponseTransferIe, buf, bufSize);
  oai::logger::logger_common::ngap().debug("er.encoded %d", er.encoded);
  return er.encoded;
}

//------------------------------------------------------------------------------
bool UeContextResumeResponseTransfer::decode(uint8_t* buf, int bufSize) {
  asn_dec_rval_t rc = asn_decode(
      NULL, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_UEContextResumeResponseTransfer,
      (void**) &m_UeContextResumeResponseTransferIe, buf, bufSize);
  if (rc.code == RC_OK) {
    oai::logger::logger_common::ngap().debug(
        "Decoded handoverRequestAcknowledegTransfer successfully");
  } else if (rc.code == RC_WMORE) {
    oai::logger::logger_common::ngap().debug("More data expected, call again");
    return false;
  } else {
    oai::logger::logger_common::ngap().debug(
        "Failure to decode handoverRequestAcknowledegTransfer data");
    // return false;
  }
  oai::logger::logger_common::ngap().debug(
      "rc.consumed to decode: %d", rc.consumed);

  if (m_UeContextResumeResponseTransferIe->qosFlowFailedToResumeList) {
    QosFlowListWithCause qosFlowFailedToResumeList = {};

    if (!qosFlowFailedToResumeList.decode(
            *m_UeContextResumeResponseTransferIe->qosFlowFailedToResumeList)) {
      oai::logger::logger_common::ngap().error(
          "Decode NGAP  QoS Flow Failed to Resume List IE error");
      return false;
    }
    m_QosFlowFailedToResumeList =
        std::make_optional<QosFlowListWithCause>(qosFlowFailedToResumeList);
  }

  return true;
}

}  // namespace oai::ngap
