/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "DistributionSetupFailure.hpp"

#include "logger_base.hpp"
#include "utils.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
DistributionSetupFailureMsg::DistributionSetupFailureMsg() : NgapMessage() {
  m_DistributionSetupFailureIes = nullptr;
  m_MbsAreaSessionId            = std::nullopt;
  m_CriticalityDiag             = std::nullopt;

  NgapMessage::setMessageType(NgapMessageType::DISTRIBUTION_SETUP_FAILURE);
  initialize();
}

//------------------------------------------------------------------------------
void DistributionSetupFailureMsg::initialize() {
  m_DistributionSetupFailureIes = &(ngapPdu->choice.unsuccessfulOutcome->value
                                        .choice.DistributionSetupFailure);
}

//------------------------------------------------------------------------------
void DistributionSetupFailureMsg::setMbsSessionId(const MbsSessionId& v) {
  m_MbsSessionId = v;

  Ngap_DistributionSetupFailureIEs_t* ie =
      (Ngap_DistributionSetupFailureIEs_t*) calloc(
          1, sizeof(Ngap_DistributionSetupFailureIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_MBS_SessionID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DistributionSetupFailureIEs__value_PR_MBS_SessionID;

  if (!m_MbsSessionId.encode(ie->value.choice.MBS_SessionID)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret =
      ASN_SEQUENCE_ADD(&m_DistributionSetupFailureIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
}

//------------------------------------------------------------------------------
bool DistributionSetupFailureMsg::getMbsSessionId(MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
void DistributionSetupFailureMsg::setMbsAreaSessionId(
    const MbsAreaSessionId& v) {
  m_MbsAreaSessionId = std::make_optional<MbsAreaSessionId>(v);

  Ngap_DistributionSetupFailureIEs_t* ie =
      (Ngap_DistributionSetupFailureIEs_t*) calloc(
          1, sizeof(Ngap_DistributionSetupFailureIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MBS_AreaSessionID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DistributionSetupFailureIEs__value_PR_MBS_AreaSessionID;

  if (!m_MbsAreaSessionId.value().encode(ie->value.choice.MBS_AreaSessionID)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-AreaSessionID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret =
      ASN_SEQUENCE_ADD(&m_DistributionSetupFailureIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-AreaSessionID IE error");
}

//------------------------------------------------------------------------------
bool DistributionSetupFailureMsg::getMbsAreaSessionId(
    MbsAreaSessionId& v) const {
  if (!m_MbsAreaSessionId.has_value()) return false;
  v = m_MbsAreaSessionId.value();
  return true;
}

//------------------------------------------------------------------------------
void DistributionSetupFailureMsg::setMbsDistributionSetupUnsuccessfulTransfer(
    const MbsDistributionSetupUnsuccessfulTransfer& v) {
  uint8_t buf[512] = {};
  int len          = v.encode(buf, sizeof(buf));
  if (len <= 0) {
    oai::logger::logger_common::ngap().error(
        "Encode MbsDistributionSetupUnsuccessfulTransfer error");
    return;
  }

  Ngap_DistributionSetupFailureIEs_t* ie =
      (Ngap_DistributionSetupFailureIEs_t*) calloc(
          1, sizeof(Ngap_DistributionSetupFailureIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_MBS_DistributionSetupUnsuccessfulTransfer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DistributionSetupFailureIEs__value_PR_OCTET_STRING_CONTAINING_MBS_DistributionSetupUnsuccessfulTransfer_;

  OCTET_STRING_fromBuf(
      &ie->value.choice
           .OCTET_STRING_CONTAINING_MBS_DistributionSetupUnsuccessfulTransfer_,
      (const char*) buf, len);

  int ret =
      ASN_SEQUENCE_ADD(&m_DistributionSetupFailureIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS_DistributionSetupUnsuccessfulTransfer IE error");
}

//------------------------------------------------------------------------------
void DistributionSetupFailureMsg::setCause(const Cause& v) {
  m_Cause = v;

  Ngap_DistributionSetupFailureIEs_t* ie =
      (Ngap_DistributionSetupFailureIEs_t*) calloc(
          1, sizeof(Ngap_DistributionSetupFailureIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_DistributionSetupFailureIEs__value_PR_Cause;

  if (!m_Cause.encode(ie->value.choice.Cause)) {
    oai::logger::logger_common::ngap().error("Encode NGAP Cause IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret =
      ASN_SEQUENCE_ADD(&m_DistributionSetupFailureIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode NGAP Cause IE error");
}

//------------------------------------------------------------------------------
bool DistributionSetupFailureMsg::getCause(Cause& v) const {
  v = m_Cause;
  return true;
}

//------------------------------------------------------------------------------
bool DistributionSetupFailureMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_unsuccessfulOutcome &&
      ngapPdu->choice.unsuccessfulOutcome &&
      ngapPdu->choice.unsuccessfulOutcome->procedureCode ==
          Ngap_ProcedureCode_id_DistributionSetup &&
      ngapPdu->choice.unsuccessfulOutcome->value.present ==
          Ngap_UnsuccessfulOutcome__value_PR_DistributionSetupFailure) {
    m_DistributionSetupFailureIes = &ngapPdu->choice.unsuccessfulOutcome->value
                                         .choice.DistributionSetupFailure;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check DistributionSetupFailure message error");
    return false;
  }

  for (int i = 0; i < m_DistributionSetupFailureIes->protocolIEs->list.count;
       i++) {
    Ngap_DistributionSetupFailureIEs_t* ie =
        (Ngap_DistributionSetupFailureIEs_t*)
            m_DistributionSetupFailureIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_DistributionSetupFailureIEs__value_PR_MBS_SessionID) {
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
                Ngap_DistributionSetupFailureIEs__value_PR_MBS_AreaSessionID) {
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
      case Ngap_ProtocolIE_ID_id_MBS_DistributionSetupUnsuccessfulTransfer: {
        // Transfer as OCTET_STRING; decoded by application layer.
      } break;
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (ie->criticality == Ngap_Criticality_ignore &&
            ie->value.present ==
                Ngap_DistributionSetupFailureIEs__value_PR_Cause) {
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
                Ngap_DistributionSetupFailureIEs__value_PR_CriticalityDiagnostics) {
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
