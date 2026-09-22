/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "UeContextModificationRequest.hpp"

#include "logger_base.hpp"
extern "C" {
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_ProtocolIE-ID.h"
#include "Ngap_ProtocolIE_Container_compat.h"
#include "Ngap_UEContextModificationRequest.h"
}
#include "ngap_utils.hpp"
#include "utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
UeContextModificationRequestMsg::UeContextModificationRequestMsg()
    : NgapUeMessage() {
  m_UeContextModificationRequestIes = nullptr;
  setMessageType(NgapMessageType::UE_CONTEXT_MODIFICATION_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
UeContextModificationRequestMsg::~UeContextModificationRequestMsg() {}

//------------------------------------------------------------------------------
void UeContextModificationRequestMsg::initialize() {
  m_UeContextModificationRequestIes = &ngapPdu->choice.initiatingMessage->value
                                           .choice.UEContextModificationRequest;
  if (!m_UeContextModificationRequestIes->protocolIEs) {
    m_UeContextModificationRequestIes->protocolIEs =
        (struct Ngap_ProtocolIE_Container*) calloc(
            1, sizeof(struct Ngap_ProtocolIE_Container));
  }
}

//------------------------------------------------------------------------------
void UeContextModificationRequestMsg::setAmfUeNgapId(const uint64_t& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP AMF_UE_NGAP_ID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UeContextModificationRequestMsg::setRanUeNgapId(
    const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RAN_UE_NGAP_ID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
bool UeContextModificationRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  if (!ngapMsgPdu) return false;
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_UEContextModification &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_UEContextModificationRequest) {
      m_UeContextModificationRequestIes =
          &ngapPdu->choice.initiatingMessage->value.choice
               .UEContextModificationRequest;
    } else {
      oai::logger::logger_common::ngap().error(
          "Check UEContextModificationRequest message error!");
      return false;
    }
  } else {
    oai::logger::logger_common::ngap().error(
        "UEContextModificationRequest MessageType error!");
    return false;
  }

  for (int i = 0;
       i < m_UeContextModificationRequestIes->protocolIEs->list.count; i++) {
    Ngap_UEContextModificationRequestIEs_t* ngap_ie =
        (Ngap_UEContextModificationRequestIEs_t*)
            m_UeContextModificationRequestIes->protocolIEs->list.array[i];
    switch (ngap_ie->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ngap_ie->criticality == Ngap_Criticality_reject &&
            ngap_ie->value.present ==
                Ngap_UEContextModificationRequestIEs__value_PR_AMF_UE_NGAP_ID) {
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
                Ngap_UEContextModificationRequestIEs__value_PR_RAN_UE_NGAP_ID) {
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
      case Ngap_ProtocolIE_ID_id_RANPagingPriority: {
        // decode RANPagingPriority (Optional) — not yet implemented
      } break;
      case Ngap_ProtocolIE_ID_id_SecurityKey: {
        // decode SecurityKey (Optional) — not yet implemented
      } break;
      case Ngap_ProtocolIE_ID_id_IndexToRFSP: {
        // decode IndexToRFSP (Optional) — not yet implemented
      } break;
      case Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate: {
        // decode UEAggregateMaximumBitRate (Optional) — not yet implemented
      } break;
      case Ngap_ProtocolIE_ID_id_UESecurityCapabilities: {
        // decode UESecurityCapabilities (Optional) — not yet implemented
      } break;
      case Ngap_ProtocolIE_ID_id_CoreNetworkAssistanceInformationForInactive: {
        // decode CoreNetworkAssistanceInformationForInactive (Optional) — not
        // yet implemented
      } break;
      case Ngap_ProtocolIE_ID_id_EmergencyFallbackIndicator: {
        // decode EmergencyFallbackIndicator (Optional) — not yet implemented
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
bool UeContextModificationRequestMsg::setNewAmfUeNgapId(const uint64_t& id) {
  AmfUeNgapId newAmfUeNgapId = {};
  newAmfUeNgapId.set(id);

  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NewAMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_AMF_UE_NGAP_ID_1;

  if (!newAmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID_1)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP NewAMF_UE_NGAP_ID IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return false;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP NewAMF_UE_NGAP_ID IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UeContextModificationRequestMsg::setRanPagingPriority(
    const uint32_t& priority) {
  RanPagingPriority ranPagingPriority = {};
  if (!ranPagingPriority.set(priority)) {
    oai::logger::logger_common::ngap().error(
        "Set NGAP RANPagingPriority value error");
    return false;
  }

  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RANPagingPriority;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_RANPagingPriority;

  if (!ranPagingPriority.encode(ie->value.choice.RANPagingPriority)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RANPagingPriority IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return false;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RANPagingPriority IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UeContextModificationRequestMsg::setSecurityKey(
    uint8_t* buffer, const size_t& size) {
  SecurityKey securityKey = {};
  securityKey.set(buffer, size);

  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_SecurityKey;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_SecurityKey;

  if (!securityKey.encode(ie->value.choice.SecurityKey)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP SecurityKey IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return false;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP SecurityKey IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UeContextModificationRequestMsg::setIndexToRfsp(const uint32_t& index) {
  IndexToRfsp indexToRfsp(index);

  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_IndexToRFSP;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_IndexToRFSP;

  if (!indexToRfsp.encode(ie->value.choice.IndexToRFSP)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP IndexToRFSP IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return false;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP IndexToRFSP IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UeContextModificationRequestMsg::setUeAggregateMaximumBitRate(
    const uint64_t& dl, const uint64_t& ul) {
  UeAggregateMaxBitRate ueAmbrIe = {};
  ueAmbrIe.set(dl, ul);

  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_UEAggregateMaximumBitRate;

  if (!ueAmbrIe.encode(ie->value.choice.UEAggregateMaximumBitRate)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP UEAggregateMaximumBitRate IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return false;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP UEAggregateMaximumBitRate IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UeContextModificationRequestMsg::setUeSecurityCapabilities(
    uint16_t nrEncryptionAlgs, uint16_t nrIntegrityProtectionAlgs,
    uint16_t eutraEncryptionAlgs, uint16_t eutraIntegrityProtectionAlgs) {
  UeSecurityCapabilities ueSec = {};
  ueSec.set(
      nrEncryptionAlgs, nrIntegrityProtectionAlgs, eutraEncryptionAlgs,
      eutraIntegrityProtectionAlgs);

  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UESecurityCapabilities;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_UESecurityCapabilities;

  if (!ueSec.encode(ie->value.choice.UESecurityCapabilities)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP UESecurityCapabilities IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return false;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP UESecurityCapabilities IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UeContextModificationRequestMsg::setCoreNetworkAssistanceInformation(
    const CoreNetworkAssistanceInformationForInactive& info) {
  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_CoreNetworkAssistanceInformationForInactive;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_CoreNetworkAssistanceInformationForInactive;

  ie->value.choice.CoreNetworkAssistanceInformationForInactive =
      (Ngap_CoreNetworkAssistanceInformationForInactive_t*) calloc(
          1, sizeof(Ngap_CoreNetworkAssistanceInformationForInactive_t));
  if (!info.encode(
          *ie->value.choice.CoreNetworkAssistanceInformationForInactive)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP CoreNetworkAssistanceInformationForInactive IE error");
    oai::utils::utils::free_wrapper(
        (void**) &ie->value.choice.CoreNetworkAssistanceInformationForInactive);
    oai::utils::utils::free_wrapper((void**) &ie);
    return false;
  }

  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP CoreNetworkAssistanceInformationForInactive IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UeContextModificationRequestMsg::setEmergencyFallbackIndicator(
    const Ngap_EmergencyFallbackIndicator_t& indicator) {
  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_EmergencyFallbackIndicator;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_EmergencyFallbackIndicator;

  ie->value.choice.EmergencyFallbackIndicator = indicator;

  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP EmergencyFallbackIndicator IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void UeContextModificationRequestMsg::setTimeSynchronisationAssistanceInfo(
    const TimeSynchronisationAssistanceInfo& value) {
  m_TimeSynchronisationAssistanceInfo =
      std::make_optional<TimeSynchronisationAssistanceInfo>(value);
  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_TimeSyncAssistanceInfo;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_TimeSyncAssistanceInfo;
  if (!m_TimeSynchronisationAssistanceInfo.value().encode(
          ie->value.choice.TimeSyncAssistanceInfo)) {
    oai::logger::logger_common::ngap().error(
        "Encode TimeSynchronisationAssistanceInfo IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode TimeSynchronisationAssistanceInfo IE error");
}

//------------------------------------------------------------------------------
void UeContextModificationRequestMsg::setFiveGProSeAuthorized(
    const FiveGProSeAuthorized& value) {
  m_FiveGProSeAuthorized = std::make_optional<FiveGProSeAuthorized>(value);
  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_FiveG_ProSeAuthorized;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_FiveG_ProSeAuthorized;
  if (!m_FiveGProSeAuthorized.value().encode(
          ie->value.choice.FiveG_ProSeAuthorized)) {
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSeAuthorized IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSeAuthorized IE error");
}

//------------------------------------------------------------------------------
void UeContextModificationRequestMsg::setFiveGProSeUePC5AggregateMaximumBitRate(
    const FiveGProSeUePC5AggregateMaximumBitRate& value) {
  m_FiveGProSeUePC5AggregateMaximumBitRate =
      std::make_optional<FiveGProSeUePC5AggregateMaximumBitRate>(value);
  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_FiveG_ProSeUEPC5AggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_NRUESidelinkAggregateMaximumBitrate_1;
  if (!m_FiveGProSeUePC5AggregateMaximumBitRate.value().encode(
          ie->value.choice.NRUESidelinkAggregateMaximumBitrate_1)) {
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSeUePC5AggregateMaximumBitRate IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSeUePC5AggregateMaximumBitRate IE error");
}

//------------------------------------------------------------------------------
void UeContextModificationRequestMsg::setFiveGProSePC5QoSParameters(
    const FiveGProSePC5QoSParameters& value) {
  m_FiveGProSePC5QoSParameters =
      std::make_optional<FiveGProSePC5QoSParameters>(value);
  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_FiveG_ProSePC5QoSParameters;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_FiveG_ProSePC5QoSParameters;
  if (!m_FiveGProSePC5QoSParameters.value().encode(
          ie->value.choice.FiveG_ProSePC5QoSParameters)) {
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSePC5QoSParameters IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSePC5QoSParameters IE error");
}

//------------------------------------------------------------------------------
void UeContextModificationRequestMsg::setUeSliceMaximumBitRateList(
    const UeSliceMaximumBitRateList& value) {
  m_UeSliceMaximumBitRateList =
      std::make_optional<UeSliceMaximumBitRateList>(value);
  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UESliceMaximumBitRateList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_UESliceMaximumBitRateList;
  if (!m_UeSliceMaximumBitRateList.value().encode(
          ie->value.choice.UESliceMaximumBitRateList)) {
    oai::logger::logger_common::ngap().error(
        "Encode UeSliceMaximumBitRateList IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode UeSliceMaximumBitRateList IE error");
}

//------------------------------------------------------------------------------
void UeContextModificationRequestMsg::setQmcDeactivation(
    const QmcDeactivation& value) {
  m_QmcDeactivation = std::make_optional<QmcDeactivation>(value);
  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_QMCDeactivation;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_QMCDeactivation;
  if (!m_QmcDeactivation.value().encode(ie->value.choice.QMCDeactivation)) {
    oai::logger::logger_common::ngap().error("Encode QmcDeactivation IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode QmcDeactivation IE error");
}

//------------------------------------------------------------------------------
void UeContextModificationRequestMsg::setManagementBasedMdtPlmnModificationList(
    const ManagementBasedMdtPlmnModificationList& value) {
  m_ManagementBasedMdtPlmnModificationList =
      std::make_optional<ManagementBasedMdtPlmnModificationList>(value);
  Ngap_UEContextModificationRequestIEs_t* ie =
      (Ngap_UEContextModificationRequestIEs_t*) calloc(
          1, sizeof(Ngap_UEContextModificationRequestIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_ManagementBasedMDTPLMNModificationList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UEContextModificationRequestIEs__value_PR_MDTPLMNModificationList;
  if (!m_ManagementBasedMdtPlmnModificationList.value().encode(
          ie->value.choice.MDTPLMNModificationList)) {
    oai::logger::logger_common::ngap().error(
        "Encode ManagementBasedMdtPlmnModificationList IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_UeContextModificationRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode ManagementBasedMdtPlmnModificationList IE error");
}

}  // namespace oai::ngap
