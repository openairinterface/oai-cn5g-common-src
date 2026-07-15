/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "BroadcastSessionModificationFailure.hpp"

#include "logger_base.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
BroadcastSessionModificationFailureMsg::BroadcastSessionModificationFailureMsg()
    : NgapMessage() {
  m_BroadcastSessionModificationFailureIes = nullptr;
  m_CriticalityDiag                        = std::nullopt;

  NgapMessage::setMessageType(
      NgapMessageType::BROADCAST_SESSION_MODIFICATION_FAILURE);
  initialize();
}

//------------------------------------------------------------------------------
void BroadcastSessionModificationFailureMsg::initialize() {
  m_BroadcastSessionModificationFailureIes =
      &(ngapPdu->choice.unsuccessfulOutcome->value.choice
            .BroadcastSessionModificationFailure);
}

//------------------------------------------------------------------------------
bool BroadcastSessionModificationFailureMsg::getMbsSessionId(
    MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
bool BroadcastSessionModificationFailureMsg::getCause(Cause& v) const {
  v = m_Cause;
  return true;
}

//------------------------------------------------------------------------------
bool BroadcastSessionModificationFailureMsg::getCriticalityDiagnostics(
    CriticalityDiagnostics& v) const {
  if (!m_CriticalityDiag.has_value()) return false;
  v = m_CriticalityDiag.value();
  return true;
}

//------------------------------------------------------------------------------
bool BroadcastSessionModificationFailureMsg::decode(
    Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_unsuccessfulOutcome &&
      ngapPdu->choice.unsuccessfulOutcome &&
      ngapPdu->choice.unsuccessfulOutcome->procedureCode ==
          Ngap_ProcedureCode_id_BroadcastSessionModification &&
      ngapPdu->choice.unsuccessfulOutcome->value.present ==
          Ngap_UnsuccessfulOutcome__value_PR_BroadcastSessionModificationFailure) {
    m_BroadcastSessionModificationFailureIes =
        &ngapPdu->choice.unsuccessfulOutcome->value.choice
             .BroadcastSessionModificationFailure;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check BroadcastSessionModificationFailure message error");
    return false;
  }

  for (int i = 0;
       i < m_BroadcastSessionModificationFailureIes->protocolIEs->list.count;
       i++) {
    Ngap_BroadcastSessionModificationFailureIEs_t* ie =
        (Ngap_BroadcastSessionModificationFailureIEs_t*)
            m_BroadcastSessionModificationFailureIes->protocolIEs->list
                .array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_BroadcastSessionModificationFailureIEs__value_PR_MBS_SessionID) {
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
      case Ngap_ProtocolIE_ID_id_MBSSessionModificationFailureTransfer: {
        // Transfer as OCTET_STRING; decoded by application layer.
      } break;
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (ie->criticality == Ngap_Criticality_ignore &&
            ie->value.present ==
                Ngap_BroadcastSessionModificationFailureIEs__value_PR_Cause) {
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
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        if (ie->criticality == Ngap_Criticality_ignore &&
            ie->value.present ==
                Ngap_BroadcastSessionModificationFailureIEs__value_PR_CriticalityDiagnostics) {
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
