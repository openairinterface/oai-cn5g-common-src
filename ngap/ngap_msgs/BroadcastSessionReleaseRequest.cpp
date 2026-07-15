/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "BroadcastSessionReleaseRequest.hpp"

#include "logger_base.hpp"
#include "utils.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
BroadcastSessionReleaseRequestMsg::BroadcastSessionReleaseRequestMsg()
    : NgapMessage() {
  m_BroadcastSessionReleaseRequestIes = nullptr;

  NgapMessage::setMessageType(
      NgapMessageType::BROADCAST_SESSION_RELEASE_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
void BroadcastSessionReleaseRequestMsg::initialize() {
  m_BroadcastSessionReleaseRequestIes =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .BroadcastSessionReleaseRequest);
}

//------------------------------------------------------------------------------
void BroadcastSessionReleaseRequestMsg::setMbsSessionId(const MbsSessionId& v) {
  m_MbsSessionId = v;

  Ngap_BroadcastSessionReleaseRequestIEs_t* ie =
      (Ngap_BroadcastSessionReleaseRequestIEs_t*) calloc(
          1, sizeof(Ngap_BroadcastSessionReleaseRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MBS_SessionID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_BroadcastSessionReleaseRequestIEs__value_PR_MBS_SessionID;

  if (!m_MbsSessionId.encode(ie->value.choice.MBS_SessionID)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_BroadcastSessionReleaseRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
}

//------------------------------------------------------------------------------
bool BroadcastSessionReleaseRequestMsg::getMbsSessionId(MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
void BroadcastSessionReleaseRequestMsg::setCause(const Cause& v) {
  m_Cause = v;

  Ngap_BroadcastSessionReleaseRequestIEs_t* ie =
      (Ngap_BroadcastSessionReleaseRequestIEs_t*) calloc(
          1, sizeof(Ngap_BroadcastSessionReleaseRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_BroadcastSessionReleaseRequestIEs__value_PR_Cause;

  if (!m_Cause.encode(ie->value.choice.Cause)) {
    oai::logger::logger_common::ngap().error("Encode NGAP Cause IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_BroadcastSessionReleaseRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode NGAP Cause IE error");
}

//------------------------------------------------------------------------------
bool BroadcastSessionReleaseRequestMsg::getCause(Cause& v) const {
  v = m_Cause;
  return true;
}

//------------------------------------------------------------------------------
bool BroadcastSessionReleaseRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage &&
      ngapPdu->choice.initiatingMessage &&
      ngapPdu->choice.initiatingMessage->procedureCode ==
          Ngap_ProcedureCode_id_BroadcastSessionRelease &&
      ngapPdu->choice.initiatingMessage->value.present ==
          Ngap_InitiatingMessage__value_PR_BroadcastSessionReleaseRequest) {
    m_BroadcastSessionReleaseRequestIes =
        &ngapPdu->choice.initiatingMessage->value.choice
             .BroadcastSessionReleaseRequest;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check BroadcastSessionReleaseRequest message error");
    return false;
  }

  for (int i = 0;
       i < m_BroadcastSessionReleaseRequestIes->protocolIEs->list.count; i++) {
    Ngap_BroadcastSessionReleaseRequestIEs_t* ie =
        (Ngap_BroadcastSessionReleaseRequestIEs_t*)
            m_BroadcastSessionReleaseRequestIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_BroadcastSessionReleaseRequestIEs__value_PR_MBS_SessionID) {
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
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (ie->criticality == Ngap_Criticality_ignore &&
            ie->value.present ==
                Ngap_BroadcastSessionReleaseRequestIEs__value_PR_Cause) {
          if (!m_Cause.decode(ie->value.choice.Cause)) {
            oai::logger::logger_common::ngap().error(
                "Decode NGAP Cause IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decode NGAP Cause IE error");
          return false;
        }
      } break;
      default: {
        break;
      }
    }
  }
  return true;
}

}  // namespace oai::ngap
