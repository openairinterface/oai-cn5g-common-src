/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "PathSwitchRequestAck.hpp"

#include "logger_base.hpp"
extern "C" {
#include "Ngap_PathSwitchRequestAcknowledge.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_ProtocolIE-ID.h"
#include "Ngap_ProtocolIE_Container_compat.h"
}
#include "ngap_utils.hpp"
#include "utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
PathSwitchRequestAckMsg::PathSwitchRequestAckMsg() : NgapUeMessage() {
  m_PathSwitchRequestAckIes                = nullptr;
  m_ManagementBasedMdtPlmnList             = std::nullopt;
  m_ManagementBasedMdtPlmnModificationList = std::nullopt;
  m_TimeSynchronisationAssistanceInfo      = std::nullopt;
  m_FiveGProSeAuthorized                   = std::nullopt;
  m_FiveGProSeUePC5AggregateMaximumBitRate = std::nullopt;
  m_FiveGProSePC5QoSParameters             = std::nullopt;
  m_IabAuthorized                          = std::nullopt;
  setMessageType(NgapMessageType::PATH_SWITCH_REQUEST_ACKNOWLEDGE);
  initialize();
}

//------------------------------------------------------------------------------
PathSwitchRequestAckMsg::~PathSwitchRequestAckMsg() {}

//------------------------------------------------------------------------------
void PathSwitchRequestAckMsg::initialize() {
  m_PathSwitchRequestAckIes = &(ngapPdu->choice.successfulOutcome->value.choice
                                    .PathSwitchRequestAcknowledge);
  if (!m_PathSwitchRequestAckIes->protocolIEs) {
    m_PathSwitchRequestAckIes->protocolIEs =
        (struct Ngap_ProtocolIE_Container*) calloc(
            1, sizeof(struct Ngap_ProtocolIE_Container));
  }
}

//------------------------------------------------------------------------------
void PathSwitchRequestAckMsg::setAmfUeNgapId(const uint64_t& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_PathSwitchRequestAcknowledgeIEs_t* ie =
      (Ngap_PathSwitchRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_PathSwitchRequestAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PathSwitchRequestAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP AMF_UE_NGAP_ID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_PathSwitchRequestAckIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PathSwitchRequestAckMsg::setRanUeNgapId(const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_PathSwitchRequestAcknowledgeIEs_t* ie =
      (Ngap_PathSwitchRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_PathSwitchRequestAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PathSwitchRequestAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RAN_UE_NGAP_ID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_PathSwitchRequestAckIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
bool PathSwitchRequestAckMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  if (!ngapMsgPdu) return false;
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_PathSwitchRequest &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_PathSwitchRequestAcknowledge) {
      m_PathSwitchRequestAckIes = &ngapPdu->choice.successfulOutcome->value
                                       .choice.PathSwitchRequestAcknowledge;
    } else {
      oai::logger::logger_common::ngap().error(
          "Check PathSwitchRequestAck message error!");
      return false;
    }
  } else {
    oai::logger::logger_common::ngap().error(
        "PathSwitchRequestAck MessageType error!");
    return false;
  }

  for (int i = 0; i < m_PathSwitchRequestAckIes->protocolIEs->list.count; i++) {
    Ngap_PathSwitchRequestAcknowledgeIEs_t* ngap_ie =
        (Ngap_PathSwitchRequestAcknowledgeIEs_t*)
            m_PathSwitchRequestAckIes->protocolIEs->list.array[i];
    switch (ngap_ie->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ngap_ie->criticality == Ngap_Criticality_reject &&
            ngap_ie->value.present ==
                Ngap_PathSwitchRequestAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID) {
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
                Ngap_PathSwitchRequestAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID) {
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
      case Ngap_ProtocolIE_ID_id_UESecurityCapabilities: {
        // TODO: decode UESecurityCapabilities
      } break;
      case Ngap_ProtocolIE_ID_id_SecurityContext: {
        // TODO: decode SecurityContext (Mandatory)
      } break;
      case Ngap_ProtocolIE_ID_id_NewSecurityContextInd: {
        // TODO: decode NewSecurityContextInd (Optional)
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceSwitchedList: {
        // TODO: decode PDUSessionResourceSwitchedList (Mandatory)
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceReleasedListPSAck: {
        // TODO: decode PDUSessionResourceReleasedListPSAck (Optional)
      } break;
      case Ngap_ProtocolIE_ID_id_AllowedNSSAI: {
        // TODO: decode AllowedNSSAI (Optional)
      } break;
      case Ngap_ProtocolIE_ID_id_CoreNetworkAssistanceInformationForInactive: {
        // TODO: decode CoreNetworkAssistanceInformationForInactive (Optional)
      } break;
      case Ngap_ProtocolIE_ID_id_RRCInactiveTransitionReportRequest: {
        // TODO: decode RRCInactiveTransitionReportRequest (Optional)
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

//------------------------------------------------------------------------------
void PathSwitchRequestAckMsg::setManagementBasedMdtPlmnList(
    const ManagementBasedMdtPlmnList& value) {
  m_ManagementBasedMdtPlmnList =
      std::make_optional<ManagementBasedMdtPlmnList>(value);
  Ngap_PathSwitchRequestAcknowledgeIEs_t* ie =
      (Ngap_PathSwitchRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_PathSwitchRequestAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_ManagementBasedMDTPLMNList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PathSwitchRequestAcknowledgeIEs__value_PR_MDTPLMNList;
  if (!m_ManagementBasedMdtPlmnList.value().encode(
          ie->value.choice.MDTPLMNList)) {
    oai::logger::logger_common::ngap().error(
        "Encode ManagementBasedMdtPlmnList IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(&m_PathSwitchRequestAckIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode ManagementBasedMdtPlmnList IE error");
}

//------------------------------------------------------------------------------
void PathSwitchRequestAckMsg::setManagementBasedMdtPlmnModificationList(
    const ManagementBasedMdtPlmnModificationList& value) {
  m_ManagementBasedMdtPlmnModificationList =
      std::make_optional<ManagementBasedMdtPlmnModificationList>(value);
  Ngap_PathSwitchRequestAcknowledgeIEs_t* ie =
      (Ngap_PathSwitchRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_PathSwitchRequestAcknowledgeIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_ManagementBasedMDTPLMNModificationList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PathSwitchRequestAcknowledgeIEs__value_PR_MDTPLMNModificationList;
  if (!m_ManagementBasedMdtPlmnModificationList.value().encode(
          ie->value.choice.MDTPLMNModificationList)) {
    oai::logger::logger_common::ngap().error(
        "Encode ManagementBasedMdtPlmnModificationList IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(&m_PathSwitchRequestAckIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode ManagementBasedMdtPlmnModificationList IE error");
}

//------------------------------------------------------------------------------
void PathSwitchRequestAckMsg::setTimeSynchronisationAssistanceInfo(
    const TimeSynchronisationAssistanceInfo& value) {
  m_TimeSynchronisationAssistanceInfo =
      std::make_optional<TimeSynchronisationAssistanceInfo>(value);
  Ngap_PathSwitchRequestAcknowledgeIEs_t* ie =
      (Ngap_PathSwitchRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_PathSwitchRequestAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_TimeSyncAssistanceInfo;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PathSwitchRequestAcknowledgeIEs__value_PR_TimeSyncAssistanceInfo;
  if (!m_TimeSynchronisationAssistanceInfo.value().encode(
          ie->value.choice.TimeSyncAssistanceInfo)) {
    oai::logger::logger_common::ngap().error(
        "Encode TimeSynchronisationAssistanceInfo IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(&m_PathSwitchRequestAckIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode TimeSynchronisationAssistanceInfo IE error");
}

//------------------------------------------------------------------------------
void PathSwitchRequestAckMsg::setFiveGProSeAuthorized(
    const FiveGProSeAuthorized& value) {
  m_FiveGProSeAuthorized = std::make_optional<FiveGProSeAuthorized>(value);
  Ngap_PathSwitchRequestAcknowledgeIEs_t* ie =
      (Ngap_PathSwitchRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_PathSwitchRequestAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_FiveG_ProSeAuthorized;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PathSwitchRequestAcknowledgeIEs__value_PR_FiveG_ProSeAuthorized;
  if (!m_FiveGProSeAuthorized.value().encode(
          ie->value.choice.FiveG_ProSeAuthorized)) {
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSeAuthorized IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(&m_PathSwitchRequestAckIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSeAuthorized IE error");
}

//------------------------------------------------------------------------------
void PathSwitchRequestAckMsg::setFiveGProSeUePC5AggregateMaximumBitRate(
    const FiveGProSeUePC5AggregateMaximumBitRate& value) {
  m_FiveGProSeUePC5AggregateMaximumBitRate =
      std::make_optional<FiveGProSeUePC5AggregateMaximumBitRate>(value);
  Ngap_PathSwitchRequestAcknowledgeIEs_t* ie =
      (Ngap_PathSwitchRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_PathSwitchRequestAcknowledgeIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_FiveG_ProSeUEPC5AggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PathSwitchRequestAcknowledgeIEs__value_PR_NRUESidelinkAggregateMaximumBitrate_1;
  if (!m_FiveGProSeUePC5AggregateMaximumBitRate.value().encode(
          ie->value.choice.NRUESidelinkAggregateMaximumBitrate_1)) {
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSeUePC5AggregateMaximumBitRate IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(&m_PathSwitchRequestAckIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSeUePC5AggregateMaximumBitRate IE error");
}

//------------------------------------------------------------------------------
void PathSwitchRequestAckMsg::setFiveGProSePC5QoSParameters(
    const FiveGProSePC5QoSParameters& value) {
  m_FiveGProSePC5QoSParameters =
      std::make_optional<FiveGProSePC5QoSParameters>(value);
  Ngap_PathSwitchRequestAcknowledgeIEs_t* ie =
      (Ngap_PathSwitchRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_PathSwitchRequestAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_FiveG_ProSePC5QoSParameters;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PathSwitchRequestAcknowledgeIEs__value_PR_FiveG_ProSePC5QoSParameters;
  if (!m_FiveGProSePC5QoSParameters.value().encode(
          ie->value.choice.FiveG_ProSePC5QoSParameters)) {
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSePC5QoSParameters IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(&m_PathSwitchRequestAckIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSePC5QoSParameters IE error");
}

//------------------------------------------------------------------------------
void PathSwitchRequestAckMsg::setIabAuthorized(const IabAuthorized& value) {
  m_IabAuthorized = std::make_optional<IabAuthorized>(value);
  Ngap_PathSwitchRequestAcknowledgeIEs_t* ie =
      (Ngap_PathSwitchRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_PathSwitchRequestAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_IAB_Authorized;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PathSwitchRequestAcknowledgeIEs__value_PR_IAB_Authorized;
  if (!m_IabAuthorized.value().encode(ie->value.choice.IAB_Authorized)) {
    oai::logger::logger_common::ngap().error("Encode IabAuthorized IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(&m_PathSwitchRequestAckIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode IabAuthorized IE error");
}

}  // namespace oai::ngap
