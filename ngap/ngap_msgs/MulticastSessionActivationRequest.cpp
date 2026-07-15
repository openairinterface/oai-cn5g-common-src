/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MulticastSessionActivationRequest.hpp"

#include "logger_base.hpp"
#include "utils.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
MulticastSessionActivationRequestMsg::MulticastSessionActivationRequestMsg()
    : NgapMessage() {
  m_MulticastSessionActivationRequestIes = nullptr;

  NgapMessage::setMessageType(
      NgapMessageType::MULTICAST_SESSION_ACTIVATION_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
void MulticastSessionActivationRequestMsg::initialize() {
  m_MulticastSessionActivationRequestIes =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .MulticastSessionActivationRequest);
}

//------------------------------------------------------------------------------
void MulticastSessionActivationRequestMsg::setMbsSessionId(
    const MbsSessionId& v) {
  m_MbsSessionId = v;

  Ngap_MulticastSessionActivationRequestIEs_t* ie =
      (Ngap_MulticastSessionActivationRequestIEs_t*) calloc(
          1, sizeof(Ngap_MulticastSessionActivationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MBS_SessionID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_MulticastSessionActivationRequestIEs__value_PR_MBS_SessionID;

  if (!m_MbsSessionId.encode(ie->value.choice.MBS_SessionID)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_MulticastSessionActivationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
}

//------------------------------------------------------------------------------
bool MulticastSessionActivationRequestMsg::getMbsSessionId(
    MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
void MulticastSessionActivationRequestMsg::
    setMulticastSessionActivationRequestTransfer(
        const MulticastSessionActivationRequestTransfer& v) {
  uint8_t buf[512] = {};
  int len          = v.encode(buf, sizeof(buf));
  if (len <= 0) {
    oai::logger::logger_common::ngap().error(
        "Encode MulticastSessionActivationRequestTransfer error");
    return;
  }

  Ngap_MulticastSessionActivationRequestIEs_t* ie =
      (Ngap_MulticastSessionActivationRequestIEs_t*) calloc(
          1, sizeof(Ngap_MulticastSessionActivationRequestIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_MulticastSessionActivationRequestTransfer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_MulticastSessionActivationRequestIEs__value_PR_OCTET_STRING_CONTAINING_MulticastSessionActivationRequestTransfer_;

  OCTET_STRING_fromBuf(
      &ie->value.choice
           .OCTET_STRING_CONTAINING_MulticastSessionActivationRequestTransfer_,
      (const char*) buf, len);

  int ret = ASN_SEQUENCE_ADD(
      &m_MulticastSessionActivationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MulticastSessionActivationRequestTransfer IE error");
}

//------------------------------------------------------------------------------
bool MulticastSessionActivationRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage &&
      ngapPdu->choice.initiatingMessage &&
      ngapPdu->choice.initiatingMessage->procedureCode ==
          Ngap_ProcedureCode_id_MulticastSessionActivation &&
      ngapPdu->choice.initiatingMessage->value.present ==
          Ngap_InitiatingMessage__value_PR_MulticastSessionActivationRequest) {
    m_MulticastSessionActivationRequestIes =
        &ngapPdu->choice.initiatingMessage->value.choice
             .MulticastSessionActivationRequest;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check MulticastSessionActivationRequest message error");
    return false;
  }

  for (int i = 0;
       i < m_MulticastSessionActivationRequestIes->protocolIEs->list.count;
       i++) {
    Ngap_MulticastSessionActivationRequestIEs_t* ie =
        (Ngap_MulticastSessionActivationRequestIEs_t*)
            m_MulticastSessionActivationRequestIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_MulticastSessionActivationRequestIEs__value_PR_MBS_SessionID) {
          if (!m_MbsSessionId.decode(ie->value.choice.MBS_SessionID)) {
            oai::logger::logger_common::ngap().error(
                "Decode NGAP MBS-SessionID IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decode NGAP MBS-SessionID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_MulticastSessionActivationRequestTransfer: {
        // Transfer as OCTET_STRING; decoded by application layer.
      } break;
      default: {
        break;
      }
    }
  }
  return true;
}

}  // namespace oai::ngap
