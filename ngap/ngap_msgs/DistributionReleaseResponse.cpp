/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "DistributionReleaseResponse.hpp"

#include "logger_base.hpp"
#include "utils.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
DistributionReleaseResponseMsg::DistributionReleaseResponseMsg()
    : NgapMessage() {
  m_DistributionReleaseResponseIes = nullptr;
  m_MbsAreaSessionId               = std::nullopt;
  m_CriticalityDiag                = std::nullopt;

  NgapMessage::setMessageType(NgapMessageType::DISTRIBUTION_RELEASE_RESPONSE);
  initialize();
}

//------------------------------------------------------------------------------
void DistributionReleaseResponseMsg::initialize() {
  m_DistributionReleaseResponseIes =
      &(ngapPdu->choice.successfulOutcome->value.choice
            .DistributionReleaseResponse);
}

//------------------------------------------------------------------------------
void DistributionReleaseResponseMsg::setMbsSessionId(const MbsSessionId& v) {
  m_MbsSessionId = v;

  Ngap_DistributionReleaseResponseIEs_t* ie =
      (Ngap_DistributionReleaseResponseIEs_t*) calloc(
          1, sizeof(Ngap_DistributionReleaseResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MBS_SessionID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DistributionReleaseResponseIEs__value_PR_MBS_SessionID;

  if (!m_MbsSessionId.encode(ie->value.choice.MBS_SessionID)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_DistributionReleaseResponseIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
}

//------------------------------------------------------------------------------
bool DistributionReleaseResponseMsg::getMbsSessionId(MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
void DistributionReleaseResponseMsg::setMbsAreaSessionId(
    const MbsAreaSessionId& v) {
  m_MbsAreaSessionId = std::make_optional<MbsAreaSessionId>(v);

  Ngap_DistributionReleaseResponseIEs_t* ie =
      (Ngap_DistributionReleaseResponseIEs_t*) calloc(
          1, sizeof(Ngap_DistributionReleaseResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MBS_AreaSessionID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DistributionReleaseResponseIEs__value_PR_MBS_AreaSessionID;

  if (!m_MbsAreaSessionId.value().encode(
          ie->value.choice.MBS_AreaSessionID)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-AreaSessionID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_DistributionReleaseResponseIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-AreaSessionID IE error");
}

//------------------------------------------------------------------------------
bool DistributionReleaseResponseMsg::getMbsAreaSessionId(
    MbsAreaSessionId& v) const {
  if (!m_MbsAreaSessionId.has_value()) return false;
  v = m_MbsAreaSessionId.value();
  return true;
}

//------------------------------------------------------------------------------
bool DistributionReleaseResponseMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome &&
      ngapPdu->choice.successfulOutcome &&
      ngapPdu->choice.successfulOutcome->procedureCode ==
          Ngap_ProcedureCode_id_DistributionRelease &&
      ngapPdu->choice.successfulOutcome->value.present ==
          Ngap_SuccessfulOutcome__value_PR_DistributionReleaseResponse) {
    m_DistributionReleaseResponseIes =
        &ngapPdu->choice.successfulOutcome->value.choice
             .DistributionReleaseResponse;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check DistributionReleaseResponse message error");
    return false;
  }

  for (int i = 0;
       i < m_DistributionReleaseResponseIes->protocolIEs->list.count;
       i++) {
    Ngap_DistributionReleaseResponseIEs_t* ie =
        (Ngap_DistributionReleaseResponseIEs_t*)
            m_DistributionReleaseResponseIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_DistributionReleaseResponseIEs__value_PR_MBS_SessionID) {
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
                Ngap_DistributionReleaseResponseIEs__value_PR_MBS_AreaSessionID) {
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
                Ngap_DistributionReleaseResponseIEs__value_PR_CriticalityDiagnostics) {
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
