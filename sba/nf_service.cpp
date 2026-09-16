/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "nf_service.hpp"

#include <boost/bind/bind.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include "logger_base.hpp"
#include "PatchItem.h"
#include "http_client.hpp"

using namespace oai::sba;
using namespace boost::placeholders;

//------------------------------------------------------------------------------
nf_service::nf_service(
    const std::shared_ptr<nf_event>& ev,
    const std::shared_ptr<oai::sba::http_client>& client_inst)
    : event_sub_(ev), http_client_inst_(client_inst) {
  generate_uuid();
}

//------------------------------------------------------------------------------
nf_service::~nf_service() {
  if (task_connection.connected()) task_connection.disconnect();
  if (retry_nrf_registration_task_connection.connected())
    retry_nrf_registration_task_connection.disconnect();
}

//---------------------------------------------------------------------------------------------
void nf_service::generate_uuid() {
  nf_instance_id = to_string(boost::uuids::random_generator()());
}

//---------------------------------------------------------------------------------------------
void nf_service::generate_nf_profile() {
  // TODO:
}

//---------------------------------------------------------------------------------------------
bool nf_service::register_to_nrf(
    const oai::common::sbi::nf_addr_t& nrf_addr,
    const nlohmann::json& nf_profile) {
  nrf_addr_   = std::make_optional<oai::common::sbi::nf_addr_t>(nrf_addr);
  nf_profile_ = std::make_optional<nlohmann::json>(nf_profile);
  return send_nf_registration();
}

//---------------------------------------------------------------------------------------------
bool nf_service::send_nf_registration() {
  if (!nrf_registration_enabled()) return true;
  if (!nrf_addr_.has_value()) return false;
  if (!nf_profile_.has_value()) return false;

  std::string nrf_uri = {};
  oai::common::sbi::sbi_helper::get_nrf_nf_instance_uri(
      nrf_addr_.value(), nf_instance_id, nrf_uri);
  oai::logger::logger_common::common().info(
      "Sending NF registration request to NRF, NRF's URI: %s", nrf_uri);

  oai::sba::request http_request = http_client_inst_->prepare_json_request(
      nrf_uri, nf_profile_.value().dump());
  auto http_response = send_with_policy(
      nrf_call_kind::registration, oai::common::sbi::method_e::PUT,
      http_request);

  const bool registration_success = registration_succeeded(http_response);
  on_registration_outcome(registration_success, http_response);
  return registration_success;
}

//---------------------------------------------------------------------------------------------
bool nf_service::registration_succeeded(const oai::sba::response& resp) const {
  if ((resp.status_code != oai::common::sbi::http_status_code::OK) and
      (resp.status_code != oai::common::sbi::http_status_code::CREATED)) {
    oai::logger::logger_common::common().warn(
        "Could not get response from NRF, try again ...");
    return false;
  }
  try {
    // TODO: use Heart-beat timer interval returned from NRF
    nlohmann::json response_data = nlohmann::json::parse(resp.body);
    auto it                      = response_data.find("nfStatus");
    if (it != response_data.end() &&
        it->get<std::string>().compare("REGISTERED") == 0) {
      return true;
    }
  } catch (nlohmann::json::exception& e) {
  }
  oai::logger::logger_common::common().warn(
      "NF Registration procedure failed, try again ...");
  return false;
}

//---------------------------------------------------------------------------------------------
void nf_service::on_registration_outcome(
    bool success, const oai::sba::response& resp) {
  (void) resp;
  if (success) {
    start_event_nf_heartbeat();
    stop_nrf_registration_retry();
  } else {
    start_nrf_registration_retry();
  }
}

//---------------------------------------------------------------------------------------------
oai::sba::response nf_service::send_with_policy(
    nrf_call_kind kind, const oai::common::sbi::method_e& method,
    const oai::sba::request& req) {
  (void) kind;
  return http_client_inst_->send_http_request(method, req);
}

//---------------------------------------------------------------------------------------------
bool nf_service::deregister_to_nrf() {
  if (!nrf_registration_enabled()) return true;
  if (!nrf_addr_.has_value()) return false;

  std::string nrf_uri = {};
  oai::common::sbi::sbi_helper::get_nrf_nf_instance_uri(
      nrf_addr_.value(), nf_instance_id, nrf_uri);
  oai::logger::logger_common::common().info(
      "Sending NF Deregistration request");

  oai::sba::request http_request =
      http_client_inst_->prepare_json_request(nrf_uri);
  auto http_response = send_with_policy(
      nrf_call_kind::deregistration, oai::common::sbi::method_e::DELETE,
      http_request);

  if (http_response.status_code ==
      oai::common::sbi::http_status_code::NO_CONTENT) {
    oai::logger::logger_common::common().info(
        "NF Deregistration procedure successful");
    return true;
  }
  oai::logger::logger_common::common().info(
      "NF Deregistration procedure failed");
  return false;
}

//---------------------------------------------------------------------------------------------
void nf_service::start_event_nf_heartbeat(uint64_t heartbeat_seconds) {
  // get current time
  uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();
  const uint64_t interval = heartbeat_seconds * 1000;  // convert sec to msec

  // The task tick carries only a timestamp, so the NRF address is bound by
  // value from the address retained at registration time.
  task_connection = event_sub_->subscribe_task_nf_heartbeat(
      boost::bind(&nf_service::trigger_nf_heartbeat_procedure, this, _1),
      interval, ms + interval);
}

//---------------------------------------------------------------------------------------------
void nf_service::trigger_nf_heartbeat_procedure(uint64_t ms) {
  (void) ms;
  if (!nrf_addr_.has_value()) return;

  oai::_3gpp::model::PatchItem patch_item = {};
  std::vector<oai::_3gpp::model::PatchItem> patch_items;
  //{"op":"replace","path":"/nfStatus", "value": "REGISTERED"}
  oai::_3gpp::model::PatchOperation op;
  op.setEnumValue(
      oai::_3gpp::model::PatchOperation_anyOf::ePatchOperation_anyOf::REPLACE);
  patch_item.setOp(op);
  patch_item.setPath("/nfStatus");
  patch_item.setValue("REGISTERED");
  patch_items.push_back(patch_item);
  oai::logger::logger_common::common().info("Sending NF heartbeat request");

  nlohmann::json json_data = nlohmann::json::array();
  for (auto i : patch_items) {
    nlohmann::json item = {};
    to_json(item, i);
    json_data.push_back(item);
  }

  std::string nrf_uri = {};
  oai::common::sbi::sbi_helper::get_nrf_nf_instance_uri(
      nrf_addr_.value(), nf_instance_id, nrf_uri);

  oai::sba::request http_request =
      http_client_inst_->prepare_json_request(nrf_uri, json_data.dump());
  auto http_response = send_with_policy(
      nrf_call_kind::heartbeat, oai::common::sbi::method_e::PATCH,
      http_request);

  if ((http_response.status_code == oai::common::sbi::http_status_code::OK) or
      (http_response.status_code ==
       oai::common::sbi::http_status_code::NO_CONTENT)) {
    // TODO: process the response
  } else {
    oai::logger::logger_common::common().info(
        "NF Heartbeat procedure failed, try to register again");
    if (task_connection.connected()) task_connection.disconnect();
    send_nf_registration();
  }
}

//---------------------------------------------------------------------------------------------
void nf_service::start_nrf_registration_retry() {
  if (!retry_nrf_registration_task_connection.connected()) {
    // get current time
    uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::system_clock::now().time_since_epoch())
                      .count();
    const uint64_t interval =
        kNrfRegistrationRetryTimerSeconds * 1000;  // convert sec to msec

    oai::logger::logger_common::common().debug(
        "Start NRF registration retry task");
    retry_nrf_registration_task_connection =
        event_sub_->subscribe_task_nf_heartbeat(
            boost::bind(
                &nf_service::trigger_nrf_registration_retry_procedure, this,
                _1),
            interval, ms + interval);
  }
}

//---------------------------------------------------------------------------------------------
void nf_service::trigger_nrf_registration_retry_procedure(uint64_t ms) {
  (void) ms;
  send_nf_registration();
}

//---------------------------------------------------------------------------------------------
void nf_service::stop_nrf_registration_retry() {
  if (retry_nrf_registration_task_connection.connected()) {
    oai::logger::logger_common::common().info(
        "Stop NRF registration retry task");
    retry_nrf_registration_task_connection.disconnect();
  }
}

//------------------------------------------------------------------------------
bool nf_service::discovery_cache_lookup(
    const std::string& target_nf_type, const std::string& service_name,
    std::string& endpoint) {
  const std::string cache_key = target_nf_type + ":" + service_name;
  const auto now              = std::chrono::steady_clock::now();
  std::shared_lock<std::shared_mutex> lock(m_discovery_mutex);
  auto it = m_discovery_cache.find(cache_key);
  if (it == m_discovery_cache.end()) return false;
  // An expired entry is a miss: leave it in place, the next store overwrites
  // it anyway.
  if (now >= it->second.expires_at) return false;
  endpoint = it->second.endpoint;
  oai::logger::logger_common::common().debug(
      "NF discovery cache hit: %s -> %s", cache_key.c_str(), endpoint.c_str());
  return true;
}

//------------------------------------------------------------------------------
void nf_service::discovery_cache_store(
    const std::string& target_nf_type, const std::string& service_name,
    std::string endpoint, int ttl_seconds) {
  const std::string cache_key = target_nf_type + ":" + service_name;
  const auto expires_at =
      std::chrono::steady_clock::now() + std::chrono::seconds(ttl_seconds);
  std::unique_lock<std::shared_mutex> lock(m_discovery_mutex);
  m_discovery_cache[cache_key] = {std::move(endpoint), expires_at};
}

//------------------------------------------------------------------------------
void nf_service::discovery_cache_invalidate(
    const std::string& target_nf_type, const std::string& service_name) {
  const std::string cache_key = target_nf_type + ":" + service_name;
  std::unique_lock<std::shared_mutex> lock(m_discovery_mutex);
  m_discovery_cache.erase(cache_key);
}

//------------------------------------------------------------------------------
void nf_service::discovery_cache_clear() {
  std::unique_lock<std::shared_mutex> lock(m_discovery_mutex);
  m_discovery_cache.clear();
}

//------------------------------------------------------------------------------
bool nf_service::handle_discovery_response(
    const oai::sba::response& search_result_resp,
    const std::string& target_nf_type, const std::string& service_name,
    std::string& endpoint) {
  nlohmann::json search_result = search_result_resp.get_json();
  if (!search_result.contains("nfInstances") ||
      !search_result["nfInstances"].is_array()) {
    oai::logger::logger_common::common().warn(
        "NRF discovery: no nfInstances in SearchResult");
    return false;
  }

  for (const auto& nf_instance : search_result["nfInstances"]) {
    // Prefer the matching service's ipEndPoints; fall back to instance-level
    // ipv4Addresses if the desired service is not present.
    if (nf_instance.contains("nfServices") &&
        nf_instance["nfServices"].is_array()) {
      for (const auto& svc : nf_instance["nfServices"]) {
        // An empty service_name means "any service": take the first one.
        if (!service_name.empty() &&
            svc.value("serviceName", std::string{}) != service_name)
          continue;
        std::string svc_scheme = svc.value("scheme", std::string{"http"});
        if (svc.contains("ipEndPoints") && svc["ipEndPoints"].is_array() &&
            !svc["ipEndPoints"].empty()) {
          const auto& ep = svc["ipEndPoints"][0];
          std::string ip = ep.value("ipv4Address", std::string{});
          int port = ep.value("port", static_cast<int>(default_sbi_port()));
          if (!ip.empty()) {
            endpoint = svc_scheme + "://" + ip + ":" + std::to_string(port);
            discovery_cache_store(target_nf_type, service_name, endpoint);
            return true;
          }
        }
      }
    }
    // Fallback: instance-level ipv4Addresses
    if (nf_instance.contains("ipv4Addresses") &&
        nf_instance["ipv4Addresses"].is_array() &&
        !nf_instance["ipv4Addresses"].empty()) {
      std::string ip = nf_instance["ipv4Addresses"][0].get<std::string>();
      if (!ip.empty()) {
        // The bare address carries no port, so the NF's default SBI port is
        // appended: an endpoint without one is only usable when that port
        // happens to be 80.
        endpoint = "http://" + ip + ":" +
                   std::to_string(static_cast<int>(default_sbi_port()));
        oai::logger::logger_common::common().warn(
            "NRF discovery: service %s not found for %s, using instance "
            "address %s",
            service_name.c_str(), target_nf_type.c_str(), endpoint.c_str());
        discovery_cache_store(target_nf_type, service_name, endpoint);
        return true;
      }
    }
  }

  oai::logger::logger_common::common().warn(
      "NRF discovery: no usable endpoint for %s/%s", target_nf_type.c_str(),
      service_name.c_str());
  return false;
}

//------------------------------------------------------------------------------
bool nf_service::discover_nf(
    const oai::common::sbi::nf_addr_t& nrf_addr,
    const std::string& requester_nf_type, const std::string& target_nf_type,
    const std::string& service_name, std::string& endpoint) {
  // Local configuration first: a deployment that pins its peers statically
  // must not be made to depend on the NRF being up.
  if (resolve_endpoint_from_config(target_nf_type, service_name, endpoint))
    return true;

  if (!nrf_discovery_enabled()) {
    oai::logger::logger_common::common().warn(
        "NRF discovery is disabled and no static configuration for %s",
        target_nf_type.c_str());
    return false;
  }

  if (discovery_cache_lookup(target_nf_type, service_name, endpoint))
    return true;

  // Build the NRF SearchNFInstances URI and add discovery query parameters.
  std::string uri = {};
  oai::common::sbi::sbi_helper::get_nrf_disc_search_nf_instances_uri(
      nrf_addr, uri);
  uri += "?target-nf-type=" + target_nf_type +
         "&requester-nf-type=" + requester_nf_type;

  oai::sba::request req   = http_client_inst_->prepare_json_request(uri);
  oai::sba::response resp = send_with_policy(
      nrf_call_kind::discovery, oai::common::sbi::method_e::GET, req);
  if (resp.status_code != oai::common::sbi::http_status_code::OK) {
    oai::logger::logger_common::common().warn(
        "NRF discovery for %s failed (HTTP %d)", target_nf_type.c_str(),
        resp.status_code);
    return false;
  }

  return handle_discovery_response(
      resp, target_nf_type, service_name, endpoint);
}
