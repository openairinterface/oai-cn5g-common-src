/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "InitialContextSetupRequest.hpp"

#include "logger_base.hpp"
extern "C" {
#include "Ngap_InitialContextSetupRequest.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_ProtocolIE-ID.h"
#include "Ngap_ProtocolIE_Container_compat.h"
}
#include "ngap_utils.hpp"
#include "utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
InitialContextSetupRequestMsg::InitialContextSetupRequestMsg()
    : NgapUeMessage() {
  m_InitialContextSetupRequestIes               = nullptr;
  m_OldAmf                                      = std::nullopt;
  m_UeAggregateMaxBitRate                       = std::nullopt;
  m_CoreNetworkAssistanceInformationForInactive = std::nullopt;
  m_PduSessionResourceSetupRequestList          = std::nullopt;
  m_UeRadioCapability                           = std::nullopt;
  m_MaskedImeiSv                                = std::nullopt;
  m_NasPdu                                      = std::nullopt;
  m_ManagementBasedMdtPlmnList                  = std::nullopt;
  m_TimeSynchronisationAssistanceInfo           = std::nullopt;
  m_QmcConfigInfo                               = std::nullopt;
  m_TargetNssaiInformation                      = std::nullopt;
  m_UeSliceMaximumBitRateList                   = std::nullopt;
  m_FiveGProSeAuthorized                        = std::nullopt;
  m_FiveGProSeUePC5AggregateMaximumBitRate      = std::nullopt;
  m_FiveGProSePC5QoSParameters                  = std::nullopt;

  setMessageType(NgapMessageType::INITIAL_CONTEXT_SETUP_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
InitialContextSetupRequestMsg::~InitialContextSetupRequestMsg() {}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::initialize() {
  m_InitialContextSetupRequestIes = &(ngapPdu->choice.initiatingMessage->value
                                          .choice.InitialContextSetupRequest);
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setAmfUeNgapId(const uint64_t& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setRanUeNgapId(
    const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setOldAmf(const std::string& name) {
  AmfName tmp = {};
  tmp.set(name);
  m_OldAmf = std::optional<AmfName>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_OldAMF;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_AMFName;

  int ret = m_OldAmf.value().encode(ie->value.choice.AMFName);
  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode oldAmfName IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode oldAmfName IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getOldAmf(std::string& name) const {
  if (!m_OldAmf.has_value()) return false;
  m_OldAmf.value().get(name);
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUeAggregateMaxBitRate(
    const uint64_t& bitRateDl, const uint64_t& bitRateUl) {
  UeAggregateMaxBitRate tmp = {};
  tmp.set(bitRateDl, bitRateUl);
  m_UeAggregateMaxBitRate = std::optional<UeAggregateMaxBitRate>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

  int ret = m_UeAggregateMaxBitRate.value().encode(
      ie->value.choice.UEAggregateMaximumBitRate);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode UeAggregateMaxBitRate IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode UeAggregateMaxBitRate IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getUeAggregateMaxBitRate(
    uint64_t& bitRateDl, uint64_t& bitRateUl) const {
  if (!m_UeAggregateMaxBitRate.has_value()) return false;
  m_UeAggregateMaxBitRate.value().get(bitRateDl, bitRateUl);
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUeAggregateMaxBitRate(
    const UeAggregateMaxBitRate& bitRate) {
  m_UeAggregateMaxBitRate = std::make_optional<UeAggregateMaxBitRate>(bitRate);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

  int ret = m_UeAggregateMaxBitRate.value().encode(
      ie->value.choice.UEAggregateMaximumBitRate);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode UEAggregateMaximumBitRate IE error");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode NGAP UEAggregateMaximumBitRate IE error");

  return;
};

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getUeAggregateMaxBitRate(
    UeAggregateMaxBitRate& bitRate) const {
  if (!m_UeAggregateMaxBitRate.has_value()) return false;
  bitRate = m_UeAggregateMaxBitRate.value();
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setCoreNetworkAssistanceInfo(
    uint16_t ueIdentityIndexValue_Value,
    e_Ngap_PagingDRX defaultPagingDrx_Value,
    uint8_t periodicRegistrationUpdateTimer_Value, bool micoModeIndValue,
    const std::vector<Tai_t>& taiListForRrcInactive) {
  CoreNetworkAssistanceInformationForInactive tmp = {};

  UeIdentityIndexValue ueIdentityIndexValue = {};
  ueIdentityIndexValue.set(ueIdentityIndexValue_Value);
  PagingDrx defaultPagingDrx = {};
  defaultPagingDrx.set(defaultPagingDrx_Value);
  PeriodicRegistrationUpdateTimer periodicRegistrationUpdateTimer = {};
  periodicRegistrationUpdateTimer.set(periodicRegistrationUpdateTimer_Value);

  std::vector<Tai> taiList;
  for (int i = 0; i < taiListForRrcInactive.size(); i++) {
    Tai tai = {};
    tai.set(
        taiListForRrcInactive[i].mcc, taiListForRrcInactive[i].mnc,
        taiListForRrcInactive[i].tac);
    taiList.push_back(tai);
  }

  tmp.set(
      ueIdentityIndexValue, defaultPagingDrx, periodicRegistrationUpdateTimer,
      micoModeIndValue, taiList);

  m_CoreNetworkAssistanceInformationForInactive =
      std::optional<CoreNetworkAssistanceInformationForInactive>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_CoreNetworkAssistanceInformationForInactive;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_CoreNetworkAssistanceInformationForInactive;

  if (!ie->value.choice.CoreNetworkAssistanceInformationForInactive) ie->value.choice.CoreNetworkAssistanceInformationForInactive = (Ngap_CoreNetworkAssistanceInformationForInactive_t*) calloc(1, sizeof(Ngap_CoreNetworkAssistanceInformationForInactive_t));
  int ret = m_CoreNetworkAssistanceInformationForInactive.value().encode(
      *ie->value.choice.CoreNetworkAssistanceInformationForInactive);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode CoreNetworkAssistanceInformationForInactive IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode CoreNetworkAssistanceInformation IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getCoreNetworkAssistanceInfo(
    uint16_t& ueIdentityIndexValue_Value, int& defaultPagingDrx_Value,
    uint8_t& periodicRegistrationUpdateTimer_Value, bool& micoModeIndValue,
    std::vector<Tai_t>& taiListForRrcInactive) const {
  if (!m_CoreNetworkAssistanceInformationForInactive.has_value()) return false;
  UeIdentityIndexValue ueIdentityIndexValue = {};
  std::optional<PagingDrx> defaultPagingDrx = std::nullopt;
  PeriodicRegistrationUpdateTimer periodicRegistrationUpdateTimer = {};

  std::vector<Tai> taiList;

  m_CoreNetworkAssistanceInformationForInactive.value().get(
      ueIdentityIndexValue, defaultPagingDrx, periodicRegistrationUpdateTimer,
      micoModeIndValue, taiList);
  ueIdentityIndexValue.get(ueIdentityIndexValue_Value);
  if (defaultPagingDrx)
    defaultPagingDrx_Value = defaultPagingDrx->get();
  else
    defaultPagingDrx_Value = -1;

  periodicRegistrationUpdateTimer.get(periodicRegistrationUpdateTimer_Value);

  for (std::vector<Tai>::iterator it = std::begin(taiList);
       it < std::end(taiList); ++it) {
    Tai_t tai = {};
    it->get(tai);
    taiListForRrcInactive.push_back(tai);
  }
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setGuami(const guami_full_format_t& value) {
  m_Guami.set(
      value.mcc, value.mnc, value.region_id, value.amf_set_id,
      value.amf_pointer);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_GUAMI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_GUAMI;

  int ret = m_Guami.encode(ie->value.choice.GUAMI);
  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode GUAMI IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode GUAMI IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getGuami(guami_full_format_t& value) const {
  m_Guami.get(
      value.mcc, value.mnc, value.region_id, value.amf_set_id,
      value.amf_pointer);
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setPduSessionResourceSetupRequestList(
    const std::vector<PDUSessionResourceSetupRequestItem_t>& list) {
  PduSessionResourceSetupListCxtReq cxtReqList = {};

  std::vector<PduSessionResourceSetupItemCxtReq>
      pduSessionResourceSetupItemCxtReqList;
  pduSessionResourceSetupItemCxtReqList.reserve(list.size());

  for (int i = 0; i < list.size(); i++) {
    PduSessionResourceSetupItemCxtReq pduSessionResourceSetupItemCxtReq = {};
    PduSessionId pDUSessionID                                           = {};
    pDUSessionID.set(list[i].pduSessionId);
    std::optional<NasPdu> nAS_PDU = std::nullopt;

    if (ngap_utils::check_bstring(list[i].nasPdu)) {
      NasPdu tmp = {};
      tmp.set(list[i].nasPdu);
      nAS_PDU = std::optional<NasPdu>(tmp);
    }
    SNssai s_NSSAI = {};
    s_NSSAI.setSst(list[i].sNssai.sst);
    if (list[i].sNssai.sd.size()) s_NSSAI.setSd(list[i].sNssai.sd);
    pduSessionResourceSetupItemCxtReq.set(
        pDUSessionID, nAS_PDU, s_NSSAI,
        list[i].pduSessionResourceSetupRequestTransfer);
    pduSessionResourceSetupItemCxtReqList.push_back(
        pduSessionResourceSetupItemCxtReq);
  }

  cxtReqList.set(pduSessionResourceSetupItemCxtReqList);
  m_PduSessionResourceSetupRequestList =
      std::optional<PduSessionResourceSetupListCxtReq>(cxtReqList);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtReq;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_PDUSessionResourceSetupListCxtReq;

  int ret = m_PduSessionResourceSetupRequestList.value().encode(
      ie->value.choice.PDUSessionResourceSetupListCxtReq);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode PDUSessionResourceSetupListCxtReq IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode PDUSessionResourceSetupListCxtReq IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getPduSessionResourceSetupRequestList(
    std::vector<PDUSessionResourceSetupRequestItem_t>& list) const {
  if (!m_PduSessionResourceSetupRequestList.has_value()) return false;

  std::vector<PduSessionResourceSetupItemCxtReq>
      pduSessionResourceSetupItemCxtReqList;
  m_PduSessionResourceSetupRequestList.value().get(
      pduSessionResourceSetupItemCxtReqList);

  for (std::vector<PduSessionResourceSetupItemCxtReq>::iterator it =
           std::begin(pduSessionResourceSetupItemCxtReqList);
       it < std::end(pduSessionResourceSetupItemCxtReqList); ++it) {
    PDUSessionResourceSetupRequestItem_t request = {};

    PduSessionId pDUSessionID     = {};
    std::optional<NasPdu> nAS_PDU = std::nullopt;
    SNssai s_NSSAI                = {};
    it->get(
        pDUSessionID, nAS_PDU, s_NSSAI,
        request.pduSessionResourceSetupRequestTransfer);
    pDUSessionID.get(request.pduSessionId);
    s_NSSAI.getSst(request.sNssai.sst);
    s_NSSAI.getSd(request.sNssai.sd);
    if (nAS_PDU.has_value()) {
      nAS_PDU.value().get(request.nasPdu);
    }
    list.push_back(request);
  }

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setAllowedNssai(
    const std::vector<S_Nssai>& list) {
  std::vector<SNssai> snssaiList;

  for (int i = 0; i < list.size(); i++) {
    SNssai snssai = {};
    snssai.setSst(list[i].sst);
    if (!list[i].sd.empty()) {
      snssai.setSd(list[i].sd);
    } else {
      snssai.setSd(SD_NO_VALUE);
    }
    snssaiList.push_back(snssai);
  }
  m_AllowedNssai.set(snssaiList);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AllowedNSSAI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_AllowedNSSAI;

  int ret = m_AllowedNssai.encode(ie->value.choice.AllowedNSSAI);
  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode AllowedNSSAI IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode AllowedNSSAI IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getAllowedNssai(
    std::vector<S_Nssai>& list) const {
  std::vector<SNssai> snssaiList;
  m_AllowedNssai.get(snssaiList);
  for (std::vector<SNssai>::iterator it = std::begin(snssaiList);
       it < std::end(snssaiList); ++it) {
    S_Nssai s_nssai = {};
    it->getSst(s_nssai.sst);
    it->getSd(s_nssai.sd);
    list.push_back(s_nssai);
  }

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUeSecurityCapability(
    uint16_t nrEncryptionAlgs, uint16_t nrIntegrityProtectionAlgs,
    uint16_t eutraEncryptionAlgs, uint16_t eutraIntegrityProtectionAlgs) {
  m_UeSecurityCapabilities.set(
      nrEncryptionAlgs, nrIntegrityProtectionAlgs, eutraEncryptionAlgs,
      eutraIntegrityProtectionAlgs);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UESecurityCapabilities;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities;

  int ret =
      m_UeSecurityCapabilities.encode(ie->value.choice.UESecurityCapabilities);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode UESecurityCapabilities IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode UESecurityCapabilities IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getUeSecurityCapability(
    uint16_t& nrEncryptionAlgs, uint16_t& nrIntegrityProtectionAlgs,
    uint16_t& eutraEncryptionAlgs,
    uint16_t& eutraIntegrityProtectionAlgs) const {
  if (!m_UeSecurityCapabilities.get(
          nrEncryptionAlgs, nrIntegrityProtectionAlgs, eutraEncryptionAlgs,
          eutraIntegrityProtectionAlgs))
    return false;

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setSecurityKey(
    uint8_t* key, const size_t& size) {
  size_t actual_size = (size > 256) ? 256 : size;
  m_SecurityKey.set(key, actual_size);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_SecurityKey;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_SecurityKey;

  int ret = m_SecurityKey.encode(ie->value.choice.SecurityKey);
  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode SecurityKey IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode SecurityKey IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getSecurityKey(uint8_t*& key) const {
  if (!m_SecurityKey.get(key)) return false;

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setMobilityRestrictionList(
    const PlmnId& plmn_id) {
  MobilityRestrictionList tmp = {};
  tmp.setPlmn(plmn_id);
  m_MobilityRestrictionList = std::optional<MobilityRestrictionList>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_MobilityRestrictionList;

  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_MobilityRestrictionList;

  if (!ie->value.choice.MobilityRestrictionList) ie->value.choice.MobilityRestrictionList = (Ngap_MobilityRestrictionList_t*) calloc(1, sizeof(Ngap_MobilityRestrictionList_t));
  int ret = m_MobilityRestrictionList.value().encode(
      *ie->value.choice.MobilityRestrictionList);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode MobilityRestrictionList IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode MobilityRestrictionList IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setNasPdu(const bstring& pdu) {
  NasPdu tmp = {};
  tmp.set(pdu);
  m_NasPdu = std::optional<NasPdu>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_NAS_PDU;

  int ret = m_NasPdu.value().encode(ie->value.choice.NAS_PDU);
  if (!ret) {
    oai::logger::logger_common::ngap().error("Encode NAS PDU error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode NAS PDU error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getNasPdu(bstring& pdu) const {
  if (!m_NasPdu.has_value()) return false;
  return m_NasPdu.value().get(pdu);
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUeRadioCapability(
    const bstring& ue_radio_capability) {
  UeRadioCapability tmp = {};
  tmp.set(ue_radio_capability);
  m_UeRadioCapability = std::optional<UeRadioCapability>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UERadioCapability;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UERadioCapability;

  int ret =
      m_UeRadioCapability.value().encode(ie->value.choice.UERadioCapability);
  if (!ret) {
    oai::logger::logger_common::ngap().error(
        "Encode UERadioCapability IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode UERadioCapability IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::getUeRadioCapability(
    bstring& ue_radio_capability) const {
  if (!m_UeRadioCapability.has_value()) return;
  m_UeRadioCapability.value().get(ue_radio_capability);
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setMaskedImeisv(const std::string& imeisv) {
  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_MaskedIMEISV;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_MaskedIMEISV;

  if (!ngap_utils::string_2_masked_imeisv(
          imeisv, ie->value.choice.MaskedIMEISV)) {
    oai::logger::logger_common::ngap().error("Encode MaskedIMEISV IE error!");
    oai::utils::utils::free_wrapper((void**) &ie);
    return;
  }

  int ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode MaskedIMEISV IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_InitialContextSetup &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_InitialContextSetupRequest) {
      m_InitialContextSetupRequestIes =
          &ngapPdu->choice.initiatingMessage->value.choice
               .InitialContextSetupRequest;
    } else {
      oai::logger::logger_common::ngap().error(
          "Check InitialContextSetupRequest message error!");
      return false;
    }
  } else {
    oai::logger::logger_common::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0; i < m_InitialContextSetupRequestIes->protocolIEs->list.count;
       i++) {
    Ngap_InitialContextSetupRequestIEs_t* ngap_ie =
        (Ngap_InitialContextSetupRequestIEs_t*) m_InitialContextSetupRequestIes->protocolIEs->list.array[i];
    switch (ngap_ie->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ngap_ie
                    ->criticality == Ngap_Criticality_reject &&
            ngap_ie
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_AMF_UE_NGAP_ID) {
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
        if (ngap_ie
                    ->criticality == Ngap_Criticality_reject &&
            ngap_ie
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_RAN_UE_NGAP_ID) {
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
      case Ngap_ProtocolIE_ID_id_OldAMF: {
        if (ngap_ie
                    ->criticality == Ngap_Criticality_reject &&
            ngap_ie
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_AMFName) {
          AmfName tmp = {};
          if (!tmp.decode(
                  ngap_ie
                      ->value.choice.AMFName)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP OldAMFName IE error");
            return false;
          }
          m_OldAmf = std::optional<AmfName>(tmp);
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP OldAMFName IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate: {
        if (ngap_ie
                    ->criticality == Ngap_Criticality_reject &&
            ngap_ie
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate) {
          UeAggregateMaxBitRate tmp = {};
          if (!tmp.decode(
                  ngap_ie
                      ->value.choice.UEAggregateMaximumBitRate)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP UEAggregateMaximumBitRate IE error");
            return false;
          }
          m_UeAggregateMaxBitRate = std::optional<UeAggregateMaxBitRate>(tmp);
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP UEAggregateMaximumBitRate IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CoreNetworkAssistanceInformationForInactive: {
        if (ngap_ie
                    ->criticality == Ngap_Criticality_ignore &&
            ngap_ie
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_CoreNetworkAssistanceInformationForInactive) {
          CoreNetworkAssistanceInformationForInactive tmp = {};

          if (!ngap_ie->value.choice.CoreNetworkAssistanceInformationForInactive)
            return false;
          if (!tmp.decode(
                  *ngap_ie->value.choice
                       .CoreNetworkAssistanceInformationForInactive)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP CoreNetworkAssistanceInformationForInactive "
                "IE "
                "error");
            return false;
          }
          m_CoreNetworkAssistanceInformationForInactive =
              std::optional<CoreNetworkAssistanceInformationForInactive>(tmp);
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP CoreNetworkAssistanceInformation IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_GUAMI: {
        if (ngap_ie
                    ->criticality == Ngap_Criticality_reject &&
            ngap_ie
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_GUAMI) {
          if (!m_Guami.decode(
                  ngap_ie
                      ->value.choice.GUAMI)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP GUAMI IE error");

            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP GUAMI IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtReq: {
        if (ngap_ie
                    ->criticality == Ngap_Criticality_reject &&
            ngap_ie
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_PDUSessionResourceSetupListCxtReq) {
          PduSessionResourceSetupListCxtReq tmp = {};
          if (!tmp.decode(
                  ngap_ie
                      ->value.choice.PDUSessionResourceSetupListCxtReq)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP PDUSessionResourceSetupListCxtReq IE error");
            return false;
          }
          m_PduSessionResourceSetupRequestList =
              std::optional<PduSessionResourceSetupListCxtReq>(tmp);
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP PDUSessionResourceSetupListCxtReq IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_AllowedNSSAI: {
        if (ngap_ie
                    ->criticality == Ngap_Criticality_reject &&
            ngap_ie
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_AllowedNSSAI) {
          if (!m_AllowedNssai.decode(
                  ngap_ie
                      ->value.choice.AllowedNSSAI)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP AllowedNSSAI IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP AllowedNSSAI IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UESecurityCapabilities: {
        if (ngap_ie
                    ->criticality == Ngap_Criticality_reject &&
            ngap_ie
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities) {
          if (!m_UeSecurityCapabilities.decode(
                  ngap_ie
                      ->value.choice.UESecurityCapabilities)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP UESecurityCapabilities IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP UESecurityCapabilities IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_SecurityKey: {
        if (ngap_ie
                    ->criticality == Ngap_Criticality_reject &&
            ngap_ie
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_SecurityKey) {
          if (!m_SecurityKey.decode(
                  ngap_ie
                      ->value.choice.SecurityKey)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP SecurityKey IE error");
            return false;
          }
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP SecurityKey IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (ngap_ie
                    ->criticality == Ngap_Criticality_ignore &&
            ngap_ie
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_NAS_PDU) {
          NasPdu tmp = {};
          if (!tmp.decode(
                  ngap_ie
                      ->value.choice.NAS_PDU)) {
            oai::logger::logger_common::ngap().error(
                "Decoded NGAP NasPdu IE error");
            return false;
          }
          m_NasPdu = std::optional<NasPdu>(tmp);
        } else {
          oai::logger::logger_common::ngap().error(
              "Decoded NGAP NAS_PDU IE error");
          return false;
        }
      } break;

      default: {
        oai::logger::logger_common::ngap().error(
            "Decoded NGAP Message PDU error");
        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setManagementBasedMdtPlmnList(
    const ManagementBasedMdtPlmnList& value) {
  m_ManagementBasedMdtPlmnList = std::make_optional<ManagementBasedMdtPlmnList>(value);
  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_ManagementBasedMDTPLMNList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_MDTPLMNList;
  if (!m_ManagementBasedMdtPlmnList.value().encode(
          ie->value.choice.MDTPLMNList)) {
    oai::logger::logger_common::ngap().error(
        "Encode ManagementBasedMdtPlmnList IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode ManagementBasedMdtPlmnList IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setTimeSynchronisationAssistanceInfo(
    const TimeSynchronisationAssistanceInfo& value) {
  m_TimeSynchronisationAssistanceInfo =
      std::make_optional<TimeSynchronisationAssistanceInfo>(value);
  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_TimeSyncAssistanceInfo;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_TimeSyncAssistanceInfo;
  if (!m_TimeSynchronisationAssistanceInfo.value().encode(
          ie->value.choice.TimeSyncAssistanceInfo)) {
    oai::logger::logger_common::ngap().error(
        "Encode TimeSynchronisationAssistanceInfo IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode TimeSynchronisationAssistanceInfo IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setQmcConfigInfo(
    const QmcConfigInfo& value) {
  m_QmcConfigInfo = std::make_optional<QmcConfigInfo>(value);
  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_QMCConfigInfo;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_QMCConfigInfo;
  if (!m_QmcConfigInfo.value().encode(ie->value.choice.QMCConfigInfo)) {
    oai::logger::logger_common::ngap().error("Encode QmcConfigInfo IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error("Encode QmcConfigInfo IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setTargetNssaiInformation(
    const TargetNssaiInformation& value) {
  m_TargetNssaiInformation = std::make_optional<TargetNssaiInformation>(value);
  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_TargetNSSAIInformation;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_TargetNSSAIInformation;
  if (!m_TargetNssaiInformation.value().encode(
          ie->value.choice.TargetNSSAIInformation)) {
    oai::logger::logger_common::ngap().error(
        "Encode TargetNssaiInformation IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode TargetNssaiInformation IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUeSliceMaximumBitRateList(
    const UeSliceMaximumBitRateList& value) {
  m_UeSliceMaximumBitRateList =
      std::make_optional<UeSliceMaximumBitRateList>(value);
  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UESliceMaximumBitRateList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UESliceMaximumBitRateList;
  if (!m_UeSliceMaximumBitRateList.value().encode(
          ie->value.choice.UESliceMaximumBitRateList)) {
    oai::logger::logger_common::ngap().error(
        "Encode UeSliceMaximumBitRateList IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode UeSliceMaximumBitRateList IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setFiveGProSeAuthorized(
    const FiveGProSeAuthorized& value) {
  m_FiveGProSeAuthorized = std::make_optional<FiveGProSeAuthorized>(value);
  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_FiveG_ProSeAuthorized;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_FiveG_ProSeAuthorized;
  if (!m_FiveGProSeAuthorized.value().encode(
          ie->value.choice.FiveG_ProSeAuthorized)) {
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSeAuthorized IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSeAuthorized IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setFiveGProSeUePC5AggregateMaximumBitRate(
    const FiveGProSeUePC5AggregateMaximumBitRate& value) {
  m_FiveGProSeUePC5AggregateMaximumBitRate =
      std::make_optional<FiveGProSeUePC5AggregateMaximumBitRate>(value);
  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_FiveG_ProSeUEPC5AggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_NRUESidelinkAggregateMaximumBitrate_1;
  if (!m_FiveGProSeUePC5AggregateMaximumBitRate.value().encode(
          ie->value.choice.NRUESidelinkAggregateMaximumBitrate_1)) {
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSeUePC5AggregateMaximumBitRate IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSeUePC5AggregateMaximumBitRate IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setFiveGProSePC5QoSParameters(
    const FiveGProSePC5QoSParameters& value) {
  m_FiveGProSePC5QoSParameters =
      std::make_optional<FiveGProSePC5QoSParameters>(value);
  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_FiveG_ProSePC5QoSParameters;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_FiveG_ProSePC5QoSParameters;
  if (!m_FiveGProSePC5QoSParameters.value().encode(
          ie->value.choice.FiveG_ProSePC5QoSParameters)) {
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSePC5QoSParameters IE error");
    free(ie);
    return;
  }
  int ret = ASN_SEQUENCE_ADD(
      &m_InitialContextSetupRequestIes->protocolIEs->list, ie);
  if (ret != 0)
    oai::logger::logger_common::ngap().error(
        "Encode FiveGProSePC5QoSParameters IE error");
}

}  // namespace oai::ngap
