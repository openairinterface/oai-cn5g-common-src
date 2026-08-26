/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "UeContextModificationResponse.hpp"

#include "logger_base.hpp"
extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}
#include "ngap_utils.hpp"
#include "utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
UeContextModificationResponseMsg::UeContextModificationResponseMsg()
    : NgapUeMessage() {
  m_UeContextModificationResponseIes = nullptr;
  setMessageType(NgapMessageType::UE_CONTEXT_MODIFICATION_RESPONSE);
  initialize();
}

//------------------------------------------------------------------------------
UeContextModificationResponseMsg::~UeContextModificationResponseMsg() {}

//------------------------------------------------------------------------------
void UeContextModificationResponseMsg::initialize() {
  m_UeContextModificationResponseIes =
      &(ngapPdu->choice.successfulOutcome->value.choice
            .UEContextModificationResponse);
  if (!m_UeContextModificationResponseIes->protocolIEs) {
    m_UeContextModificationResponseIes->protocolIEs =
        (struct Ngap_ProtocolIE_Container*) calloc(
            1, sizeof(struct Ngap_ProtocolIE_Container));
  }
}

//------------------------------------------------------------------------------
void UeContextModificationResponseMsg::setAmfUeNgapId(const uint64_t& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_UEContextModificationResponseIEs_t* ie =
      (Ngap_UEContextModificationResponseIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextModificationResponseIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP AMF_UE_NGAP_ID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationResponseIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UeContextModificationResponseMsg::setRanUeNgapId(
    const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_UEContextModificationResponseIEs_t* ie =
      (Ngap_UEContextModificationResponseIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextModificationResponseIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RAN_UE_NGAP_ID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationResponseIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
bool UeContextModificationResponseMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  if (!ngapMsgPdu) return false;
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_UEContextModification &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_UEContextModificationResponse) {
      m_UeContextModificationResponseIes =
          &ngapPdu->choice.successfulOutcome->value.choice
               .UEContextModificationResponse;
    } else {
      oai::logger::logger_common::ngap().error(
          "Check UEContextModificationResponse message error!");
      return false;
    }
  } else {
    oai::logger::logger_common::ngap().error(
        "UEContextModificationResponse MessageType error!");
    return false;
  }

  for (int i = 0;
       i < m_UeContextModificationResponseIes->protocolIEs->list.count; i++) {
    Ngap_UEContextModificationResponseIEs_t* ngap_ie =
        (Ngap_UEContextModificationResponseIEs_t*)
            m_UeContextModificationResponseIes->protocolIEs->list.array[i];
    switch (ngap_ie->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ngap_ie->criticality == Ngap_Criticality_reject &&
            ngap_ie->value.present ==
                Ngap_UEContextModificationResponseIEs__value_PR_AMF_UE_NGAP_ID) {
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
                Ngap_UEContextModificationResponseIEs__value_PR_RAN_UE_NGAP_ID) {
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
      case Ngap_ProtocolIE_ID_id_RRCState: {
        // TODO: decode RRCState (Optional)
      } break;
      case Ngap_ProtocolIE_ID_id_UserLocationInformation: {
        // TODO: decode UserLocationInformation (Optional)
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
