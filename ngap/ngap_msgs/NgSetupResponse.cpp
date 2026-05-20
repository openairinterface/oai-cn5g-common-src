/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "NgSetupResponse.hpp"

#include "3gpp_23.003.h"
#include "logger_base.hpp"
extern "C" {
#include "Ngap_NGSetupResponse.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_ProtocolIE-ID.h"
#include "Ngap_ProtocolIE_Container_compat.h"
}
#include "utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
NgSetupResponseMsg::NgSetupResponseMsg() : NgapMessage() {
  m_NgSetupResponsIes = nullptr;
  NgapMessage::setMessageType(NgapMessageType::NG_SETUP_RESPONSE);
  initialize();
  // criticalityDiagnostics = nullptr;
}

//------------------------------------------------------------------------------
NgSetupResponseMsg::~NgSetupResponseMsg() {}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::initialize() {
  m_NgSetupResponsIes =
      &(ngapPdu->choice.successfulOutcome->value.choice.NGSetupResponse);
  m_IabSupported      = std::nullopt;
  m_ExtendedAmfName   = std::nullopt;
}

//------------------------------------------------------------------------------
bool NgSetupResponseMsg::setAmfName(const std::string& name) {
  if (!m_AmfName.set(name)) {
    oai::logger::logger_common::ngap().warn("Not a valid AMF Name value!");
    return false;
  }

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMFName;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_AMFName;

  if (!m_AmfName.encode(ie->value.choice.AMFName)) {
    oai::logger::logger_common::ngap().error("Encode NGAP AMFName IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return false;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupResponsIes->protocolIEs->list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error("Encode NGAP AMFName IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::setGuamiList(std::vector<struct GuamiItem_s>& list) {
  ServedGuamiItem servedGUAMIItem = {};
  for (int i = 0; i < list.size(); i++) {
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
    m_ServedGuamiList.addItem(servedGUAMIItem);
  }

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_ServedGUAMIList;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_ServedGUAMIList;

  if (!m_ServedGuamiList.encode(ie->value.choice.ServedGUAMIList)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP ServedGUAMIList IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupResponsIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP ServedGUAMIList IE error");
}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::setRelativeAmfCapacity(const long& capacity) {
  m_RelativeAmfCapacity.set(capacity);

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RelativeAMFCapacity;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_RelativeAMFCapacity;

  if (!m_RelativeAmfCapacity.encode(ie->value.choice.RelativeAMFCapacity)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RelativeAMFCapacity IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupResponsIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP RelativeAMFCapacity IE error");
}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::setPlmnSupportList(const PlmnSupportList& list) {
  m_PlmnSupportList = list;

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_PLMNSupportList;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_PLMNSupportList;

  if (!m_PlmnSupportList.encode(ie->value.choice.PLMNSupportList)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP PLMNSupportList IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupResponsIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP PLMNSupportList IE error");
}

//------------------------------------------------------------------------------
bool NgSetupResponseMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_NGSetup &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_NGSetupResponse) {
      m_NgSetupResponsIes =
          &ngapPdu->choice.successfulOutcome->value.choice.NGSetupResponse;
    } else {
      oai::logger::logger_common::ngap().error(
          "Check NGSetupResponse message error");
      return false;
    }
  } else {
    oai::logger::logger_common::ngap().error(
        "MessageType error, ngapPdu->present %d", ngapPdu->present);
    return false;
  }
  for (int i = 0; i < m_NgSetupResponsIes->protocolIEs->list.count; i++) {
    Ngap_NGSetupResponseIEs_t* ngap_ie =
        (Ngap_NGSetupResponseIEs_t*) m_NgSetupResponsIes->protocolIEs->list.array[i];
    switch (ngap_ie->id) {
      case Ngap_ProtocolIE_ID_id_AMFName: {
        if (ngap_ie->criticality ==
                Ngap_Criticality_reject &&
            ngap_ie->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_AMFName) {
          if (!m_AmfName.decode(ngap_ie
                                    ->value.choice.AMFName)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP AMFName error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP AMFName error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_ServedGUAMIList: {
        if (ngap_ie->criticality ==
                Ngap_Criticality_reject &&
            ngap_ie->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_ServedGUAMIList) {
          if (!m_ServedGuamiList.decode(
                  ngap_ie
                      ->value.choice.ServedGUAMIList)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP ServedGUAMIList error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP ServedGUAMIList error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RelativeAMFCapacity: {
        if (ngap_ie->criticality ==
                Ngap_Criticality_ignore &&
            ngap_ie->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_RelativeAMFCapacity) {
          if (!m_RelativeAmfCapacity.decode(
                  ngap_ie
                      ->value.choice.RelativeAMFCapacity)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP RelativeAMFCapacity error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP RelativeAMFCapacity error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PLMNSupportList: {
        if (ngap_ie->criticality ==
                Ngap_Criticality_reject &&
            ngap_ie->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_PLMNSupportList) {
          if (!m_PlmnSupportList.decode(
                  ngap_ie
                      ->value.choice.PLMNSupportList)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP PLMNSupportList error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP PLMNSupportList error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UERetentionInformation: {
        if (ngap_ie->criticality ==
                Ngap_Criticality_ignore &&
            ngap_ie->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_UERetentionInformation) {
          UeRetentionInformation tmp = {};
          if (!tmp.decode(ngap_ie
                              ->value.choice.UERetentionInformation)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP UeRetentionInformation IE error");
            return false;
          }
          m_UeRetentionInformation =
              std::make_optional<UeRetentionInformation>(tmp);
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP UeRetentionInformation IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        oai::logger::logger_common::ngap().debug(
            "Decoded NGAP CriticalityDiagnostics");
      } break;
      case Ngap_ProtocolIE_ID_id_IAB_Supported: {
        if (ngap_ie->value.present ==
            Ngap_NGSetupResponseIEs__value_PR_IAB_Supported) {
          IabSupported tmp{};
          if (tmp.decode(ngap_ie->value.choice.IAB_Supported)) {
            m_IabSupported = std::make_optional<IabSupported>(tmp);
          } else {
            oai::logger::logger_common::ngap().warn(
                "Decoded NGAP IAB-Supported IE error");
          }
        }
      } break;
      case Ngap_ProtocolIE_ID_id_Extended_AMFName: {
        if (ngap_ie->value.present ==
            Ngap_NGSetupResponseIEs__value_PR_Extended_AMFName) {
          ExtendedAmfName tmp{};
          if (tmp.decode(ngap_ie->value.choice.Extended_AMFName)) {
            m_ExtendedAmfName = std::make_optional<ExtendedAmfName>(tmp);
          } else {
            oai::logger::logger_common::ngap().warn(
                "Decoded NGAP Extended-AMFName IE error");
          }
        }
      } break;
      default: {
        oai::logger::logger_common::ngap().warn(
            "Decoded NGAP NGSetupResponse: unknown IE id %ld, skipping",
            ngap_ie->id);
      } break;
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool NgSetupResponseMsg::getAmfName(std::string& name) const {
  m_AmfName.get(name);
  return true;
}

//------------------------------------------------------------------------------
bool NgSetupResponseMsg::getGuamiList(
    std::vector<struct GuamiItem_s>& list) const {
  std::vector<ServedGuamiItem> servedGUAMIItems;
  m_ServedGuamiList.get(servedGUAMIItems);

  for (std::vector<ServedGuamiItem>::iterator it = std::begin(servedGUAMIItems);
       it != std::end(servedGUAMIItems); ++it) {
    GuamiItem_t guamiItem = {};
    Guami guami           = {};
    it->getGuami(guami);
    guami.get(
        guamiItem.mcc, guamiItem.mnc, guamiItem.regionId, guamiItem.amfSetId,
        guamiItem.amfPointer);

    AmfName amf_name = {};
    if (it->getBackupAmfName(amf_name)) {
      amf_name.get(guamiItem.backupAmfName);
    }

    list.push_back(guamiItem);
  }

  return true;
}

//------------------------------------------------------------------------------
long NgSetupResponseMsg::getRelativeAmfCapacity() const {
  return m_RelativeAmfCapacity.get();
}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::getPlmnSupportList(PlmnSupportList& list) const {
  list = m_PlmnSupportList;
}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::setUeRetentionInformation(
    const UeRetentionInformation& value) {
  m_UeRetentionInformation = std::make_optional<UeRetentionInformation>(value);

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UERetentionInformation;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_UERetentionInformation;

  if (!m_UeRetentionInformation.value().encode(
          ie->value.choice.UERetentionInformation)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP UeRetentionInformation IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupResponsIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP UeRetentionInformation IE error");
}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::getUeRetentionInformation(
    std::optional<UeRetentionInformation>& value) const {
  value = m_UeRetentionInformation;
}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::setIabSupported(const IabSupported& value) {
  m_IabSupported = std::make_optional<IabSupported>(value);

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_IAB_Supported;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_IAB_Supported;

  if (!m_IabSupported.value().encode(ie->value.choice.IAB_Supported)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP IAB-Supported IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupResponsIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP IAB-Supported IE error");
}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::setExtendedAmfName(const ExtendedAmfName& value) {
  m_ExtendedAmfName = std::make_optional<ExtendedAmfName>(value);

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Extended_AMFName;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_Extended_AMFName;

  if (!m_ExtendedAmfName.value().encode(ie->value.choice.Extended_AMFName)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP Extended-AMFName IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupResponsIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP Extended-AMFName IE error");
}

}  // namespace oai::ngap
