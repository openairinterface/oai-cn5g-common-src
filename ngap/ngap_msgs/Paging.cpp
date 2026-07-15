/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "Paging.hpp"

#include "logger_base.hpp"
extern "C" {
#include "Ngap_ProtocolIE_Container_compat.h"
}
#include "utils.hpp"

extern "C" {
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_ProtocolIE-ID.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
PagingMsg::PagingMsg() {
  m_PagingIes                  = nullptr;
  m_NrPagingEDrxInformation    = std::nullopt;
  m_PagingCause                = std::nullopt;
  m_PeipsAssistanceInformation = std::nullopt;

  NgapMessage::setMessageType(NgapMessageType::PAGING);
  initialize();
}

//------------------------------------------------------------------------------
PagingMsg::~PagingMsg() {}

//------------------------------------------------------------------------------
void PagingMsg::initialize() {
  m_PagingIes = &(ngapPdu->choice.initiatingMessage->value.choice.Paging);
}

//------------------------------------------------------------------------------
bool PagingMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_Paging &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_Paging) {
      m_PagingIes = &ngapPdu->choice.initiatingMessage->value.choice.Paging;
    } else {
      oai::logger::logger_common::ngap().error("Check Paging message error");
      return false;
    }
  } else {
    oai::logger::logger_common::ngap().error("MessageType error");
    return false;
  }
  for (int i = 0; i < m_PagingIes->protocolIEs->list.count; i++) {
    Ngap_PagingIEs_t* ngap_ie =
        (Ngap_PagingIEs_t*) m_PagingIes->protocolIEs->list.array[i];
    switch (ngap_ie->id) {
      case Ngap_ProtocolIE_ID_id_UEPagingIdentity: {
        if (ngap_ie->criticality == Ngap_Criticality_ignore &&
            ngap_ie->value.present ==
                Ngap_PagingIEs__value_PR_UEPagingIdentity) {
          if (!m_UePagingIdentity.decode(
                  ngap_ie->value.choice.UEPagingIdentity)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP UEPagingIdentity IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP UEPagingIdentity IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PagingDRX: {
        if (ngap_ie->criticality == Ngap_Criticality_ignore &&
            ngap_ie->value.present == Ngap_PagingIEs__value_PR_PagingDRX) {
          PagingDrx paging_drx = {};
          if (!paging_drx.decode(ngap_ie->value.choice.PagingDRX)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP PagingDRX IE error");
            return false;
          }
          m_pagingDRX = std::make_optional<PagingDrx>(paging_drx);
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP PagingDRX IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_TAIListForPaging: {
        if (ngap_ie->criticality == Ngap_Criticality_ignore &&
            ngap_ie->value.present ==
                Ngap_PagingIEs__value_PR_TAIListForPaging) {
          if (!m_TaiListForPaging.decode(
                  ngap_ie->value.choice.TAIListForPaging)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP TAIListForPaging IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP TAIListForPaging IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_PagingPriority: {
        if (ngap_ie->criticality == Ngap_Criticality_ignore &&
            ngap_ie->value.present == Ngap_PagingIEs__value_PR_PagingPriority) {
          PagingPriority paging_priority = {};
          if (!paging_priority.decode(ngap_ie->value.choice.PagingPriority)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP PagingPriority IE error");
            return false;
          }
          m_pagingPriority =
              std::make_optional<PagingPriority>(paging_priority);
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP PagingPriority IE error");
          return false;
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
void PagingMsg::setUePagingIdentity(
    const std::string& setId, const std::string& pointer, std::string tmsi) {
  m_UePagingIdentity.set(setId, pointer, tmsi);

  Ngap_PagingIEs_t* ie =
      (Ngap_PagingIEs_t*) calloc(1, sizeof(Ngap_PagingIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UEPagingIdentity;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_UEPagingIdentity;

  int ret = m_UePagingIdentity.encode(ie->value.choice.UEPagingIdentity);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP UEPagingIdentity IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP UEPagingIdentity IE error");
}

//------------------------------------------------------------------------------
void PagingMsg::getUePagingIdentity(std::string& _5g_s_tmsi) const {
  m_UePagingIdentity.get(_5g_s_tmsi);
}

//------------------------------------------------------------------------------
void PagingMsg::getUePagingIdentity(
    std::string& setId, std::string& pointer, std::string& tmsi) const {
  m_UePagingIdentity.get(setId, pointer, tmsi);
}

//------------------------------------------------------------------------------
void PagingMsg::setTaiListForPaging(const std::vector<Tai_t>& list) {
  if (list.size() == 0) {
    oai::logger::logger_common::ngap().warn("Setup failed, vector is empty");
    return;
  }

  std::vector<Tai> tailist;

  PlmnId plmnid[list.size()];
  TAC tac[list.size()];
  for (int i = 0; i < list.size(); i++) {
    Tai tai = {};
    plmnid[i].set(list[i].mcc, list[i].mnc);
    tac[i].set(list[i].tac);
    tai.set(plmnid[i], tac[i]);
    tailist.push_back(tai);
  }
  m_TaiListForPaging.set(tailist);

  Ngap_PagingIEs_t* ie =
      (Ngap_PagingIEs_t*) calloc(1, sizeof(Ngap_PagingIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_TAIListForPaging;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_TAIListForPaging;

  int ret = m_TaiListForPaging.encode(ie->value.choice.TAIListForPaging);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP TAIListForPaging IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP TAIListForPaging IE error");
}

//------------------------------------------------------------------------------
void PagingMsg::getTaiListForPaging(std::vector<Tai_t>& list) const {
  std::vector<Tai> taiList;
  m_TaiListForPaging.get(taiList);

  for (auto& tai : taiList) {
    Tai_t t = {};
    tai.get(t);
    list.push_back(t);
  }
}

//------------------------------------------------------------------------------
// Paging DRX — per-UE DRX cycle override (TS 38.413 §9.3.1.102)
void PagingMsg::setPagingDrx(e_Ngap_PagingDRX drx) {
  PagingDrx paging_drx = {};
  paging_drx.set(drx);
  m_pagingDRX = std::make_optional<PagingDrx>(paging_drx);

  Ngap_PagingIEs_t* ie =
      (Ngap_PagingIEs_t*) calloc(1, sizeof(Ngap_PagingIEs_t));
  if (!ie) {
    oai::logger::logger_common::ngap().error("calloc failed for Paging DRX IE");
    return;
  }
  ie->id            = Ngap_ProtocolIE_ID_id_PagingDRX;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_PagingDRX;

  if (!paging_drx.encode(ie->value.choice.PagingDRX)) {
    oai::logger::logger_common::ngap().error("Encode NGAP PagingDRX IE error");
    free(ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode NGAP PagingDRX IE error");
}

//------------------------------------------------------------------------------
// Paging Priority — mapped from Paging Policy Indicator (TS 23.501 §5.6.3)
void PagingMsg::setPagingPriority(uint8_t ppi) {
  PagingPriority paging_priority = {};
  paging_priority.setFromPpi(ppi);
  m_pagingPriority = std::make_optional<PagingPriority>(paging_priority);

  Ngap_PagingIEs_t* ie =
      (Ngap_PagingIEs_t*) calloc(1, sizeof(Ngap_PagingIEs_t));
  if (!ie) {
    oai::logger::logger_common::ngap().error(
        "calloc failed for Paging Priority IE");
    return;
  }
  ie->id            = Ngap_ProtocolIE_ID_id_PagingPriority;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_PagingPriority;

  if (!paging_priority.encode(ie->value.choice.PagingPriority)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP PagingPriority IE error");
    free(ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP PagingPriority IE error");
}

//------------------------------------------------------------------------------
void PagingMsg::setNrPagingEDrxInformation(
    const NrPagingEDrxInformation& value) {
  m_NrPagingEDrxInformation =
      std::make_optional<NrPagingEDrxInformation>(value);
  Ngap_PagingIEs_t* ie =
      (Ngap_PagingIEs_t*) calloc(1, sizeof(Ngap_PagingIEs_t));
  if (!ie) {
    oai::logger::logger_common::ngap().error(
        "calloc failed for NrPagingEDrxInformation IE");
    return;
  }
  ie->id            = Ngap_ProtocolIE_ID_id_NR_PagingeDRXInformation;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_NR_PagingeDRXInformation;
  if (!m_NrPagingEDrxInformation.value().encode(
          ie->value.choice.NR_PagingeDRXInformation)) {
    oai::logger::logger_common::ngap().error(
        "Encode NrPagingEDrxInformation IE error");
    free(ie);
    return;
  }
  int ret2 = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs->list, ie);
  if (ret2 != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NrPagingEDrxInformation IE error");
}

//------------------------------------------------------------------------------
void PagingMsg::setPagingCause(const PagingCause& value) {
  m_PagingCause = std::make_optional<PagingCause>(value);
  Ngap_PagingIEs_t* ie =
      (Ngap_PagingIEs_t*) calloc(1, sizeof(Ngap_PagingIEs_t));
  if (!ie) {
    oai::logger::logger_common::ngap().error(
        "calloc failed for PagingCause IE");
    return;
  }
  ie->id            = Ngap_ProtocolIE_ID_id_PagingCause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_PagingCause;
  if (!m_PagingCause.value().encode(ie->value.choice.PagingCause)) {
    oai::logger::logger_common::ngap().error("Encode PagingCause IE error");
    free(ie);
    return;
  }
  int ret3 = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs->list, ie);
  if (ret3 != 0)
    oai::logger::logger_common::ngap().error("Encode PagingCause IE error");
}

//------------------------------------------------------------------------------
void PagingMsg::setPeipsAssistanceInformation(
    const PeipsAssistanceInformation& value) {
  m_PeipsAssistanceInformation =
      std::make_optional<PeipsAssistanceInformation>(value);
  Ngap_PagingIEs_t* ie =
      (Ngap_PagingIEs_t*) calloc(1, sizeof(Ngap_PagingIEs_t));
  if (!ie) {
    oai::logger::logger_common::ngap().error(
        "calloc failed for PeipsAssistanceInformation IE");
    return;
  }
  ie->id            = Ngap_ProtocolIE_ID_id_PEIPSassistanceInformation;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_PEIPSassistanceInformation;
  if (!m_PeipsAssistanceInformation.value().encode(
          ie->value.choice.PEIPSassistanceInformation)) {
    oai::logger::logger_common::ngap().error(
        "Encode PeipsAssistanceInformation IE error");
    free(ie);
    return;
  }
  int ret4 = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs->list, ie);
  if (ret4 != 0)
    oai::logger::logger_common::ngap().error(
        "Encode PeipsAssistanceInformation IE error");
}

}  // namespace oai::ngap
