/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "Paging.hpp"

#include "logger_base.hpp"
#include "ngap_utils.hpp"
#include "utils.hpp"

extern "C" {
#include "Ngap_ProtocolIE-ID.h"
}

namespace oai::ngap {

//------------------------------------------------------------------------------
PagingMsg::PagingMsg() {
  m_PagingIes = nullptr;

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
  for (int i = 0; i < m_PagingIes->protocolIEs.list.count; i++) {
    switch (m_PagingIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_UEPagingIdentity: {
        if (m_PagingIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_PagingIes->protocolIEs.list.array[i]->value.present ==
                Ngap_PagingIEs__value_PR_UEPagingIdentity) {
          if (!m_UePagingIdentity.decode(m_PagingIes->protocolIEs.list.array[i]
                                             ->value.choice.UEPagingIdentity)) {
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
        if (m_PagingIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_PagingIes->protocolIEs.list.array[i]->value.present ==
                Ngap_PagingIEs__value_PR_PagingDRX) {
          PagingDrx paging_drx = {};
          if (!paging_drx.decode(m_PagingIes->protocolIEs.list.array[i]
                                     ->value.choice.PagingDRX)) {
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
        if (m_PagingIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_PagingIes->protocolIEs.list.array[i]->value.present ==
                Ngap_PagingIEs__value_PR_TAIListForPaging) {
          if (!m_TaiListForPaging.decode(m_PagingIes->protocolIEs.list.array[i]
                                             ->value.choice.TAIListForPaging)) {
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
        if (m_PagingIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_PagingIes->protocolIEs.list.array[i]->value.present ==
                Ngap_PagingIEs__value_PR_PagingPriority) {
          PagingPriority paging_priority = {};
          if (!paging_priority.decode(m_PagingIes->protocolIEs.list.array[i]
                                          ->value.choice.PagingPriority)) {
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

      case Ngap_ProtocolIE_ID_id_PagingOrigin: {
        if (m_PagingIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_PagingIes->protocolIEs.list.array[i]->value.present ==
                Ngap_PagingIEs__value_PR_PagingOrigin) {
          m_pagingOrigin = static_cast<e_Ngap_PagingOrigin>(
              m_PagingIes->protocolIEs.list.array[i]
                  ->value.choice.PagingOrigin);
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP PagingOrigin IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_UERadioCapabilityForPaging: {
        if (m_PagingIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_PagingIes->protocolIEs.list.array[i]->value.present ==
                Ngap_PagingIEs__value_PR_UERadioCapabilityForPaging) {
          UeRadioCapabilityForPaging capability = {};
          if (!capability.decode(
                  m_PagingIes->protocolIEs.list.array[i]
                      ->value.choice.UERadioCapabilityForPaging)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP UERadioCapabilityForPaging IE error");
            return false;
          }
          m_ueRadioCapabilityForPaging =
              std::make_optional<UeRadioCapabilityForPaging>(capability);
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP UERadioCapabilityForPaging IE error");
          return false;
        }
      } break;

      default: {
        oai::logger::logger_common::ngap().warn(
            "Not decoded IE %d", m_PagingIes->protocolIEs.list.array[i]->id);

        return true;
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
  if (!ie) {
    oai::logger::logger_common::ngap().error(
        "calloc failed for UE Paging Identity IE");
    return;
  }
  ie->id            = Ngap_ProtocolIE_ID_id_UEPagingIdentity;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_UEPagingIdentity;

  int ret = m_UePagingIdentity.encode(ie->value.choice.UEPagingIdentity);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP UEPagingIdentity IE error");
    ASN_STRUCT_FREE(asn_DEF_Ngap_PagingIEs, ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs.list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP UEPagingIdentity IE error");
    ASN_STRUCT_FREE(asn_DEF_Ngap_PagingIEs, ie);
  }
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
  if (!ie) {
    oai::logger::logger_common::ngap().error(
        "calloc failed for TAI List For Paging IE");
    return;
  }
  ie->id            = Ngap_ProtocolIE_ID_id_TAIListForPaging;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_TAIListForPaging;

  int ret = m_TaiListForPaging.encode(ie->value.choice.TAIListForPaging);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP TAIListForPaging IE error");
    ASN_STRUCT_FREE(asn_DEF_Ngap_PagingIEs, ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs.list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP TAIListForPaging IE error");
    ASN_STRUCT_FREE(asn_DEF_Ngap_PagingIEs, ie);
  }
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
    ASN_STRUCT_FREE(asn_DEF_Ngap_PagingIEs, ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs.list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error("Encode NGAP PagingDRX IE error");
    ASN_STRUCT_FREE(asn_DEF_Ngap_PagingIEs, ie);
  }
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
    ASN_STRUCT_FREE(asn_DEF_Ngap_PagingIEs, ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs.list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP PagingPriority IE error");
    ASN_STRUCT_FREE(asn_DEF_Ngap_PagingIEs, ie);
  }
}

//------------------------------------------------------------------------------
void PagingMsg::setPagingOrigin(e_Ngap_PagingOrigin origin) {
  m_pagingOrigin = std::make_optional(origin);

  Ngap_PagingIEs_t* ie =
      (Ngap_PagingIEs_t*) calloc(1, sizeof(Ngap_PagingIEs_t));
  if (!ie) {
    oai::logger::logger_common::ngap().error(
        "calloc failed for Paging Origin IE");
    return;
  }
  ie->id                        = Ngap_ProtocolIE_ID_id_PagingOrigin;
  ie->criticality               = Ngap_Criticality_ignore;
  ie->value.present             = Ngap_PagingIEs__value_PR_PagingOrigin;
  ie->value.choice.PagingOrigin = static_cast<Ngap_PagingOrigin_t>(origin);

  int ret = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs.list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP PagingOrigin IE error");
    ASN_STRUCT_FREE(asn_DEF_Ngap_PagingIEs, ie);
  }
}

//------------------------------------------------------------------------------
void PagingMsg::setUeRadioCapabilityForPaging(
    const OCTET_STRING_t& ue_radio_capability_for_paging_of_nr,
    const OCTET_STRING_t& ue_radio_capability_for_paging_of_eutra) {
  if (!ngap_utils::check_octet_string(ue_radio_capability_for_paging_of_nr) &&
      !ngap_utils::check_octet_string(
          ue_radio_capability_for_paging_of_eutra)) {
    return;
  }

  UeRadioCapabilityForPaging capability = {};
  if (ngap_utils::check_octet_string(ue_radio_capability_for_paging_of_nr)) {
    capability.setUeRadioCapabilityForPagingOfNr(
        ue_radio_capability_for_paging_of_nr);
  }
  if (ngap_utils::check_octet_string(ue_radio_capability_for_paging_of_eutra)) {
    capability.setUeRadioCapabilityForPagingOfEutra(
        ue_radio_capability_for_paging_of_eutra);
  }
  m_ueRadioCapabilityForPaging =
      std::make_optional<UeRadioCapabilityForPaging>(capability);

  Ngap_PagingIEs_t* ie =
      (Ngap_PagingIEs_t*) calloc(1, sizeof(Ngap_PagingIEs_t));
  if (!ie) {
    oai::logger::logger_common::ngap().error(
        "calloc failed for UE Radio Capability For Paging IE");
    return;
  }
  ie->id            = Ngap_ProtocolIE_ID_id_UERadioCapabilityForPaging;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_UERadioCapabilityForPaging;

  if (!m_ueRadioCapabilityForPaging.value().encode(
          ie->value.choice.UERadioCapabilityForPaging)) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP UERadioCapabilityForPaging IE error");
    ASN_STRUCT_FREE(asn_DEF_Ngap_PagingIEs, ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs.list, ie);
  if (ret != 0) {
    oai::logger::logger_common::ngap().error(
        "Encode NGAP UERadioCapabilityForPaging IE error");
    ASN_STRUCT_FREE(asn_DEF_Ngap_PagingIEs, ie);
  }
}

}  // namespace oai::ngap
