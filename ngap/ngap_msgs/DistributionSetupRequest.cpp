/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "DistributionSetupRequest.hpp"

#include "logger_base.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
DistributionSetupRequestMsg::DistributionSetupRequestMsg() : NgapMessage() {
  m_DistributionSetupRequestIes = nullptr;
  m_MbsAreaSessionId            = std::nullopt;

  NgapMessage::setMessageType(NgapMessageType::DISTRIBUTION_SETUP_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
void DistributionSetupRequestMsg::initialize() {
  m_DistributionSetupRequestIes =
      &(ngapPdu->choice.initiatingMessage->value.choice.DistributionSetupRequest);
}

//------------------------------------------------------------------------------
bool DistributionSetupRequestMsg::getMbsSessionId(MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
bool DistributionSetupRequestMsg::getMbsAreaSessionId(
    MbsAreaSessionId& v) const {
  if (!m_MbsAreaSessionId.has_value()) return false;
  v = m_MbsAreaSessionId.value();
  return true;
}

//------------------------------------------------------------------------------
bool DistributionSetupRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage &&
      ngapPdu->choice.initiatingMessage &&
      ngapPdu->choice.initiatingMessage->procedureCode ==
          Ngap_ProcedureCode_id_DistributionSetup &&
      ngapPdu->choice.initiatingMessage->value.present ==
          Ngap_InitiatingMessage__value_PR_DistributionSetupRequest) {
    m_DistributionSetupRequestIes =
        &ngapPdu->choice.initiatingMessage->value.choice.DistributionSetupRequest;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check DistributionSetupRequest message error");
    return false;
  }

  bool hasMbsSessionId = false;
  for (int i = 0;
       i < m_DistributionSetupRequestIes->protocolIEs->list.count;
       i++) {
    Ngap_DistributionSetupRequestIEs_t* ie =
        (Ngap_DistributionSetupRequestIEs_t*)
            m_DistributionSetupRequestIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_DistributionSetupRequestIEs__value_PR_MBS_SessionID) {
          if (!m_MbsSessionId.decode(ie->value.choice.MBS_SessionID)) {
            oai::logger::logger_common::ngap().error(
                "Decode NGAP MBS-SessionID IE error");
            return false;
          }
          hasMbsSessionId = true;
        } else {
          oai::logger::logger_common::ngap().error(
              "Decode NGAP MBS-SessionID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_MBS_AreaSessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_DistributionSetupRequestIEs__value_PR_MBS_AreaSessionID) {
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
      case Ngap_ProtocolIE_ID_id_MBS_DistributionSetupRequestTransfer: {
        // Transfer as OCTET_STRING; decoded by application layer.
      } break;
      default: {
        break;
      }
    }
  }

  // Defensive: mandatory MBS-SessionID must be present
  if (!hasMbsSessionId) {
    oai::logger::logger_common::ngap().error(
        "DistributionSetupRequest missing mandatory MBS-SessionID");
    return false;
  }

  return true;
}

}  // namespace oai::ngap
