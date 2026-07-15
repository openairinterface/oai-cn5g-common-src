/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "BroadcastSessionReleaseResponse.hpp"

#include "logger_base.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
BroadcastSessionReleaseResponseMsg::BroadcastSessionReleaseResponseMsg()
    : NgapMessage() {
  m_BroadcastSessionReleaseResponseIes = nullptr;
  m_CriticalityDiag                    = std::nullopt;

  NgapMessage::setMessageType(
      NgapMessageType::BROADCAST_SESSION_RELEASE_RESPONSE);
  initialize();
}

//------------------------------------------------------------------------------
void BroadcastSessionReleaseResponseMsg::initialize() {
  m_BroadcastSessionReleaseResponseIes =
      &(ngapPdu->choice.successfulOutcome->value.choice
            .BroadcastSessionReleaseResponse);
}

//------------------------------------------------------------------------------
bool BroadcastSessionReleaseResponseMsg::getMbsSessionId(
    MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
bool BroadcastSessionReleaseResponseMsg::getCriticalityDiagnostics(
    CriticalityDiagnostics& v) const {
  if (!m_CriticalityDiag.has_value()) return false;
  v = m_CriticalityDiag.value();
  return true;
}

//------------------------------------------------------------------------------
bool BroadcastSessionReleaseResponseMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome &&
      ngapPdu->choice.successfulOutcome &&
      ngapPdu->choice.successfulOutcome->procedureCode ==
          Ngap_ProcedureCode_id_BroadcastSessionRelease &&
      ngapPdu->choice.successfulOutcome->value.present ==
          Ngap_SuccessfulOutcome__value_PR_BroadcastSessionReleaseResponse) {
    m_BroadcastSessionReleaseResponseIes =
        &ngapPdu->choice.successfulOutcome->value.choice
             .BroadcastSessionReleaseResponse;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check BroadcastSessionReleaseResponse message error");
    return false;
  }

  for (int i = 0;
       i < m_BroadcastSessionReleaseResponseIes->protocolIEs->list.count; i++) {
    Ngap_BroadcastSessionReleaseResponseIEs_t* ie =
        (Ngap_BroadcastSessionReleaseResponseIEs_t*)
            m_BroadcastSessionReleaseResponseIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_BroadcastSessionReleaseResponseIEs__value_PR_MBS_SessionID) {
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
      case Ngap_ProtocolIE_ID_id_MBSSessionReleaseResponseTransfer: {
        // Transfer as OCTET_STRING; decoded by application layer.
      } break;
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        if (ie->criticality == Ngap_Criticality_ignore &&
            ie->value.present ==
                Ngap_BroadcastSessionReleaseResponseIEs__value_PR_CriticalityDiagnostics) {
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
