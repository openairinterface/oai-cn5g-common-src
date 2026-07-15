/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MulticastGroupPaging.hpp"

#include "logger_base.hpp"
#include "utils.hpp"

extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
MulticastGroupPagingMsg::MulticastGroupPagingMsg() : NgapMessage() {
  m_MulticastGroupPagingIes = nullptr;
  m_MbsServiceArea          = std::nullopt;

  NgapMessage::setMessageType(NgapMessageType::MULTICAST_GROUP_PAGING);
  initialize();
}

//------------------------------------------------------------------------------
void MulticastGroupPagingMsg::initialize() {
  m_MulticastGroupPagingIes =
      &(ngapPdu->choice.initiatingMessage->value.choice.MulticastGroupPaging);
}

//------------------------------------------------------------------------------
void MulticastGroupPagingMsg::setMbsSessionId(const MbsSessionId& v) {
  m_MbsSessionId = v;

  Ngap_MulticastGroupPagingIEs_t* ie = (Ngap_MulticastGroupPagingIEs_t*) calloc(
      1, sizeof(Ngap_MulticastGroupPagingIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_MBS_SessionID;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_MulticastGroupPagingIEs__value_PR_MBS_SessionID;

  if (!m_MbsSessionId.encode(ie->value.choice.MBS_SessionID)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_MulticastGroupPagingIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-SessionID IE error");
}

//------------------------------------------------------------------------------
bool MulticastGroupPagingMsg::getMbsSessionId(MbsSessionId& v) const {
  v = m_MbsSessionId;
  return true;
}

//------------------------------------------------------------------------------
void MulticastGroupPagingMsg::setMbsServiceArea(const MbsServiceArea& v) {
  m_MbsServiceArea = std::make_optional<MbsServiceArea>(v);

  Ngap_MulticastGroupPagingIEs_t* ie = (Ngap_MulticastGroupPagingIEs_t*) calloc(
      1, sizeof(Ngap_MulticastGroupPagingIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_MBS_ServiceArea;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_MulticastGroupPagingIEs__value_PR_MBS_ServiceArea;

  if (!m_MbsServiceArea.value().encode(ie->value.choice.MBS_ServiceArea)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-ServiceArea IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_MulticastGroupPagingIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP MBS-ServiceArea IE error");
}

//------------------------------------------------------------------------------
bool MulticastGroupPagingMsg::getMbsServiceArea(MbsServiceArea& v) const {
  if (!m_MbsServiceArea.has_value()) return false;
  v = m_MbsServiceArea.value();
  return true;
}

//------------------------------------------------------------------------------
bool MulticastGroupPagingMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage &&
      ngapPdu->choice.initiatingMessage &&
      ngapPdu->choice.initiatingMessage->procedureCode ==
          Ngap_ProcedureCode_id_MulticastGroupPaging &&
      ngapPdu->choice.initiatingMessage->value.present ==
          Ngap_InitiatingMessage__value_PR_MulticastGroupPaging) {
    m_MulticastGroupPagingIes =
        &ngapPdu->choice.initiatingMessage->value.choice.MulticastGroupPaging;
  } else {
    oai::logger::logger_common::ngap().error(
        "Check MulticastGroupPaging message error");
    return false;
  }

  for (int i = 0; i < m_MulticastGroupPagingIes->protocolIEs->list.count; i++) {
    Ngap_MulticastGroupPagingIEs_t* ie =
        (Ngap_MulticastGroupPagingIEs_t*)
            m_MulticastGroupPagingIes->protocolIEs->list.array[i];
    switch (ie->id) {
      case Ngap_ProtocolIE_ID_id_MBS_SessionID: {
        if (ie->criticality == Ngap_Criticality_ignore &&
            ie->value.present ==
                Ngap_MulticastGroupPagingIEs__value_PR_MBS_SessionID) {
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
        if (ie->criticality == Ngap_Criticality_ignore &&
            ie->value.present ==
                Ngap_MulticastGroupPagingIEs__value_PR_MBS_ServiceArea) {
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
      default: {
        break;
      }
    }
  }
  return true;
}

}  // namespace oai::ngap
