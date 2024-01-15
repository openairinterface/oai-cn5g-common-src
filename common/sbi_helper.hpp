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

#include <boost/algorithm/string.hpp>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

#include "common_defs.h"
#include "if.hpp"
#include "logger_base.hpp"
#include "string.hpp"

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

constexpr auto kDefaultSbiApiVersion = "v1";

typedef struct interface_cfg_s {
  std::string if_name;
  struct in_addr addr4;
  struct in_addr network4;
  struct in6_addr addr6;
  unsigned int mtu;
  unsigned int port;
  std::optional<std::string> api_version;

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["if_name"]     = this->if_name;
    json_data["addr4"]       = inet_ntoa(this->addr4);
    json_data["network4"]    = inet_ntoa(this->network4);
    char str_addr6[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &this->addr6, str_addr6, sizeof(str_addr6));
    json_data["addr6"] = str_addr6;
    json_data["mtu"]   = this->mtu;
    json_data["port"]  = this->port;
    if (api_version.has_value())
      json_data["api_version"] = this->api_version.value();
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->if_name         = json_data["if_name"].get<std::string>();
    std::string addr4_str = {};
    addr4_str             = json_data["addr4"].get<std::string>();

    if (boost::iequals(addr4_str, "read")) {
      if (get_inet_addr_infos_from_iface(
              this->if_name, this->addr4, this->network4, this->mtu)) {
        oai::logger::logger_registry::get_logger(LOGGER_COMMON)
            .error(
                "Could not read %s network interface configuration",
                this->if_name);
        return;
      }
    } else {
      IPV4_STR_ADDR_TO_INADDR(
          util::trim(addr4_str).c_str(), this->addr4,
          "BAD IPv4 ADDRESS FORMAT FOR INTERFACE !");

      std::string network4_str = json_data["network4"].get<std::string>();
      IPV4_STR_ADDR_TO_INADDR(
          util::trim(network4_str).c_str(), this->network4,
          "BAD IPv4 ADDRESS FORMAT FOR INTERFACE !");
      // TODO: addr6
      this->mtu  = json_data["mtu"].get<int>();
      this->port = json_data["port"].get<int>();

      if (json_data.find("api_version") != json_data.end()) {
        this->api_version = std::make_optional<std::string>(
            json_data["api_version"].get<std::string>());
      }
    }
  }

} interface_cfg_t;

typedef struct nf_addr_s {
  struct in_addr ipv4_addr;
  unsigned int port;
  std::string api_version;
  std::string uri_root;

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["uri_root"]    = this->uri_root;
    json_data["api_version"] = this->api_version;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->uri_root    = json_data["uri_root"].get<std::string>();
    this->api_version = json_data["api_version"].get<std::string>();
  }

} nf_addr_t;

class sbi_helper {
 public:
  // AMF: Communication Service
  static inline const std::string AmfCommBase = "/namf-comm/";
  static inline const std::string AmfCommPathUeContextContextId =
      "/ue-contexts/:ueContextId";
  static inline const std::string AmfCommPathUeContextContextIdRelease =
      "/ue-contexts/:ueContextId/release";
  static inline const std::string AmfCommPathUeContextContextIdAssignEbi =
      "/ue-contexts/:ueContextId/assign-ebi";
  static inline const std::string AmfCommPathUeContextContextIdTransfer =
      "/ue-contexts/:ueContextId/transfer";
  static inline const std::string AmfCommPathUeContextContextIdTransferUpdate =
      "/ue-contexts/:ueContextId/transfer-update";
  static inline const std::string AmfCommPathUeContextContextIdRelocate =
      "/ue-contexts/:ueContextId/relocate";
  static inline const std::string AmfCommPathUeContextContextIdN1N2Message =
      "/ue-contexts/:ueContextId/n1-n2-messages";
  static inline const std::string
      AmfCommPathUeContextContextIdN1N2MessageSubscriptions =
          "/ue-contexts/:ueContextId/n1-n2-messages/subscriptions";
  static inline const std::string
      AmfCommPathUeContextContextIdN1N2MessageSubscriptionsSubscriptionId =
          "/ue-contexts/:ueContextId/n1-n2-messages/subscriptions/"
          ":subscriptionId";
  static inline const std::string AmfCommPathNonUeN1N2MessageTransfer =
      "/non-ue-n2-messages/transfer";

  static inline const std::string AmfCommPathNonUeN1N2MessageSubscriptions =
      "/non-ue-n2-messages/subscriptions";
  static inline const std::string
      AmfCommPathNonUeN1N2MessageSubscriptionsn2NotifySubscriptionId =
          "/non-ue-n2-messages/subscriptions/:n2NotifySubscriptionId";
  static inline const std::string AmfCommPathSubscriptions = "/subscriptions";
  static inline const std::string AmfCommPathSubscriptionsSubscriptionId =
      "/subscriptions/:subscriptionId";

  // AMF: Event Exposure Service
  static inline const std::string AmfEvtsBase              = "/namf-evts/";
  static inline const std::string AmfEvtsPathSubscriptions = "/subscriptions";
  static inline const std::string AmfEvtsPathSubscriptionsSubscriptionId =
      "/subscriptions/:subscriptionId";

  // AMF: Location Service
  static inline const std::string AmflocBase = "/namf-loc/";
  static inline const std::string AmflocPathUeContextIdProvidePosInfo =
      "/:ueContextId/provide-pos-info";
  static inline const std::string AmflocPathUeContextIdProvideLocInfo =
      "/:ueContextId/provide-loc-info";
  static inline const std::string AmflocPathUeContextIdCancelPosInfo =
      "/:ueContextId/cancel-pos-info";

  // TODO: AMF: Mobile Terminated Service

  // AUSF: UEAuthentication
  static inline const std::string AusfAuthBase = "/nausf-auth/";
  static inline const std::string AusfAuthPathUeAuthentications =
      "/ue-authentications";
  static inline const std::string
      AusfAuthPathUeAuthentications5gAkaConfirmation =
          "/ue-authentications/:authCtxId/5g-aka-confirmation";
  static inline const std::string AusfAuthPathUeAuthenticationsEapSession =
      "/ue-authentications/:authCtxId/eap-session";

  // TODO: AUSF: SoR Protection
  // TODO: AUSF: UPU Protection

  // NRF: NF Management Service
  static inline const std::string NrfNfmBase = "/nnrf-nfm/";
  static inline const std::string NrfNfmPathNfInstancesNfInstanceId =
      "/nf-instances/:nfInstanceID";
  static inline const std::string NrfNfmPathNfInstances   = "/nf-instances";
  static inline const std::string NrfNfmPathSubscriptions = "/subscriptions";
  static inline const std::string NrfNfmPathSubscriptionsSubscriptionId =
      "/subscriptions/:subscriptionID";

  // NRF: NF Discovery Service
  static inline const std::string NrfDiscBase            = "/nnrf-disc/";
  static inline const std::string NrfDiscPathNfInstances = "/nf-instances";
  static inline const std::string NrfDiscPathSearchesSearchId =
      "/searches/:searchId";
  static inline const std::string NrfDiscPathSearchesSearchIdComplete =
      "/searches/:searchId/complete";
  static inline const std::string NrfDiscPathScpDomainRoutingInfo =
      "/scp-domain-routing-info";
  static inline const std::string NrfDiscPathScpDomainRoutingInfoSubs =
      "/scp-domain-routing-info-subs";
  static inline const std::string
      NrfDiscPathScpDomainRoutingInfoSubsSubscriptionId =
          "/scp-domain-routing-info-subs/:subscriptionID";

  // TODO: NRF: Access Token Service
  // TODO: NRF: Bootstrapping Service

  // TODO: NSSF

  // TODO: PCF

  // TODO: SMF

  // UDM: Subscriber Data Management
  static inline const std::string UdmSdmBase       = "/nudm-sdm/";
  static inline const std::string UdmSdmSupi       = "/:supi";
  static inline const std::string UdmSdmSupiNssai  = "/:supi/nssai";
  static inline const std::string UdmSdmSupiAmData = "/:supi/am-data";
  static inline const std::string UdmSdmSupiSmfSelData =
      "/:supi/smf-select-data";
  static inline const std::string UdmSdmSupiUeCtxInSmfData =
      "/:supi/ue-context-in-smf-data";
  static inline const std::string UdmSdmSupiUeCtxInSmsfData =
      "/:supi/ue-context-in-smsf-data";
  static inline const std::string UdmSdmSupiTraceConfigData =
      "/:supi/trace-data";
  static inline const std::string UdmSdmSupiSmData      = "/:supi/sm-data";
  static inline const std::string UdmSdmSupiSmsData     = "/:supi/sms-data";
  static inline const std::string UdmSdmSupiSmsMngtData = "/:supi/sms-mng-data";
  static inline const std::string UdmSdmSupiSdmSubscriptions =
      "/:supi/sdm-subscriptions";
  static inline const std::string UdmSdmSupiSdmSubscriptionsSubscriptionId =
      "/:supi/sdm-subscriptions/:subscriptionId";
  static inline const std::string UdmSdmUeIdSupiOrGpsi =
      "/:ueId/id-translation-result";
  static inline const std::string UdmSdmSupiAmDataSorAckInfo =
      "/:supi/am-data/sor-ack";
  static inline const std::string UdmSdmSupiAmDataUpuAck =
      "/:supi/am-data/upu-ack";
  static inline const std::string UdmSdmSupiAmDataSubscribedSNssaisAck =
      "/:supi/am-data/subscribed-snssais-ack";
  static inline const std::string UdmSdmSupiAmDataCagAck =
      "/:supi/am-data/cag-ack";
  static inline const std::string UdmSdmSharedData = "/shared-data";
  static inline const std::string UdmSdmSharedDataSubscriptions =
      "/shared-data-subscriptions";
  static inline const std::string UdmSdmSharedDataSubscriptionsSubscriptionId =
      "/shared-data-subscriptions/:subscriptionId";
  static inline const std::string UdmSdmGroupDataGroupIdentifiers =
      "/group-data/group-identifiers";

  // TODO: UDM: UE Authentication Service
  static inline const std::string UdmUeAuBase = "/nudm-ueau/";
  static inline const std::string UdmUeAuPathGenerateAuthData =
      "/:supiOrSuci/security-information/generate-auth-data";
  static inline const std::string UdmUeAuPathConfirmAuth = "/:supi/auth-events";

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
      ":realmId/:storageId/records/:recordId/blocks/:blockId";
  static inline const std::string UdsfDrBlockCRUDApiList =
      ":realmId/:storageId/records/:recordId/blocks";
  static inline const std::string UdsfDrMetaSchemaCRUDApi =
      "/:realmId/:storageId/meta-schemas/:schemaId";
  static inline const std::string UdsfDrNotificationSubscriptionCRUDApi =
      "/:realmId/:storageId/subs-to-notify/:subscriptionId";
  static inline const std::string UdsfDrNotificationSubscriptionsCRUDApi =
      "/:realmId/:storageId/subs-to-notify";
  static inline const std::string UdsfDrRecordCRUDApi =
      "/:realmId/:storageId/records/:recordId";
  static inline const std::string UdsfDrRecordCRUDApiList =
      "/:realmId/:storageId/records";
  static inline const std::string UdsfDrRecordCRUDApiMeta =
      "/:realmId/:storageId/records/:recordId/meta";

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

  static void get_fmt_format_form(
      const std::string& input_str, std::string& output_str);
};
