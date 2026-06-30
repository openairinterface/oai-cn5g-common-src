/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MulticastSessionActivationResponse.hpp"

#include "logger_base.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
MulticastSessionActivationResponseMsg::MulticastSessionActivationResponseMsg()
    : NgapMessage() {
  m_MulticastSessionActivationResponseIes = nullptr;
  m_CriticalityDiag                       = std::nullopt;

  NgapMessage::setMessageType(
      NgapMessageType::MULTICAST_SESSION_ACTIVATION_RESPONSE);
  initialize();
}

//------------------------------------------------------------------------------
void MulticastSessionActivationResponseMsg::initialize() {
  m_MulticastSessionActivationResponseIes =
      &(ngapPdu->choice.successfulOutcome->value.choice
            .MulticastSessionActivationResponse);
}

//------------------------------------------------------------------------------
bool MulticastSessionActivationResponseMsg::getMbsSessionId(
    MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
bool MulticastSessionActivationResponseMsg::getCriticalityDiagnostics(
    CriticalityDiagnostics& v) const {
  if (!m_CriticalityDiag.has_value()) return false;
  v = m_CriticalityDiag.value();
  return true;
}

//------------------------------------------------------------------------------
bool MulticastSessionActivationResponseMsg::decode(
    Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome &&
      ngapPdu->choice.successfulOutcome &&
      ngapPdu->choice.successfulOutcome->procedureCode ==
          Ngap_ProcedureCode_id_MulticastSessionActivation &&
      ngapPdu->choice.successfulOutcome->value.present ==
          Ngap_SuccessfulOutcome__value_PR_MulticastSessionActivationResponse) {
    m_MulticastSessionActivationResponseIes =
        &ngapPdu->choice.successfulOutcome->value.choice
             .MulticastSessionActivationResponse;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check MulticastSessionActivationResponse message error");
    return false;
  }

  for (int i = 0;
       i < m_MulticastSessionActivationResponseIes->protocolIEs->list.count;
       i++) {
    Ngap_MulticastSessionActivationResponseIEs_t* ie =
        (Ngap_MulticastSessionActivationResponseIEs_t*)
            m_MulticastSessionActivationResponseIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_MulticastSessionActivationResponseIEs__value_PR_MBS_SessionID) {
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
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        if (ie->criticality == Ngap_Criticality_ignore &&
            ie->value.present ==
                Ngap_MulticastSessionActivationResponseIEs__value_PR_CriticalityDiagnostics) {
          m_CriticalityDiag = std::make_optional<CriticalityDiagnostics>();
          if (!m_CriticalityDiag.value().decode(
                  ie->value.choice.CriticalityDiagnostics)) {
            oai::logger::logger_common::ngap().error(
                "Decode NGAP CriticalityDiagnostics IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decode NGAP CriticalityDiagnostics IE error");
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
