/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "PathSwitchRequest.hpp"

#include "logger_base.hpp"
extern "C" {
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_ProtocolIE-ID.h"
#include "Ngap_ProtocolIE_Container_compat.h"
}
#include "ngap_utils.hpp"
#include "utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
PathSwitchRequestMsg::PathSwitchRequestMsg() : NgapUeMessage() {
  m_PathSwitchRequestIes = nullptr;
  m_RedCapIndication     = std::nullopt;
  setMessageType(NgapMessageType::PATH_SWITCH_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
PathSwitchRequestMsg::~PathSwitchRequestMsg() {}

//------------------------------------------------------------------------------
void PathSwitchRequestMsg::initialize() {
  m_PathSwitchRequestIes =
      &ngapPdu->choice.initiatingMessage->value.choice.PathSwitchRequest;
  if (!m_PathSwitchRequestIes->protocolIEs) {
    m_PathSwitchRequestIes->protocolIEs =
        (struct Ngap_ProtocolIE_Container*) calloc(
            1, sizeof(struct Ngap_ProtocolIE_Container));
  }
}

//------------------------------------------------------------------------------
void PathSwitchRequestMsg::setAmfUeNgapId(const uint64_t& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_PathSwitchRequestIEs_t* ie = (Ngap_PathSwitchRequestIEs_t*) calloc(
      1, sizeof(Ngap_PathSwitchRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_PathSwitchRequestIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP AMF_UE_NGAP_ID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_PathSwitchRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PathSwitchRequestMsg::setRanUeNgapId(const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_PathSwitchRequestIEs_t* ie = (Ngap_PathSwitchRequestIEs_t*) calloc(
      1, sizeof(Ngap_PathSwitchRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_PathSwitchRequestIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RAN_UE_NGAP_ID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_PathSwitchRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
bool PathSwitchRequestMsg::getUserLocationInformation(
    UserLocationInformation& userLocationInformation) const {
  userLocationInformation = m_UserLocationInformation;
  return true;
}

//------------------------------------------------------------------------------
bool PathSwitchRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  if (!ngapMsgPdu) return false;
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_PathSwitchRequest &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_PathSwitchRequest) {
      m_PathSwitchRequestIes =
          &ngapPdu->choice.initiatingMessage->value.choice.PathSwitchRequest;
    } else {
      oai::logger::logger_common::ngap().error(
          "Check PathSwitchRequest message error!");
      return false;
    }
  } else {
    oai::logger::logger_common::ngap().error(
        "PathSwitchRequest MessageType error!");
    return false;
  }

  for (int i = 0; i < m_PathSwitchRequestIes->protocolIEs->list.count; i++) {
    Ngap_PathSwitchRequestIEs_t* ngap_ie =
        (Ngap_PathSwitchRequestIEs_t*)
            m_PathSwitchRequestIes->protocolIEs->list.array[i];
    switch (ngap_ie->id) {
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (ngap_ie->criticality == Ngap_Criticality_reject &&
            ngap_ie->value.present ==
                Ngap_PathSwitchRequestIEs__value_PR_RAN_UE_NGAP_ID) {
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
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ngap_ie->criticality == Ngap_Criticality_reject &&
            ngap_ie->value.present ==
                Ngap_PathSwitchRequestIEs__value_PR_AMF_UE_NGAP_ID) {
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
      case Ngap_ProtocolIE_ID_id_UserLocationInformation: {
        if (ngap_ie->value.present ==
            Ngap_PathSwitchRequestIEs__value_PR_UserLocationInformation) {
          if (!ngap_ie->value.choice.UserLocationInformation) break;
          if (!m_UserLocationInformation.decode(
                  *ngap_ie->value.choice.UserLocationInformation)) {
            oai::logger::logger_common::ngap().error(
                "Decode NGAP UserLocationInformation IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decode NGAP UserLocationInformation IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UESecurityCapabilities: {
        // TODO: decode UESecurityCapabilities
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceToBeSwitchedDLList: {
        // TODO: decode PDUSessionResourceToBeSwitchedDLList
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListPSReq: {
        // TODO: decode PDUSessionResourceFailedToSetupListPSReq (Optional)
      } break;
      case Ngap_ProtocolIE_ID_id_RRCEstablishmentCause: {
        // TODO: decode RRCEstablishmentCause (Optional)
      } break;
      case Ngap_ProtocolIE_ID_id_RedCapIndication: {
        if (ngap_ie->value.present ==
            Ngap_PathSwitchRequestIEs__value_PR_RedCapIndication) {
          RedCapIndication tmp{};
          if (!tmp.decode(ngap_ie->value.choice.RedCapIndication)) {
            oai::logger::logger_common::ngap().error(
                "Decode NGAP RedCapIndication IE error");
            return false;
          }
          m_RedCapIndication = std::make_optional<RedCapIndication>(tmp);
        }
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

//------------------------------------------------------------------------------
bool PathSwitchRequestMsg::getRedCapIndication(RedCapIndication& value) const {
  if (!m_RedCapIndication.has_value()) return false;
  value = m_RedCapIndication.value();
  return true;
}

}  // namespace oai::ngap
