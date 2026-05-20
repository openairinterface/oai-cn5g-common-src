/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "AmfConfigurationUpdateMsg.hpp"

#include "3gpp_23.003.h"
#include "logger_base.hpp"
extern "C" {
#include "Ngap_AMFConfigurationUpdate.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_ProtocolIE-ID.h"
#include "Ngap_ProtocolIE_Container_compat.h"
}
#include "utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
AmfConfigurationUpdateMsg::AmfConfigurationUpdateMsg() : NgapMessage() {
  m_AmfConfigurationUpdateIes = nullptr;
  NgapMessage::setMessageType(NgapMessageType::AMF_CONFIGURATION_UPDATE);
  initialize();
}

//------------------------------------------------------------------------------
AmfConfigurationUpdateMsg::~AmfConfigurationUpdateMsg() {}

//------------------------------------------------------------------------------
void AmfConfigurationUpdateMsg::initialize() {
  m_AmfConfigurationUpdateIes =
      &(ngapPdu->choice.initiatingMessage->value.choice.AMFConfigurationUpdate);
  m_AmfName             = std::nullopt;
  m_ServedGuamiList     = std::nullopt;
  m_RelativeAmfCapacity = std::nullopt;
  m_PlmnSupportList     = std::nullopt;
  m_ExtendedAmfName     = std::nullopt;
}

//------------------------------------------------------------------------------
bool AmfConfigurationUpdateMsg::setAmfName(const std::string& name) {
  AmfName amfName{};
  if (!amfName.set(name)) {
    oai::logger::logger_common::ngap().warn(
        "AmfConfigurationUpdate: invalid AMF Name value");
    return false;
  }
  m_AmfName = std::make_optional<AmfName>(amfName);

  Ngap_AMFConfigurationUpdateIEs_t* ie = (Ngap_AMFConfigurationUpdateIEs_t*)
      calloc(1, sizeof(Ngap_AMFConfigurationUpdateIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMFName;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_AMFConfigurationUpdateIEs__value_PR_AMFName;

  if (!m_AmfName.value().encode(ie->value.choice.AMFName)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP AMFName IE error (AmfConfigurationUpdate)");
    oai::utils::utils::free_wrapper((void**) &ie);
    return false;
  }

  int ret =
      ASN_SEQUENCE_ADD(&m_AmfConfigurationUpdateIes->protocolIEs->list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP AMFName IE error (AmfConfigurationUpdate)");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void AmfConfigurationUpdateMsg::setGuamiList(
    std::vector<struct GuamiItem_s>& list) {
  ServedGuamiList servedGuamiList{};
  ServedGuamiItem servedGUAMIItem = {};
  for (int i = 0; i < (int) list.size(); i++) {
    Guami guami = {};
    guami.set(
        list[i].mcc, list[i].mnc, list[i].regionId, list[i].amfSetId,
        list[i].amfPointer);
    servedGUAMIItem.setGuami(guami);

    if (list[i].backupAmfName.size() > 0) {
      AmfName amf_name = {};
      if (amf_name.set(list[i].backupAmfName)) {
        servedGUAMIItem.setBackupAmfName(amf_name);
      }
    }
    servedGuamiList.addItem(servedGUAMIItem);
  }
  m_ServedGuamiList = std::make_optional<ServedGuamiList>(servedGuamiList);

  Ngap_AMFConfigurationUpdateIEs_t* ie = (Ngap_AMFConfigurationUpdateIEs_t*)
      calloc(1, sizeof(Ngap_AMFConfigurationUpdateIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_ServedGUAMIList;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_AMFConfigurationUpdateIEs__value_PR_ServedGUAMIList;

  if (!m_ServedGuamiList.value().encode(ie->value.choice.ServedGUAMIList)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP ServedGUAMIList IE error (AmfConfigurationUpdate)");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret =
      ASN_SEQUENCE_ADD(&m_AmfConfigurationUpdateIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP ServedGUAMIList IE error (AmfConfigurationUpdate)");
}

//------------------------------------------------------------------------------
void AmfConfigurationUpdateMsg::setRelativeAmfCapacity(uint8_t capacity) {
  RelativeAmfCapacity relativeAmfCapacity{};
  relativeAmfCapacity.set(capacity);
  m_RelativeAmfCapacity =
      std::make_optional<RelativeAmfCapacity>(relativeAmfCapacity);

  Ngap_AMFConfigurationUpdateIEs_t* ie = (Ngap_AMFConfigurationUpdateIEs_t*)
      calloc(1, sizeof(Ngap_AMFConfigurationUpdateIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RelativeAMFCapacity;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_AMFConfigurationUpdateIEs__value_PR_RelativeAMFCapacity;

  if (!m_RelativeAmfCapacity.value().encode(
          ie->value.choice.RelativeAMFCapacity)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RelativeAMFCapacity IE error (AmfConfigurationUpdate)");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret =
      ASN_SEQUENCE_ADD(&m_AmfConfigurationUpdateIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RelativeAMFCapacity IE error (AmfConfigurationUpdate)");
}

//------------------------------------------------------------------------------
void AmfConfigurationUpdateMsg::setPlmnSupportList(
    const PlmnSupportList& list) {
  m_PlmnSupportList = std::make_optional<PlmnSupportList>(list);

  Ngap_AMFConfigurationUpdateIEs_t* ie = (Ngap_AMFConfigurationUpdateIEs_t*)
      calloc(1, sizeof(Ngap_AMFConfigurationUpdateIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PLMNSupportList;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_AMFConfigurationUpdateIEs__value_PR_PLMNSupportList;

  if (!m_PlmnSupportList.value().encode(ie->value.choice.PLMNSupportList)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP PLMNSupportList IE error (AmfConfigurationUpdate)");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret =
      ASN_SEQUENCE_ADD(&m_AmfConfigurationUpdateIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP PLMNSupportList IE error (AmfConfigurationUpdate)");
}

//------------------------------------------------------------------------------
void AmfConfigurationUpdateMsg::setExtendedAmfName(
    const ExtendedAmfName& value) {
  m_ExtendedAmfName = std::make_optional<ExtendedAmfName>(value);

  Ngap_AMFConfigurationUpdateIEs_t* ie = (Ngap_AMFConfigurationUpdateIEs_t*)
      calloc(1, sizeof(Ngap_AMFConfigurationUpdateIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_Extended_AMFName;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_AMFConfigurationUpdateIEs__value_PR_Extended_AMFName;

  if (!m_ExtendedAmfName.value().encode(ie->value.choice.Extended_AMFName)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP Extended-AMFName IE error (AmfConfigurationUpdate)");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret =
      ASN_SEQUENCE_ADD(&m_AmfConfigurationUpdateIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP Extended-AMFName IE error (AmfConfigurationUpdate)");
}

//------------------------------------------------------------------------------
bool AmfConfigurationUpdateMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_AMFConfigurationUpdate &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_AMFConfigurationUpdate) {
      m_AmfConfigurationUpdateIes =
          &ngapPdu->choice.initiatingMessage->value.choice
               .AMFConfigurationUpdate;
    } else {
      oai::logger::logger_common::ngap().error(
          "Check AMFConfigurationUpdate message error");
      return false;
    }
  } else {
    oai::logger::logger_common::ngap().error(
        "MessageType error, ngapPdu->present %d", ngapPdu->present);
    return false;
  }

  for (int i = 0;
       i < m_AmfConfigurationUpdateIes->protocolIEs->list.count; i++) {
    Ngap_AMFConfigurationUpdateIEs_t* ngap_ie =
        (Ngap_AMFConfigurationUpdateIEs_t*)
            m_AmfConfigurationUpdateIes->protocolIEs->list.array[i];
    switch (ngap_ie->id) {
      case Ngap_ProtocolIE_ID_id_AMFName: {
        if (ngap_ie->value.present ==
            Ngap_AMFConfigurationUpdateIEs__value_PR_AMFName) {
          AmfName tmp{};
          if (tmp.decode(ngap_ie->value.choice.AMFName)) {
            m_AmfName = std::make_optional<AmfName>(tmp);
          } else {
            oai::logger::logger_common::ngap().warn(
                "Decoded NGAP AMFName IE error (AmfConfigurationUpdate)");
          }
        }
      } break;
      case Ngap_ProtocolIE_ID_id_ServedGUAMIList: {
        if (ngap_ie->value.present ==
            Ngap_AMFConfigurationUpdateIEs__value_PR_ServedGUAMIList) {
          ServedGuamiList tmp{};
          if (tmp.decode(ngap_ie->value.choice.ServedGUAMIList)) {
            m_ServedGuamiList = std::make_optional<ServedGuamiList>(tmp);
          } else {
            oai::logger::logger_common::ngap().warn(
                "Decoded NGAP ServedGUAMIList IE error (AmfConfigurationUpdate)");
          }
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RelativeAMFCapacity: {
        if (ngap_ie->value.present ==
            Ngap_AMFConfigurationUpdateIEs__value_PR_RelativeAMFCapacity) {
          RelativeAmfCapacity tmp{};
          if (tmp.decode(ngap_ie->value.choice.RelativeAMFCapacity)) {
            m_RelativeAmfCapacity =
                std::make_optional<RelativeAmfCapacity>(tmp);
          } else {
            oai::logger::logger_common::ngap().warn(
                "Decoded NGAP RelativeAMFCapacity IE error "
                "(AmfConfigurationUpdate)");
          }
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PLMNSupportList: {
        if (ngap_ie->value.present ==
            Ngap_AMFConfigurationUpdateIEs__value_PR_PLMNSupportList) {
          PlmnSupportList tmp{};
          if (tmp.decode(ngap_ie->value.choice.PLMNSupportList)) {
            m_PlmnSupportList = std::make_optional<PlmnSupportList>(tmp);
          } else {
            oai::logger::logger_common::ngap().warn(
                "Decoded NGAP PLMNSupportList IE error "
                "(AmfConfigurationUpdate)");
          }
        }
      } break;
      case Ngap_ProtocolIE_ID_id_AMF_TNLAssociationToAddList: {
        oai::logger::logger_common::ngap().debug(
            "AMFConfigurationUpdate: AMF-TNLAssociationToAddList received "
            "(not yet decoded — deferred to Stage 8)");
      } break;
      case Ngap_ProtocolIE_ID_id_AMF_TNLAssociationToRemoveList: {
        oai::logger::logger_common::ngap().debug(
            "AMFConfigurationUpdate: AMF-TNLAssociationToRemoveList received "
            "(not yet decoded — deferred to Stage 8)");
      } break;
      case Ngap_ProtocolIE_ID_id_AMF_TNLAssociationToUpdateList: {
        oai::logger::logger_common::ngap().debug(
            "AMFConfigurationUpdate: AMF-TNLAssociationToUpdateList received "
            "(not yet decoded — deferred to Stage 8)");
      } break;
      case Ngap_ProtocolIE_ID_id_Extended_AMFName: {
        if (ngap_ie->value.present ==
            Ngap_AMFConfigurationUpdateIEs__value_PR_Extended_AMFName) {
          ExtendedAmfName tmp{};
          if (tmp.decode(ngap_ie->value.choice.Extended_AMFName)) {
            m_ExtendedAmfName = std::make_optional<ExtendedAmfName>(tmp);
          } else {
            oai::logger::logger_common::ngap().warn(
                "Decoded NGAP Extended-AMFName IE error "
                "(AmfConfigurationUpdate)");
          }
        }
      } break;
      default: {
        oai::logger::logger_common::ngap().warn(
            "AMFConfigurationUpdate: unknown IE id %ld, skipping", ngap_ie->id);
      } break;
    }
  }

  return true;
}

}  // namespace oai::ngap
