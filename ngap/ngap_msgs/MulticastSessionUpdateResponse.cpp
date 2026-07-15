/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MulticastSessionUpdateResponse.hpp"

#include "logger_base.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
MulticastSessionUpdateResponseMsg::MulticastSessionUpdateResponseMsg()
    : NgapMessage() {
  m_MulticastSessionUpdateResponseIes = nullptr;
  m_MbsAreaSessionId                  = std::nullopt;
  m_CriticalityDiag                   = std::nullopt;

  NgapMessage::setMessageType(
      NgapMessageType::MULTICAST_SESSION_UPDATE_RESPONSE);
  initialize();
}

//------------------------------------------------------------------------------
void MulticastSessionUpdateResponseMsg::initialize() {
  m_MulticastSessionUpdateResponseIes =
      &(ngapPdu->choice.successfulOutcome->value.choice
            .MulticastSessionUpdateResponse);
}

//------------------------------------------------------------------------------
bool MulticastSessionUpdateResponseMsg::getMbsSessionId(MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
bool MulticastSessionUpdateResponseMsg::getMbsAreaSessionId(
    MbsAreaSessionId& v) const {
  if (!m_MbsAreaSessionId.has_value()) return false;
  v = m_MbsAreaSessionId.value();
  return true;
}

//------------------------------------------------------------------------------
bool MulticastSessionUpdateResponseMsg::getCriticalityDiagnostics(
    CriticalityDiagnostics& v) const {
  if (!m_CriticalityDiag.has_value()) return false;
  v = m_CriticalityDiag.value();
  return true;
}

//------------------------------------------------------------------------------
bool MulticastSessionUpdateResponseMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome &&
      ngapPdu->choice.successfulOutcome &&
      ngapPdu->choice.successfulOutcome->procedureCode ==
          Ngap_ProcedureCode_id_MulticastSessionUpdate &&
      ngapPdu->choice.successfulOutcome->value.present ==
          Ngap_SuccessfulOutcome__value_PR_MulticastSessionUpdateResponse) {
    m_MulticastSessionUpdateResponseIes =
        &ngapPdu->choice.successfulOutcome->value.choice
             .MulticastSessionUpdateResponse;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check MulticastSessionUpdateResponse message error");
    return false;
  }

  for (int i = 0;
       i < m_MulticastSessionUpdateResponseIes->protocolIEs->list.count; i++) {
    Ngap_MulticastSessionUpdateResponseIEs_t* ie =
        (Ngap_MulticastSessionUpdateResponseIEs_t*)
            m_MulticastSessionUpdateResponseIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_MulticastSessionUpdateResponseIEs__value_PR_MBS_SessionID) {
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
                Ngap_MulticastSessionUpdateResponseIEs__value_PR_MBS_AreaSessionID) {
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
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        if (ie->criticality == Ngap_Criticality_ignore &&
            ie->value.present ==
                Ngap_MulticastSessionUpdateResponseIEs__value_PR_CriticalityDiagnostics) {
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
