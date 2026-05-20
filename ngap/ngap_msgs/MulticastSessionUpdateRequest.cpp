/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MulticastSessionUpdateRequest.hpp"

#include "logger_base.hpp"
#include "utils.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
MulticastSessionUpdateRequestMsg::MulticastSessionUpdateRequestMsg()
    : NgapMessage() {
  m_MulticastSessionUpdateRequestIes = nullptr;
  m_MbsAreaSessionId                 = std::nullopt;

  NgapMessage::setMessageType(
      NgapMessageType::MULTICAST_SESSION_UPDATE_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
void MulticastSessionUpdateRequestMsg::initialize() {
  m_MulticastSessionUpdateRequestIes =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .MulticastSessionUpdateRequest);
}

//------------------------------------------------------------------------------
void MulticastSessionUpdateRequestMsg::setMbsSessionId(
    const MbsSessionId& v) {
  m_MbsSessionId = v;

  Ngap_MulticastSessionUpdateRequestIEs_t* ie =
      (Ngap_MulticastSessionUpdateRequestIEs_t*) calloc(
          1, sizeof(Ngap_MulticastSessionUpdateRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MBS_SessionID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_MulticastSessionUpdateRequestIEs__value_PR_MBS_SessionID;

  if (!m_MbsSessionId.encode(ie->value.choice.MBS_SessionID)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_MulticastSessionUpdateRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
}

//------------------------------------------------------------------------------
bool MulticastSessionUpdateRequestMsg::getMbsSessionId(
    MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
void MulticastSessionUpdateRequestMsg::setMbsAreaSessionId(
    const MbsAreaSessionId& v) {
  m_MbsAreaSessionId = std::make_optional<MbsAreaSessionId>(v);

  Ngap_MulticastSessionUpdateRequestIEs_t* ie =
      (Ngap_MulticastSessionUpdateRequestIEs_t*) calloc(
          1, sizeof(Ngap_MulticastSessionUpdateRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MBS_AreaSessionID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_MulticastSessionUpdateRequestIEs__value_PR_MBS_AreaSessionID;

  if (!m_MbsAreaSessionId.value().encode(
          ie->value.choice.MBS_AreaSessionID)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-AreaSessionID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_MulticastSessionUpdateRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-AreaSessionID IE error");
}

//------------------------------------------------------------------------------
bool MulticastSessionUpdateRequestMsg::getMbsAreaSessionId(
    MbsAreaSessionId& v) const {
  if (!m_MbsAreaSessionId.has_value()) return false;
  v = m_MbsAreaSessionId.value();
  return true;
}

//------------------------------------------------------------------------------
void MulticastSessionUpdateRequestMsg::setMulticastSessionUpdateRequestTransfer(
    const MulticastSessionUpdateRequestTransfer& v) {
  uint8_t buf[512] = {};
  int len          = v.encode(buf, sizeof(buf));
  if (len <= 0) {
    oai::logger::logger_common::ngap().error(
        "Encode MulticastSessionUpdateRequestTransfer error");
    return;
  }

  Ngap_MulticastSessionUpdateRequestIEs_t* ie =
      (Ngap_MulticastSessionUpdateRequestIEs_t*) calloc(
          1, sizeof(Ngap_MulticastSessionUpdateRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MulticastSessionUpdateRequestTransfer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_MulticastSessionUpdateRequestIEs__value_PR_OCTET_STRING_CONTAINING_MulticastSessionUpdateRequestTransfer_;

  OCTET_STRING_fromBuf(
      &ie->value.choice
           .OCTET_STRING_CONTAINING_MulticastSessionUpdateRequestTransfer_,
      (const char*) buf, len);

  int ret = ASN_SEQUENCE_ADD(
      &m_MulticastSessionUpdateRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MulticastSessionUpdateRequestTransfer IE error");
}

//------------------------------------------------------------------------------
bool MulticastSessionUpdateRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage &&
      ngapPdu->choice.initiatingMessage &&
      ngapPdu->choice.initiatingMessage->procedureCode ==
          Ngap_ProcedureCode_id_MulticastSessionUpdate &&
      ngapPdu->choice.initiatingMessage->value.present ==
          Ngap_InitiatingMessage__value_PR_MulticastSessionUpdateRequest) {
    m_MulticastSessionUpdateRequestIes =
        &ngapPdu->choice.initiatingMessage->value.choice
             .MulticastSessionUpdateRequest;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check MulticastSessionUpdateRequest message error");
    return false;
  }

  for (int i = 0;
       i < m_MulticastSessionUpdateRequestIes->protocolIEs->list.count;
       i++) {
    Ngap_MulticastSessionUpdateRequestIEs_t* ie =
        (Ngap_MulticastSessionUpdateRequestIEs_t*)
            m_MulticastSessionUpdateRequestIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_MulticastSessionUpdateRequestIEs__value_PR_MBS_SessionID) {
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
                Ngap_MulticastSessionUpdateRequestIEs__value_PR_MBS_AreaSessionID) {
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
      case Ngap_ProtocolIE_ID_id_MulticastSessionUpdateRequestTransfer: {
        // Transfer as OCTET_STRING; decoded by application layer.
      } break;
      default: {
        break;
      }
    }
  }
  return true;
}

}  // namespace oai::ngap
