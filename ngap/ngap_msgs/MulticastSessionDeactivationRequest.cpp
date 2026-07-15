/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MulticastSessionDeactivationRequest.hpp"

#include "logger_base.hpp"
#include "utils.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
MulticastSessionDeactivationRequestMsg::MulticastSessionDeactivationRequestMsg()
    : NgapMessage() {
  m_MulticastSessionDeactivationRequestIes = nullptr;

  NgapMessage::setMessageType(
      NgapMessageType::MULTICAST_SESSION_DEACTIVATION_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
void MulticastSessionDeactivationRequestMsg::initialize() {
  m_MulticastSessionDeactivationRequestIes =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .MulticastSessionDeactivationRequest);
}

//------------------------------------------------------------------------------
void MulticastSessionDeactivationRequestMsg::setMbsSessionId(
    const MbsSessionId& v) {
  m_MbsSessionId = v;

  Ngap_MulticastSessionDeactivationRequestIEs_t* ie =
      (Ngap_MulticastSessionDeactivationRequestIEs_t*) calloc(
          1, sizeof(Ngap_MulticastSessionDeactivationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MBS_SessionID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_MulticastSessionDeactivationRequestIEs__value_PR_MBS_SessionID;

  if (!m_MbsSessionId.encode(ie->value.choice.MBS_SessionID)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_MulticastSessionDeactivationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
}

//------------------------------------------------------------------------------
bool MulticastSessionDeactivationRequestMsg::getMbsSessionId(
    MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
void MulticastSessionDeactivationRequestMsg::
    setMulticastSessionDeactivationRequestTransfer(
        const MulticastSessionDeactivationRequestTransfer& v) {
  uint8_t buf[512] = {};
  int len          = v.encode(buf, sizeof(buf));
  if (len <= 0) {
    oai::logger::logger_common::ngap().error(
        "Encode MulticastSessionDeactivationRequestTransfer error");
    return;
  }

  Ngap_MulticastSessionDeactivationRequestIEs_t* ie =
      (Ngap_MulticastSessionDeactivationRequestIEs_t*) calloc(
          1, sizeof(Ngap_MulticastSessionDeactivationRequestIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_MulticastSessionDeactivationRequestTransfer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_MulticastSessionDeactivationRequestIEs__value_PR_OCTET_STRING_CONTAINING_MulticastSessionDeactivationRequestTransfer_;

  OCTET_STRING_fromBuf(
      &ie->value.choice
           .OCTET_STRING_CONTAINING_MulticastSessionDeactivationRequestTransfer_,
      (const char*) buf, len);

  int ret = ASN_SEQUENCE_ADD(
      &m_MulticastSessionDeactivationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MulticastSessionDeactivationRequestTransfer IE error");
}

//------------------------------------------------------------------------------
bool MulticastSessionDeactivationRequestMsg::decode(
    Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage &&
      ngapPdu->choice.initiatingMessage &&
      ngapPdu->choice.initiatingMessage->procedureCode ==
          Ngap_ProcedureCode_id_MulticastSessionDeactivation &&
      ngapPdu->choice.initiatingMessage->value.present ==
          Ngap_InitiatingMessage__value_PR_MulticastSessionDeactivationRequest) {
    m_MulticastSessionDeactivationRequestIes =
        &ngapPdu->choice.initiatingMessage->value.choice
             .MulticastSessionDeactivationRequest;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check MulticastSessionDeactivationRequest message error");
    return false;
  }

  for (int i = 0;
       i < m_MulticastSessionDeactivationRequestIes->protocolIEs->list.count;
       i++) {
    Ngap_MulticastSessionDeactivationRequestIEs_t* ie =
        (Ngap_MulticastSessionDeactivationRequestIEs_t*)
            m_MulticastSessionDeactivationRequestIes->protocolIEs->list
                .array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_MulticastSessionDeactivationRequestIEs__value_PR_MBS_SessionID) {
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
      case Ngap_ProtocolIE_ID_id_MulticastSessionDeactivationRequestTransfer: {
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
