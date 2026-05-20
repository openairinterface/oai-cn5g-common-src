/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */
#include "HandoverNotifyMsg.hpp"

#include "logger_base.hpp"
extern "C" {
#include "Ngap_HandoverNotify.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_ProtocolIE-ID.h"
#include "Ngap_ProtocolIE_Container_compat.h"
}
#include "utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
HandoverNotifyMsg::HandoverNotifyMsg() : NgapUeMessage() {
  m_HandoverNotifyIes = nullptr;

  setMessageType(NgapMessageType::HANDOVER_NOTIFY);
  initialize();
}

//------------------------------------------------------------------------------
HandoverNotifyMsg::~HandoverNotifyMsg(){};

//------------------------------------------------------------------------------
void HandoverNotifyMsg::initialize() {
  m_HandoverNotifyIes =
      &(ngapPdu->choice.initiatingMessage->value.choice.HandoverNotify);
  m_NotifySourceNgRanNode = std::nullopt;
}

//------------------------------------------------------------------------------
void HandoverNotifyMsg::setAmfUeNgapId(const uint64_t& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_HandoverNotifyIEs_t* ie =
      (Ngap_HandoverNotifyIEs_t*) calloc(1, sizeof(Ngap_HandoverNotifyIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverNotifyIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_HandoverNotifyIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void HandoverNotifyMsg::setRanUeNgapId(const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_HandoverNotifyIEs_t* ie =
      (Ngap_HandoverNotifyIEs_t*) calloc(1, sizeof(Ngap_HandoverNotifyIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverNotifyIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_HandoverNotifyIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void HandoverNotifyMsg::setUserLocationInfoNr(
    const NrCgi_t& cig, const Tai_t& tai) {
  UserLocationInformationNr information_nr = {};
  NrCgi nR_CGI                             = {};
  nR_CGI.set(cig.mcc, cig.mnc, cig.nrCellId);
  Tai tai_nr = {};
  tai_nr.set(tai);
  information_nr.set(nR_CGI, tai_nr);
  m_UserLocationInformation.set(information_nr);

  Ngap_HandoverNotifyIEs_t* ie =
      (Ngap_HandoverNotifyIEs_t*) calloc(1, sizeof(Ngap_HandoverNotifyIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_HandoverNotifyIEs__value_PR_UserLocationInformation;

  if (!ie->value.choice.UserLocationInformation) ie->value.choice.UserLocationInformation = (Ngap_UserLocationInformation_t*) calloc(1, sizeof(Ngap_UserLocationInformation_t));
  int ret = m_UserLocationInformation.encode(
      *ie->value.choice.UserLocationInformation);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode UserLocationInformation IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_HandoverNotifyIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode UserLocationInformation IE error");
}

//------------------------------------------------------------------------------
bool HandoverNotifyMsg::getUserLocationInfoNr(NrCgi_t& cig, Tai_t& tai) const {
  UserLocationInformationNr information_nr = {};
  if (!m_UserLocationInformation.get(information_nr)) return false;

  if (m_UserLocationInformation.getChoiceOfUserLocationInformation() !=
      Ngap_UserLocationInformation_PR_userLocationInformationNR)
    return false;

  NrCgi nR_CGI = {};
  Tai nR_TAI   = {};
  information_nr.get(nR_CGI, nR_TAI);
  nR_CGI.get(cig);
  nR_TAI.get(tai);

  return true;
}

//------------------------------------------------------------------------------
bool HandoverNotifyMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  if (!ngapMsgPdu) return false;
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_HandoverNotification &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_HandoverNotify) {
      m_HandoverNotifyIes =
          &ngapPdu->choice.initiatingMessage->value.choice.HandoverNotify;
    } else {
      oai::logger::logger_common::ngap().error(
          "Check HandoverNotify message error!");
      return false;
    }
  } else {
    oai::logger::logger_common::ngap().error(
        "HandoverNotify MessageType error!");
    return false;
  }
  for (int i = 0; i < m_HandoverNotifyIes->protocolIEs->list.count; i++) {
    Ngap_HandoverNotifyIEs_t* ngap_ie =
        (Ngap_HandoverNotifyIEs_t*) m_HandoverNotifyIes->protocolIEs->list.array[i];
    switch (ngap_ie->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ngap_ie->criticality ==
                Ngap_Criticality_reject &&
            ngap_ie->value.present ==
                Ngap_HandoverNotifyIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!NgapUeMessage::m_AmfUeNgapId.decode(
                  ngap_ie
                      ->value.choice.AMF_UE_NGAP_ID)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (ngap_ie->criticality ==
                Ngap_Criticality_reject &&
            ngap_ie->value.present ==
                Ngap_HandoverNotifyIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!NgapUeMessage::m_RanUeNgapId.decode(
                  ngap_ie
                      ->value.choice.RAN_UE_NGAP_ID)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UserLocationInformation: {
        // TODO: Temporarily disable Criticality check to be tested with dsTest
        /*if (ngap_ie->criticality ==
              Ngap_Criticality_ignore &&
          ngap_ie->value.present ==
              Ngap_HandoverNotifyIEs__value_PR_UserLocationInformation) {
              */
        if (ngap_ie->value.present ==
            Ngap_HandoverNotifyIEs__value_PR_UserLocationInformation) {
          if (!ngap_ie->value.choice.UserLocationInformation) return false;
          if (!m_UserLocationInformation.decode(
                  *ngap_ie->value.choice.UserLocationInformation)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP UserLocationInformation IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP UserLocationInformation IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NotifySourceNGRANNode: {
        if (ngap_ie->value.present ==
            Ngap_HandoverNotifyIEs__value_PR_NotifySourceNGRANNode) {
          NotifySourceNgRanNode tmp{};
          if (tmp.decode(ngap_ie->value.choice.NotifySourceNGRANNode)) {
            m_NotifySourceNgRanNode =
                std::make_optional<NotifySourceNgRanNode>(tmp);
          } else {
            oai::logger::logger_common::ngap().warn(
                "Decoded NGAP NotifySourceNGRANNode IE error");
          }
        }
      } break;
      default: {
        oai::logger::logger_common::ngap().warn(
            "Not decoded IE %d",
            ngap_ie->id);
        break;
      }
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool HandoverNotifyMsg::getNotifySourceNgRanNode(
    NotifySourceNgRanNode& value) const {
  if (!m_NotifySourceNgRanNode.has_value()) return false;
  value = m_NotifySourceNgRanNode.value();
  return true;
}

}  // namespace oai::ngap
