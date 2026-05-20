/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "BroadcastSessionSetupFailure.hpp"

#include "logger_base.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
BroadcastSessionSetupFailureMsg::BroadcastSessionSetupFailureMsg()
    : NgapMessage() {
  m_BroadcastSessionSetupFailureIes = nullptr;
  m_CriticalityDiag                 = std::nullopt;

  NgapMessage::setMessageType(
      NgapMessageType::BROADCAST_SESSION_SETUP_FAILURE);
  initialize();
}

//------------------------------------------------------------------------------
void BroadcastSessionSetupFailureMsg::initialize() {
  m_BroadcastSessionSetupFailureIes =
      &(ngapPdu->choice.unsuccessfulOutcome->value.choice
            .BroadcastSessionSetupFailure);
}

//------------------------------------------------------------------------------
bool BroadcastSessionSetupFailureMsg::getMbsSessionId(
    MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
bool BroadcastSessionSetupFailureMsg::getCause(Cause& v) const {
  v = m_Cause;
  return true;
}

//------------------------------------------------------------------------------
bool BroadcastSessionSetupFailureMsg::getCriticalityDiagnostics(
    CriticalityDiagnostics& v) const {
  if (!m_CriticalityDiag.has_value()) return false;
  v = m_CriticalityDiag.value();
  return true;
}

//------------------------------------------------------------------------------
bool BroadcastSessionSetupFailureMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_unsuccessfulOutcome &&
      ngapPdu->choice.unsuccessfulOutcome &&
      ngapPdu->choice.unsuccessfulOutcome->procedureCode ==
          Ngap_ProcedureCode_id_BroadcastSessionSetup &&
      ngapPdu->choice.unsuccessfulOutcome->value.present ==
          Ngap_UnsuccessfulOutcome__value_PR_BroadcastSessionSetupFailure) {
    m_BroadcastSessionSetupFailureIes =
        &ngapPdu->choice.unsuccessfulOutcome->value.choice
             .BroadcastSessionSetupFailure;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check BroadcastSessionSetupFailure message error");
    return false;
  }

  for (int i = 0;
       i < m_BroadcastSessionSetupFailureIes->protocolIEs->list.count;
       i++) {
    Ngap_BroadcastSessionSetupFailureIEs_t* ie =
        (Ngap_BroadcastSessionSetupFailureIEs_t*)
            m_BroadcastSessionSetupFailureIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_BroadcastSessionSetupFailureIEs__value_PR_MBS_SessionID) {
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
      case Ngap_ProtocolIE_ID_id_MBSSessionSetupFailureTransfer: {
        // Transfer as OCTET_STRING; decoded by application layer.
      } break;
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (ie->criticality == Ngap_Criticality_ignore &&
            ie->value.present ==
                Ngap_BroadcastSessionSetupFailureIEs__value_PR_Cause) {
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
                Ngap_BroadcastSessionSetupFailureIEs__value_PR_CriticalityDiagnostics) {
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
