/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "ErrorIndication.hpp"

#include "logger_base.hpp"
#include "utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
ErrorIndication::ErrorIndication() : NgapUeMessage() {
  m_ErrorIndicationIes     = nullptr;
  m_Cause                  = std::nullopt;
  m_CriticalityDiagnostics = std::nullopt;
  m_FiveGSTmsi             = std::nullopt;

  setMessageType(NgapMessageType::ERROR_INDICATION);
  initialize();
}

//------------------------------------------------------------------------------
ErrorIndication::~ErrorIndication() {}

//------------------------------------------------------------------------------
void ErrorIndication::initialize() {
  m_ErrorIndicationIes =
      &(ngapPdu->choice.initiatingMessage->value.choice.ErrorIndication);
}

//------------------------------------------------------------------------------
void ErrorIndication::setAmfUeNgapId(const uint64_t& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_ErrorIndicationIEs_t* ie =
      (Ngap_ErrorIndicationIEs_t*) calloc(1, sizeof(Ngap_ErrorIndicationIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_ErrorIndicationIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_ErrorIndicationIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void ErrorIndication::setRanUeNgapId(const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_ErrorIndicationIEs_t* ie =
      (Ngap_ErrorIndicationIEs_t*) calloc(1, sizeof(Ngap_ErrorIndicationIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_ErrorIndicationIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_ErrorIndicationIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void ErrorIndication::setCause(const Cause& cause) {
  Ngap_ErrorIndicationIEs_t* ie =
      (Ngap_ErrorIndicationIEs_t*) calloc(1, sizeof(Ngap_ErrorIndicationIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_ErrorIndicationIEs__value_PR_Cause;

  Cause tmp = {};
  tmp.setChoiceOfCause(cause.getChoiceOfCause());
  tmp.set(cause.get());
  m_Cause = std::optional<Cause>(tmp);

  int ret = tmp.encode(ie->value.choice.Cause);

  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode Cause IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_ErrorIndicationIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode Cause IE error");
}

//------------------------------------------------------------------------------
void ErrorIndication::setCause(
    const Ngap_Cause_PR& causePresent, const long& value) {
  Ngap_ErrorIndicationIEs_t* ie =
      (Ngap_ErrorIndicationIEs_t*) calloc(1, sizeof(Ngap_ErrorIndicationIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_ErrorIndicationIEs__value_PR_Cause;

  Cause tmp = {};
  tmp.setChoiceOfCause(causePresent);
  tmp.set(value);
  m_Cause = std::optional<Cause>(tmp);

  int ret = tmp.encode(ie->value.choice.Cause);

  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode Cause IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_ErrorIndicationIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode Cause IE error");
}

//------------------------------------------------------------------------------
void ErrorIndication::getCause(std::optional<Cause>& cause) const {
  // TOTO:
}

//------------------------------------------------------------------------------
void ErrorIndication::setCriticalityDiagnostics(
    const CriticalityDiagnostics& criticalityDiagnostics) {
  Ngap_ErrorIndicationIEs_t* ie =
      (Ngap_ErrorIndicationIEs_t*) calloc(1, sizeof(Ngap_ErrorIndicationIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_CriticalityDiagnostics;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_ErrorIndicationIEs__value_PR_CriticalityDiagnostics;

  // TODO: set value of IE

  int ret = ASN_SEQUENCE_ADD(&m_ErrorIndicationIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode CriticalityDiagnostics IE error");
}

//------------------------------------------------------------------------------
void ErrorIndication::getCriticalityDiagnostics(
    std::optional<CriticalityDiagnostics>& criticalityDiagnostics) const {
  // TOTO:
}

//------------------------------------------------------------------------------
void ErrorIndication::setFiveGSTmsi(const FiveGSTmsi& fiveGSTmsi) {
  Ngap_ErrorIndicationIEs_t* ie =
      (Ngap_ErrorIndicationIEs_t*) calloc(1, sizeof(Ngap_ErrorIndicationIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_FiveG_S_TMSI;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_ErrorIndicationIEs__value_PR_FiveG_S_TMSI;

  m_FiveGSTmsi = std::optional<FiveGSTmsi>(fiveGSTmsi);

  int ret = fiveGSTmsi.encode(ie->value.choice.FiveG_S_TMSI);

  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode FiveG_S_TMSI IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_ErrorIndicationIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode FiveG_S_TMSI IE error");
}
//------------------------------------------------------------------------------
void ErrorIndication::getFiveGSTmsi(
    std::optional<FiveGSTmsi>& fiveGSTmsi) const {
  fiveGSTmsi = m_FiveGSTmsi;
}

//------------------------------------------------------------------------------
bool ErrorIndication::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_ErrorIndication &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_ErrorIndication) {
      m_ErrorIndicationIes =
          &ngapPdu->choice.initiatingMessage->value.choice.ErrorIndication;
    } else {
      oai::logger::logger_common::ngap().error(
          "Decode NGAP ErrorIndication error");
      return false;
    }
  } else {
    oai::logger::logger_common::ngap().error(
        "Decode NGAP MessageType IE error");
    return false;
  }
  for (int i = 0; i < m_ErrorIndicationIes->protocolIEs->list.count; i++) {
    switch (m_ErrorIndicationIes->protocolIEs->list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (m_ErrorIndicationIes->protocolIEs->list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_ErrorIndicationIes->protocolIEs->list.array[i]->value.present ==
                Ngap_ErrorIndicationIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!NgapUeMessage::m_AmfUeNgapId.decode(
                  m_ErrorIndicationIes->protocolIEs->list.array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            oai::logger::logger_common::ngap().error(
                "Decode NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decode NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (m_ErrorIndicationIes->protocolIEs->list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_ErrorIndicationIes->protocolIEs->list.array[i]->value.present ==
                Ngap_ErrorIndicationIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!NgapUeMessage::m_RanUeNgapId.decode(
                  m_ErrorIndicationIes->protocolIEs->list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            oai::logger::logger_common::ngap().error(
                "Decode NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decode NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (m_ErrorIndicationIes->protocolIEs->list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_ErrorIndicationIes->protocolIEs->list.array[i]->value.present ==
                Ngap_ErrorIndicationIEs__value_PR_Cause) {
          Cause tmp = {};
          if (!tmp.decode(m_ErrorIndicationIes->protocolIEs->list.array[i]
                              ->value.choice.Cause)) {
            oai::logger::logger_common::ngap().error(
                "Decode NGAP Cause IE error");
            return false;
          }
          m_Cause = std::optional<Cause>(tmp);
        } else {
          oai::logger::logger_common::ngap().error(
              "Decode NGAP Cause IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        if (m_ErrorIndicationIes->protocolIEs->list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_ErrorIndicationIes->protocolIEs->list.array[i]->value.present ==
                Ngap_ErrorIndicationIEs__value_PR_CriticalityDiagnostics) {
          CriticalityDiagnostics tmp = {};
          if (!tmp.decode(m_ErrorIndicationIes->protocolIEs->list.array[i]
                              ->value.choice.CriticalityDiagnostics)) {
            oai::logger::logger_common::ngap().error(
                "Decode NGAP CriticalityDiagnostics IE error");
            return false;
          }
          m_CriticalityDiagnostics = std::optional<CriticalityDiagnostics>(tmp);
        } else {
          oai::logger::logger_common::ngap().error(
              "Decode NGAP CriticalityDiagnostics IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_FiveG_S_TMSI: {
        if (m_ErrorIndicationIes->protocolIEs->list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_ErrorIndicationIes->protocolIEs->list.array[i]->value.present ==
                Ngap_ErrorIndicationIEs__value_PR_FiveG_S_TMSI) {
          FiveGSTmsi tmp = {};
          if (!tmp.decode(m_ErrorIndicationIes->protocolIEs->list.array[i]
                              ->value.choice.FiveG_S_TMSI)) {
            oai::logger::logger_common::ngap().error(
                "Decode NGAP FiveGSTmsi IE error");
            return false;
          }
          m_FiveGSTmsi = std::optional<FiveGSTmsi>(tmp);
        } else {
          oai::logger::logger_common::ngap().error(
              "Decode NGAP FiveGSTmsi IE error");
          return false;
        }
      } break;

      default: {
        oai::logger::logger_common::ngap().error(
            "Decode NGAP message PDU error");
        return false;
      }
    }
  }

  return true;
}

}  // namespace oai::ngap
