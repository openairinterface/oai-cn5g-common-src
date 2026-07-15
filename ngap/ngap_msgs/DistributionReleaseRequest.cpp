/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "DistributionReleaseRequest.hpp"

#include "logger_base.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
DistributionReleaseRequestMsg::DistributionReleaseRequestMsg() : NgapMessage() {
  m_DistributionReleaseRequestIes = nullptr;
  m_MbsAreaSessionId              = std::nullopt;

  NgapMessage::setMessageType(NgapMessageType::DISTRIBUTION_RELEASE_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
void DistributionReleaseRequestMsg::initialize() {
  m_DistributionReleaseRequestIes = &(ngapPdu->choice.initiatingMessage->value
                                          .choice.DistributionReleaseRequest);
}

//------------------------------------------------------------------------------
bool DistributionReleaseRequestMsg::getMbsSessionId(MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
bool DistributionReleaseRequestMsg::getMbsAreaSessionId(
    MbsAreaSessionId& v) const {
  if (!m_MbsAreaSessionId.has_value()) return false;
  v = m_MbsAreaSessionId.value();
  return true;
}

//------------------------------------------------------------------------------
bool DistributionReleaseRequestMsg::getCause(Cause& v) const {
  v = m_Cause;
  return true;
}

//------------------------------------------------------------------------------
bool DistributionReleaseRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage &&
      ngapPdu->choice.initiatingMessage &&
      ngapPdu->choice.initiatingMessage->procedureCode ==
          Ngap_ProcedureCode_id_DistributionRelease &&
      ngapPdu->choice.initiatingMessage->value.present ==
          Ngap_InitiatingMessage__value_PR_DistributionReleaseRequest) {
    m_DistributionReleaseRequestIes = &ngapPdu->choice.initiatingMessage->value
                                           .choice.DistributionReleaseRequest;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check DistributionReleaseRequest message error");
    return false;
  }

  bool hasMbsSessionId = false;
  for (int i = 0; i < m_DistributionReleaseRequestIes->protocolIEs->list.count;
       i++) {
    Ngap_DistributionReleaseRequestIEs_t* ie =
        (Ngap_DistributionReleaseRequestIEs_t*)
            m_DistributionReleaseRequestIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_DistributionReleaseRequestIEs__value_PR_MBS_SessionID) {
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
                Ngap_DistributionReleaseRequestIEs__value_PR_MBS_AreaSessionID) {
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
      case Ngap_ProtocolIE_ID_id_MBS_DistributionReleaseRequestTransfer: {
        // Transfer as OCTET_STRING; decoded by application layer.
      } break;
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (ie->criticality == Ngap_Criticality_ignore &&
            ie->value.present ==
                Ngap_DistributionReleaseRequestIEs__value_PR_Cause) {
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

  // Defensive: mandatory MBS-SessionID must be present
  if (!hasMbsSessionId) {
    oai::logger::logger_common::ngap().error(
        "DistributionReleaseRequest missing mandatory MBS-SessionID");
    return false;
  }

  return true;
}

}  // namespace oai::ngap
