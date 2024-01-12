/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <nlohmann/json.hpp>
#include <string>

// TODO: namespace oai::sbi

constexpr auto kNumberOfFirstConnectionRetries       = 10;
constexpr auto kNumberOfConnectionRetries            = 3;
constexpr auto kNfDefaultCurlTimeout                 = 1000;  // in Millisecond
constexpr auto kNumberOfCurlRetries                  = 3;
constexpr auto kBaseTimeIntervalBetweenCurlRetries   = 1000;  // in microsecond
constexpr auto kNumberOfNfRegisterRetries            = 3;
constexpr auto kNumberOfNfDeregisterRetries          = 3;
constexpr auto kTimeIntervalBetweenNfRegisterRetries = 1;    // in seconds
constexpr auto kTimeIntervalBetweenNfDeregisterRetries = 1;  // in seconds

typedef struct interface_cfg_s {
  std::string if_name;
  struct in_addr addr4;
  struct in_addr network4;
  struct in6_addr addr6;
  unsigned int port;
  std::string api_version;
} interface_cfg_t;

typedef struct nf_addr_s {
  struct in_addr ipv4_addr;
  unsigned int port;
  std::string api_version;
  std::string uri_root;
} nf_addr_t;

class sbi_helper {
 public:
  // AMF: Communication Service
  static inline const std::string AmfCommBase = "/namf-comm/";
  static inline const std::string AmfCommPathUeContextContextId =
      "/ue-contexts/{ueContextId}";
  static inline const std::string AmfCommPathUeContextContextIdRelease =
      "/ue-contexts/{ueContextId}/release";
  static inline const std::string AmfCommPathUeContextContextIdAssignEbi =
      "/ue-contexts/{ueContextId}/assign-ebi";
  static inline const std::string AmfCommPathUeContextContextIdTransfer =
      "/ue-contexts/{ueContextId}/transfer";
  static inline const std::string AmfCommPathUeContextContextIdTransferUpdate =
      "/ue-contexts/{ueContextId}/transfer-update";
  static inline const std::string AmfCommPathUeContextContextIdRelocate =
      "/ue-contexts/{ueContextId}/relocate";
  static inline const std::string AmfCommPathUeContextContextIdN1N2Message =
      "/ue-contexts/{ueContextId}/n1-n2-messages";
  static inline const std::string
      AmfCommPathUeContextContextIdN1N2MessageSubscriptions =
          "/ue-contexts/{ueContextId}/n1-n2-messages/subscriptions";
  static inline const std::string
      AmfCommPathUeContextContextIdN1N2MessageSubscriptionsSubscriptionId =
          "/ue-contexts/{ueContextId}/n1-n2-messages/subscriptions/"
          "{subscriptionId}";
  static inline const std::string AmfCommPathNonUeN1N2MessageTransfer =
      "/non-ue-n2-messages/transfer";

  static inline const std::string AmfCommPathNonUeN1N2MessageSubscriptions =
      "/non-ue-n2-messages/subscriptions";
  static inline const std::string
      AmfCommPathNonUeN1N2MessageSubscriptionsn2NotifySubscriptionId =
          "/non-ue-n2-messages/subscriptions/{n2NotifySubscriptionId}";
  static inline const std::string AmfCommPathSubscriptions = "/subscriptions";
  static inline const std::string AmfCommPathSubscriptionsSubscriptionId =
      "/subscriptions/{subscriptionId}";

  // AMF: Event Exposure Service
  static inline const std::string AmfEvtsBase              = "/namf-evts/";
  static inline const std::string AmfEvtsPathSubscriptions = "/subscriptions";
  static inline const std::string AmfEvtsPathSubscriptionsSubscriptionId =
      "/subscriptions/{subscriptionId}";

  // AMF: Location Service
  static inline const std::string AmflocBase = "/namf-loc/";
  static inline const std::string AmflocPathUeContextIdProvidePosInfo =
      "/{ueContextId}/provide-pos-info";
  static inline const std::string AmflocPathUeContextIdProvideLocInfo =
      "/{ueContextId}/provide-loc-info";
  static inline const std::string AmflocPathUeContextIdCancelPosInfo =
      "/{ueContextId}/cancel-pos-info";

  // TODO: AMF: Mobile Terminated Service

  // AUSF: UEAuthentication
  static inline const std::string AusfAuthBase = "/nausf-auth/";
  static inline const std::string AusfAuthPathUeAuthentications =
      "/ue-authentications";
  static inline const std::string AusfAuthPathUeAuthentications5gAkaConfirmation =
      "/ue-authentications/{}/5g-aka-confirmation";  // /ue-authentications/{authCtxId}/5g-aka-confirmation
  static inline const std::string AusfAuthPathUeAuthenticationsEapSession =
      "/ue-authentications/{}/eap-session";  // /ue-authentications/{authCtxId}/eap-session:

  // TODO: AUSF: SoR Protection
  // TODO: AUSF: UPU Protection

  // NRF: NF Management Service
  static inline const std::string NrfNfmBase = "/nnrf-nfm/";
  static inline const std::string NrfNfmPathNfInstancesNfInstanceId =
      "/nf-instances/{}";  // nf-instances/{InstanceId}
  static inline const std::string NrfNfmPathNfInstances   = "/nf-instances";
  static inline const std::string NrfNfmPathSubscriptions = "/subscriptions";
  static inline const std::string NrfNfmPathSubscriptionsSubscriptionId =
      "/subscriptions/{}";  // subscriptions/{subscriptionID}

  // NRF: NF Discovery Service
  static inline const std::string NrfDiscBase            = "/nnrf-disc/";
  static inline const std::string NrfDiscPathNfInstances = "/nf-instances";
  static inline const std::string NrfDiscPathSearchesSearchId =
      "/searches/{}";  // searches/{searchId}
  static inline const std::string NrfDiscPathSearchesSearchIdComplete =
      "/searches/{}/complete";  // searches/{searchId}/complete
  static inline const std::string NrfDiscPathScpDomainRoutingInfo =
      "/scp-domain-routing-info";
  static inline const std::string NrfDiscPathScpDomainRoutingInfoSubs =
      "/scp-domain-routing-info-subs";
  static inline const std::string
      NrfDiscPathScpDomainRoutingInfoSubsSubscriptionId =
          "/scp-domain-routing-info-subs/{}";  // scp-domain-routing-info-subs/{subscriptionID}

  // TODO: NRF: Access Token Service
  // TODO: NRF: Bootstrapping Service

  // TODO: NSSF

  // TODO: PCF

  // TODO: SMF

  // UDM: Subscriber Data Management
  static inline const std::string UdmSdmBase = "/nudm-sdm/";
  static inline const std::string UdmSdmSupi = "/{}";  // /{supi}: GetDataSets
  static inline const std::string UdmSdmSupiNssai =
      "/{}/nssai";  // /{supi}/nssai: GetNSSAI
  static inline const std::string UdmSdmSupiAmData =
      "/{}/am-data";  // /{supi}/am-data:GetAmData
  static inline const std::string UdmSdmSupiSmfSelData =
      "/{}/smf-select-data";  // /{supi}/smf-select-data":GetSmfSelData
  static inline const std::string UdmSdmSupiUeCtxInSmfData =
      "/{}/ue-context-in-smf-data";  // /{supi}/ue-context-in-smf-data:GetUeCtxInSmfData
  static inline const std::string UdmSdmSupiUeCtxInSmsfData =
      "/{}/ue-context-in-smsf-data";  // /{supi}/ue-context-in-smsf-data:
                                      // GetUeCtxInSmsfData
  static inline const std::string UdmSdmSupiTraceConfigData =
      "/{}/trace-data";  // {supi}/trace-data:GetTraceConfigData
  static inline const std::string UdmSdmSupiSmData =
      "/{}/sm-data";  // /{supi}/sm-data:GetSmData
  static inline const std::string UdmSdmSupiSmsData =
      "/{}/sms-data";  // /{supi}/sms-data: GetSmsData
  static inline const std::string UdmSdmSupiSmsMngtData =
      "/{}/sms-mng-data";  // /{supi}/sms-mng-data:GetSmsMngtData
  static inline const std::string UdmSdmSupiSdmSubscriptions =
      "/{}/sdm-subscriptions";  // /{supi}/sdm-subscriptions:Subscribe
  static inline const std::string UdmSdmSupiSdmSubscriptionsSubscriptionId =
      "/{}/sdm-subscriptions/{}";  // /{supi}/sdm-subscriptions/{subscriptionId}:Unsubscribe
  static inline const std::string UdmSdmUeIdSupiOrGpsi =
      "/{ueId}/id-translation-result";  // /{ueId}/id-translation-result:GetSupiOrGpsi
  static inline const std::string UdmSdmSupiAmDataSorAckInfo =
      "/{}/am-data/sor-ack";  // /{supi}/am-data/sor-ack:SorAckInfo
  static inline const std::string UdmSdmSupiAmDataUpuAck =
      "/{}/am-data/upu-ack";  // /{supi}/am-data/upu-ack: UpuAck
  static inline const std::string UdmSdmSupiAmDataSubscribedSNssaisAck =
      "/{}/am-data/subscribed-snssais-ack";  // /{supi}/am-data/subscribed-snssais-ack:S-NSSAIs
                                             // Ack
  static inline const std::string UdmSdmSupiAmDataCagAck =
      "/{}/am-data/cag-ack";  // /{supi}/am-data/cag-ack:CAG Ack
  static inline const std::string UdmSdmSharedData =
      "/shared-data";  // /shared-data:GetSharedData
  static inline const std::string UdmSdmSharedDataSubscriptions =
      "/shared-data-subscriptions";  // /shared-data-subscriptions:SubscribeToSharedData
  static inline const std::string UdmSdmSharedDataSubscriptionsSubscriptionId =
      "/shared-data-subscriptions/{}";  // /shared-data-subscriptions/{subscriptionId}:UnsubscribeForSharedData
  static inline const std::string UdmSdmGroupDataGroupIdentifiers =
      "/group-data/group-identifiers";  // /group-data/group-identifiers:GetGroupIdentifiers

  // TODO: UDM: UE Authentication Service
  static inline const std::string UdmUeAuBase = "/nudm-ueau/";
  static inline const std::string UdmUeAuPathGenerateAuthData =
      "/{}/security-information/generate-auth-data";  // /{supiOrSuci}/security-information/generate-auth-data:GenerateAuthData
  static inline const std::string UdmUeAuPathConfirmAuth =
      "/{}/auth-events";  // /{supi}/auth-events:ConfirmAuth

  // TODO: UDM UE Context Management Service
  // TODO: UDM Event Exposure Service
  // TODO: UDM Parameter Provision Service
  // TODO: UDM NIDD Authorization

  // UDR: Data Repository Service
  static inline const std::string UdrDataRepositoryBase = "/nudsf-dr/";
  static inline const std::string UdrAuthenticationSubscriptionEndpoint =
      "/authentication-data/authentication-subscription";

  // UDSF: Data Repository
  static inline const std::string UdsfDrBase = "/nudsf-dr/";
  static inline const std::string UdsfDrBlockCRUDApi =
      "/{}/{}/records/{}/blocks/{}";  //{realmId}/{storageId}/records/{recordId}/blocks/{blockId}
  static inline const std::string UdsfDrBlockCRUDApiList =
      "/{}/{}/records/{}/blocks";  //{realmId}/{storageId}/records/{recordId}/blocks
  static inline const std::string UdsfDrMetaSchemaCRUDApi =
      "/{}/{}/meta-schemas/{}";  //{realmId}/{storageId}/meta-schemas/{schemaId}
  static inline const std::string UdsfDrNotificationSubscriptionCRUDApi =
      "/{}/{}/subs-to-notify/{}";  //{realmId}/{storageId}/subs-to-notify/{subscriptionId}
  static inline const std::string UdsfDrNotificationSubscriptionsCRUDApi =
      "/{}/{}/subs-to-notify";  ///{realmId}/{storageId}/subs-to-notify
  static inline const std::string UdsfDrRecordCRUDApi =
      "/{}/{}/records/{}";  ///{realmId}/{storageId}/records/{recordId}
  static inline const std::string UdsfDrRecordCRUDApiList =
      "/{}/{}/records";  ///{realmId}/{storageId}/records
  static inline const std::string UdsfDrRecordCRUDApiMeta =
      "/{}/{}/records/{}/meta";  ///{realmId}/{storageId}/records/{recordId}/meta

  // TODO: UDSF Timer

  /*
   * Get NRF Nfm API Root
   * @param [const nf_addr_t& ] nrf_addr: NRF's Addr info
   * @param [std::string& ] api_root: NRF's API Root
   * @return void
   */
  static void get_nrf_nfm_api_root(
      const nf_addr_t& nrf_addr, std::string& nrf__root);

  /*
   * Get NRF NF Register URI
   * @param [const nf_addr_t& ] nrf_addr: NRF's Addr info
   * @param [const std::string& ] nf_instance: NF instance Id
   * @param [std::string& ] uri: NRF NF Register URI
   * @return void
   */
  static void get_nrf_nf_instance_uri(
      const nf_addr_t& nrf_addr, const std::string& nf_instance,
      std::string& uri);
};
