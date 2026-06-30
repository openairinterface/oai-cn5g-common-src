/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "UeContextModificationFailure.hpp"

#include "logger_base.hpp"
extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}
#include "ngap_utils.hpp"
#include "utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
UeContextModificationFailureMsg::UeContextModificationFailureMsg()
    : NgapUeMessage() {
  m_UeContextModificationFailureIes = nullptr;
  setMessageType(NgapMessageType::UE_CONTEXT_MODIFICATION_FAILURE);
  initialize();
}

//------------------------------------------------------------------------------
UeContextModificationFailureMsg::~UeContextModificationFailureMsg() {}

//------------------------------------------------------------------------------
void UeContextModificationFailureMsg::initialize() {
  m_UeContextModificationFailureIes =
      &(ngapPdu->choice.unsuccessfulOutcome->value.choice
            .UEContextModificationFailure);
}

//------------------------------------------------------------------------------
void UeContextModificationFailureMsg::setAmfUeNgapId(const uint64_t& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_UEContextModificationFailureIEs_t* ie =
      (Ngap_UEContextModificationFailureIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationFailureIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextModificationFailureIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP AMF_UE_NGAP_ID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationFailureIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UeContextModificationFailureMsg::setRanUeNgapId(
    const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_UEContextModificationFailureIEs_t* ie =
      (Ngap_UEContextModificationFailureIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationFailureIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextModificationFailureIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RAN_UE_NGAP_ID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationFailureIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
bool UeContextModificationFailureMsg::getCause(Cause& cause) const {
  cause = m_Cause;
  return true;
}

//------------------------------------------------------------------------------
bool UeContextModificationFailureMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  if (!ngapMsgPdu) return false;
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_unsuccessfulOutcome) {
    if (ngapPdu->choice.unsuccessfulOutcome &&
        ngapPdu->choice.unsuccessfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_UEContextModification &&
        ngapPdu->choice.unsuccessfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.unsuccessfulOutcome->value.present ==
            Ngap_UnsuccessfulOutcome__value_PR_UEContextModificationFailure) {
      m_UeContextModificationFailureIes =
          &ngapPdu->choice.unsuccessfulOutcome->value.choice
               .UEContextModificationFailure;
    } else {
      oai::logger::logger_common::ngap().error(
          "Check UEContextModificationFailure message error!");
      return false;
    }
  } else {
    oai::logger::logger_common::ngap().error(
        "UEContextModificationFailure MessageType error!");
    return false;
  }

  for (int i = 0;
       i < m_UeContextModificationFailureIes->protocolIEs->list.count; i++) {
    Ngap_UEContextModificationFailureIEs_t* ngap_ie =
        (Ngap_UEContextModificationFailureIEs_t*)
            m_UeContextModificationFailureIes->protocolIEs->list.array[i];
    switch (ngap_ie->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ngap_ie->criticality == Ngap_Criticality_reject &&
            ngap_ie->value.present ==
                Ngap_UEContextModificationFailureIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!NgapUeMessage::m_AmfUeNgapId.decode(
                  ngap_ie->value.choice.AMF_UE_NGAP_ID)) {
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
        if (ngap_ie->criticality == Ngap_Criticality_reject &&
            ngap_ie->value.present ==
                Ngap_UEContextModificationFailureIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!NgapUeMessage::m_RanUeNgapId.decode(
                  ngap_ie->value.choice.RAN_UE_NGAP_ID)) {
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
        if (ngap_ie->criticality == Ngap_Criticality_ignore &&
            ngap_ie->value.present ==
                Ngap_UEContextModificationFailureIEs__value_PR_Cause) {
          if (!m_Cause.decode(ngap_ie->value.choice.Cause)) {
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
        // TODO: decode CriticalityDiagnostics (Optional)
      } break;
      default: {
        oai::logger::logger_common::ngap().warn(
            "Not decoded IE %d", ngap_ie->id);
        break;
      }
    }
  }

  return true;
}

}  // namespace oai::ngap
