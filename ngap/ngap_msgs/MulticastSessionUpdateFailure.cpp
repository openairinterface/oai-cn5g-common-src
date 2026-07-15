/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MulticastSessionUpdateFailure.hpp"

#include "logger_base.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
MulticastSessionUpdateFailureMsg::MulticastSessionUpdateFailureMsg()
    : NgapMessage() {
  m_MulticastSessionUpdateFailureIes = nullptr;
  m_MbsAreaSessionId                 = std::nullopt;
  m_CriticalityDiag                  = std::nullopt;

  NgapMessage::setMessageType(
      NgapMessageType::MULTICAST_SESSION_UPDATE_FAILURE);
  initialize();
}

//------------------------------------------------------------------------------
void MulticastSessionUpdateFailureMsg::initialize() {
  m_MulticastSessionUpdateFailureIes =
      &(ngapPdu->choice.unsuccessfulOutcome->value.choice
            .MulticastSessionUpdateFailure);
}

//------------------------------------------------------------------------------
bool MulticastSessionUpdateFailureMsg::getMbsSessionId(MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
bool MulticastSessionUpdateFailureMsg::getMbsAreaSessionId(
    MbsAreaSessionId& v) const {
  if (!m_MbsAreaSessionId.has_value()) return false;
  v = m_MbsAreaSessionId.value();
  return true;
}

//------------------------------------------------------------------------------
bool MulticastSessionUpdateFailureMsg::getCause(Cause& v) const {
  v = m_Cause;
  return true;
}

//------------------------------------------------------------------------------
bool MulticastSessionUpdateFailureMsg::getCriticalityDiagnostics(
    CriticalityDiagnostics& v) const {
  if (!m_CriticalityDiag.has_value()) return false;
  v = m_CriticalityDiag.value();
  return true;
}

//------------------------------------------------------------------------------
bool MulticastSessionUpdateFailureMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_unsuccessfulOutcome &&
      ngapPdu->choice.unsuccessfulOutcome &&
      ngapPdu->choice.unsuccessfulOutcome->procedureCode ==
          Ngap_ProcedureCode_id_MulticastSessionUpdate &&
      ngapPdu->choice.unsuccessfulOutcome->value.present ==
          Ngap_UnsuccessfulOutcome__value_PR_MulticastSessionUpdateFailure) {
    m_MulticastSessionUpdateFailureIes =
        &ngapPdu->choice.unsuccessfulOutcome->value.choice
             .MulticastSessionUpdateFailure;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check MulticastSessionUpdateFailure message error");
    return false;
  }

  for (int i = 0;
       i < m_MulticastSessionUpdateFailureIes->protocolIEs->list.count; i++) {
    Ngap_MulticastSessionUpdateFailureIEs_t* ie =
        (Ngap_MulticastSessionUpdateFailureIEs_t*)
            m_MulticastSessionUpdateFailureIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_MulticastSessionUpdateFailureIEs__value_PR_MBS_SessionID) {
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
      case Ngap_ProtocolIE_ID_id_MBS_AreaSessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_MulticastSessionUpdateFailureIEs__value_PR_MBS_AreaSessionID) {
          m_MbsAreaSessionId = std::make_optional<MbsAreaSessionId>();
          if (!m_MbsAreaSessionId.value().decode(
                  ie->value.choice.MBS_AreaSessionID)) {
            oai::logger::logger_common::ngap().error(
                "Decode NGAP MBS-AreaSessionID IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decode NGAP MBS-AreaSessionID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (ie->criticality == Ngap_Criticality_ignore &&
            ie->value.present ==
                Ngap_MulticastSessionUpdateFailureIEs__value_PR_Cause) {
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
                Ngap_MulticastSessionUpdateFailureIEs__value_PR_CriticalityDiagnostics) {
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
