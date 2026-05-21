/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "BroadcastSessionReleaseRequired.hpp"

#include "logger_base.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
BroadcastSessionReleaseRequiredMsg::BroadcastSessionReleaseRequiredMsg()
    : NgapMessage() {
  m_BroadcastSessionReleaseRequiredIes = nullptr;

  NgapMessage::setMessageType(
      NgapMessageType::BROADCAST_SESSION_RELEASE_REQUIRED);
  initialize();
}

//------------------------------------------------------------------------------
void BroadcastSessionReleaseRequiredMsg::initialize() {
  m_BroadcastSessionReleaseRequiredIes =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .BroadcastSessionReleaseRequired);
}

//------------------------------------------------------------------------------
bool BroadcastSessionReleaseRequiredMsg::getMbsSessionId(
    MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
bool BroadcastSessionReleaseRequiredMsg::getCause(Cause& v) const {
  v = m_Cause;
  return true;
}

//------------------------------------------------------------------------------
bool BroadcastSessionReleaseRequiredMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage &&
      ngapPdu->choice.initiatingMessage &&
      ngapPdu->choice.initiatingMessage->procedureCode ==
          Ngap_ProcedureCode_id_BroadcastSessionReleaseRequired &&
      ngapPdu->choice.initiatingMessage->value.present ==
          Ngap_InitiatingMessage__value_PR_BroadcastSessionReleaseRequired) {
    m_BroadcastSessionReleaseRequiredIes =
        &ngapPdu->choice.initiatingMessage->value.choice
             .BroadcastSessionReleaseRequired;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check BroadcastSessionReleaseRequired message error");
    return false;
  }

  for (int i = 0;
       i < m_BroadcastSessionReleaseRequiredIes->protocolIEs->list.count;
       i++) {
    Ngap_BroadcastSessionReleaseRequiredIEs_t* ie =
        (Ngap_BroadcastSessionReleaseRequiredIEs_t*)
            m_BroadcastSessionReleaseRequiredIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_BroadcastSessionReleaseRequiredIEs__value_PR_MBS_SessionID) {
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
                Ngap_BroadcastSessionReleaseRequiredIEs__value_PR_Cause) {
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
