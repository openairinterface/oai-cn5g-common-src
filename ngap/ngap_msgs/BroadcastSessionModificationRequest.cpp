/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "BroadcastSessionModificationRequest.hpp"

#include "logger_base.hpp"
#include "utils.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
BroadcastSessionModificationRequestMsg::
    BroadcastSessionModificationRequestMsg()
    : NgapMessage() {
  m_BroadcastSessionModificationRequestIes = nullptr;
  m_MbsServiceArea                         = std::nullopt;

  NgapMessage::setMessageType(
      NgapMessageType::BROADCAST_SESSION_MODIFICATION_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
void BroadcastSessionModificationRequestMsg::initialize() {
  m_BroadcastSessionModificationRequestIes =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .BroadcastSessionModificationRequest);
}

//------------------------------------------------------------------------------
void BroadcastSessionModificationRequestMsg::setMbsSessionId(
    const MbsSessionId& v) {
  m_MbsSessionId = v;

  Ngap_BroadcastSessionModificationRequestIEs_t* ie =
      (Ngap_BroadcastSessionModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_BroadcastSessionModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MBS_SessionID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_BroadcastSessionModificationRequestIEs__value_PR_MBS_SessionID;

  if (!m_MbsSessionId.encode(ie->value.choice.MBS_SessionID)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_BroadcastSessionModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
}

//------------------------------------------------------------------------------
bool BroadcastSessionModificationRequestMsg::getMbsSessionId(
    MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
void BroadcastSessionModificationRequestMsg::setMbsServiceArea(
    const MbsServiceArea& v) {
  m_MbsServiceArea = std::make_optional<MbsServiceArea>(v);

  Ngap_BroadcastSessionModificationRequestIEs_t* ie =
      (Ngap_BroadcastSessionModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_BroadcastSessionModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MBS_ServiceArea;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_BroadcastSessionModificationRequestIEs__value_PR_MBS_ServiceArea;

  if (!m_MbsServiceArea.value().encode(ie->value.choice.MBS_ServiceArea)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-ServiceArea IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_BroadcastSessionModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-ServiceArea IE error");
}

//------------------------------------------------------------------------------
bool BroadcastSessionModificationRequestMsg::getMbsServiceArea(
    MbsServiceArea& v) const {
  if (!m_MbsServiceArea.has_value()) return false;
  v = m_MbsServiceArea.value();
  return true;
}

//------------------------------------------------------------------------------
void BroadcastSessionModificationRequestMsg::
    setMbsSessionModificationRequestTransfer(
        const MbsSessionSetupOrModificationRequestTransfer& v) {
  uint8_t buf[512] = {};
  int len          = v.encode(buf, sizeof(buf));
  if (len <= 0) {
    oai::logger::logger_common::ngap().error(
        "Encode MbsSessionModificationRequestTransfer error");
    return;
  }

  Ngap_BroadcastSessionModificationRequestIEs_t* ie =
      (Ngap_BroadcastSessionModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_BroadcastSessionModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MBSSessionModificationRequestTransfer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_BroadcastSessionModificationRequestIEs__value_PR_OCTET_STRING_CONTAINING_MBSSessionSetupOrModRequestTransfer_;

  OCTET_STRING_fromBuf(
      &ie->value.choice
           .OCTET_STRING_CONTAINING_MBSSessionSetupOrModRequestTransfer_,
      (const char*) buf, len);

  int ret = ASN_SEQUENCE_ADD(
      &m_BroadcastSessionModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBSSessionModificationRequestTransfer IE error");
}

//------------------------------------------------------------------------------
bool BroadcastSessionModificationRequestMsg::decode(
    Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage &&
      ngapPdu->choice.initiatingMessage &&
      ngapPdu->choice.initiatingMessage->procedureCode ==
          Ngap_ProcedureCode_id_BroadcastSessionModification &&
      ngapPdu->choice.initiatingMessage->value.present ==
          Ngap_InitiatingMessage__value_PR_BroadcastSessionModificationRequest) {
    m_BroadcastSessionModificationRequestIes =
        &ngapPdu->choice.initiatingMessage->value.choice
             .BroadcastSessionModificationRequest;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check BroadcastSessionModificationRequest message error");
    return false;
  }

  for (int i = 0;
       i < m_BroadcastSessionModificationRequestIes->protocolIEs->list.count;
       i++) {
    Ngap_BroadcastSessionModificationRequestIEs_t* ie =
        (Ngap_BroadcastSessionModificationRequestIEs_t*)
            m_BroadcastSessionModificationRequestIes->protocolIEs->list
                .array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_BroadcastSessionModificationRequestIEs__value_PR_MBS_SessionID) {
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
      case Ngap_ProtocolIE_ID_id_MBS_ServiceArea: {
        if (ie->criticality == Ngap_Criticality_reject &&
            ie->value.present ==
                Ngap_BroadcastSessionModificationRequestIEs__value_PR_MBS_ServiceArea) {
          m_MbsServiceArea = std::make_optional<MbsServiceArea>();
          if (!m_MbsServiceArea.value().decode(
                  ie->value.choice.MBS_ServiceArea)) {
            oai::logger::logger_common::ngap().error(
                "Decode NGAP MBS-ServiceArea IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decode NGAP MBS-ServiceArea IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_MBSSessionModificationRequestTransfer: {
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
