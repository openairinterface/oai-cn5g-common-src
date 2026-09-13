/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "nf_service.hpp"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include "logger_base.hpp"
#include "PatchItem.h"
#include "http_client.hpp"

using namespace oai::sba;

//------------------------------------------------------------------------------
nf_service::nf_service(
    const std::shared_ptr<nf_event>& ev,
    const std::shared_ptr<oai::sba::http_client>& client_inst)
    : event_sub_(ev), http_client_inst_(client_inst) {
  // generate UUID
  nf_instance_id = to_string(boost::uuids::random_generator()());
}

//------------------------------------------------------------------------------
nf_service::~nf_service() {
  if (task_connection.connected()) task_connection.disconnect();
  if (retry_nrf_registration_task_connection.connected())
    retry_nrf_registration_task_connection.disconnect();
}
//---------------------------------------------------------------------------------------------
void nf_service::generate_nf_profile() {
  // TODO:
}
//---------------------------------------------------------------------------------------------
void nf_service::register_to_nrf(
    const oai::common::sbi::nf_addr_t& nrf_addr,
    const nlohmann::json& nf_profile) {
  nlohmann::json response_data = {};
  std::string nrf_uri          = {};

  oai::common::sbi::sbi_helper::get_nrf_nf_instance_uri(
      nrf_addr, nf_instance_id, nrf_uri);
  oai::logger::logger_common::common().info(
      "Sending NF registration request to NRF, NRF's URI: %s", nrf_uri);

  bool registration_success = false;

  oai::sba::request http_request =
      http_client_inst_->prepare_json_request(nrf_uri, nf_profile.dump());
  auto http_response = http_client_inst_->send_http_request(
      oai::common::sbi::method_e::PUT, http_request);

  if ((http_response.status_code == oai::common::sbi::http_status_code::OK) or
      (http_response.status_code ==
       oai::common::sbi::http_status_code::CREATED)) {
    try {
      response_data = nlohmann::json::parse(http_response.body);
      // TODO: use Heart-beart timer interval returned from NRF
      if (response_data.find("nfStatus") != response_data.end()) {
        std::string status = response_data["nfStatus"].get<std::string>();
        if (status.compare("REGISTERED") == 0) {
          registration_success = true;
          start_event_nf_heartbeat(nrf_uri);
          stop_nrf_registration_retry();
        }
      }
    } catch (nlohmann::json::exception& e) {
      oai::logger::logger_common::common().warn(
          "NF Registration procedure failed, try again ...");
    }
  } else {
    oai::logger::logger_common::common().warn(
        "Could not get response from NRF, try again ...");
  }

  if (!registration_success) {
    start_nrf_registration_retry();
  }
}

//---------------------------------------------------------------------------------------------
void nf_service::deregister_to_nrf(
    const oai::common::sbi::nf_addr_t& nrf_addr) {
  nlohmann::json response_data = {};
  std::string nrf_uri          = {};

  oai::common::sbi::sbi_helper::get_nrf_nf_instance_uri(
      nrf_addr, nf_instance_id, nrf_uri);
  oai::logger::logger_common::common().info(
      "Sending NF Deregistration request");

  oai::sba::request http_request =
      http_client_inst_->prepare_json_request(nrf_uri);
  auto http_response = http_client_inst_->send_http_request(
      oai::common::sbi::method_e::DELETE, http_request);

  if (http_response.status_code ==
      oai::common::sbi::http_status_code::NO_CONTENT) {
    oai::logger::logger_common::common().info(
        "NF Deregistration procedure successful");
    // TODO:
  } else {
    oai::logger::logger_common::common().info(
        "NF Deregistration procedure failed");
    // TODO:
  }
}

//---------------------------------------------------------------------------------------------
void nf_service::start_event_nf_heartbeat(std::string& nrf_uri) {
  // get current time
  uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();
  struct itimerspec its;
  its.it_value.tv_sec  = 10;  // seconds
  its.it_value.tv_nsec = 0;   // 100 * 1000 * 1000; //100ms
  const uint64_t interval =
      its.it_value.tv_sec * 1000 +
      its.it_value.tv_nsec / 1000000;  // convert sec, nsec to msec

  task_connection = event_sub_->subscribe_task_nf_heartbeat(
      boost::bind(&nf_service::trigger_nf_heartbeat_procedure, this, _1, _2),
      interval, ms + interval);
}

//---------------------------------------------------------------------------------------------
void nf_service::trigger_nf_heartbeat_procedure(
    uint64_t ms, const nf_addr_t& nrf_addr) {
  _unused(ms);
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
  sbi_helper::get_nrf_nf_instance_uri(nrf_addr, nf_instance_id, nrf_uri);

  oai::sba::request http_request =
      http_client_inst_->prepare_json_request(nrf_uri, json_data.dump());
  auto http_response = http_client_inst_->send_http_request(
      oai::common::sbi::method_e::PATCH, http_request);

  if ((http_response.status_code == oai::common::sbi::http_status_code::OK) or
      (http_response.status_code ==
       oai::common::sbi::http_status_code::NO_CONTENT)) {
    // TODO: process the response
  } else {
    oai::logger::logger_common::common().info(
        "NF Heartbeat procedure failed, try to register again");
    if (task_connection.connected()) task_connection.disconnect();
    register_to_nrf();
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
        NRF_REGISTRATION_RETRY_TIMER * 1000;  // convert sec to msec

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
  _unused(ms);
  register_to_nrf();
}

//---------------------------------------------------------------------------------------------
void nf_service::stop_nrf_registration_retry() {
  // get current time
  uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();
  if (retry_nrf_registration_task_connection.connected()) {
    oai::logger::logger_common::common().info(
        "Stop NRF registration retry task");
    retry_nrf_registration_task_connection.disconnect();
  }
}

//------------------------------------------------------------------------------
bool nf_service::discover_nf(
    const std::string& source_nf_type const std::string& target_nf_type,
    const std::string& service_name, std::string& endpoint) {
  const std::string cache_key = target_nf_type + ":" + service_name;
  {
    std::unique_lock<std::mutex> lock(m_discovery_mutex);
    auto it = m_discovery_cache.find(cache_key);
    if (it != m_discovery_cache.end()) {
      endpoint = it->second;
      return true;
    }
  }

  // Build the NRF SearchNFInstances URI and add discovery query parameters.
  std::string uri = {};
  oai::common::sbi::sbi_helper::get_nrf_disc_search_nf_instances_uri(
      nrf_addr, uri);
  uri += "?target-nf-type=" + target_nf_type +
         "&requester-nf-type=" + source_nf_type;

  oai::sba::request req = http_client_inst_->prepare_json_request(uri);
  oai::sba::response resp =
      http_client_inst__->send_http_request(method_e::GET, req);
  if (resp.status_code != oai::common::sbi::http_status_code::OK) {
    oai::logger::logger_common::common().warn(
        "NRF discovery for %s failed (HTTP %d)", target_nf_type.c_str(),
        resp.status_code);
    return false;
  }

  nlohmann::json search_result = resp.get_json();
  if (!search_result.contains("nfInstances") ||
      !search_result["nfInstances"].is_array()) {
    Logger::udm_nrf().warn("NRF discovery: no nfInstances in SearchResult");
    return false;
  }

  for (const auto& nf_instance : search_result["nfInstances"]) {
    // Prefer the matching service's ipEndPoints; fall back to instance-level
    // ipv4Addresses if the desired service is not present.
    if (nf_instance.contains("nfServices") &&
        nf_instance["nfServices"].is_array()) {
      for (const auto& svc : nf_instance["nfServices"]) {
        if (svc.value("serviceName", std::string{}) != service_name) continue;
        std::string svc_scheme = svc.value("scheme", std::string{"http"});
        if (svc.contains("ipEndPoints") && svc["ipEndPoints"].is_array() &&
            !svc["ipEndPoints"].empty()) {
          const auto& ep = svc["ipEndPoints"][0];
          std::string ip = ep.value("ipv4Address", std::string{});
          int port       = ep.value("port", 80);
          if (!ip.empty()) {
            endpoint = svc_scheme + "://" + ip + ":" + std::to_string(port);
            std::unique_lock<std::mutex> lock(m_discovery_mutex);
            m_discovery_cache[cache_key] = endpoint;
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
        endpoint = "http://" + ip;
        Logger::udm_nrf().warn(
            "NRF discovery: service %s not found for %s, using instance "
            "address %s",
            service_name.c_str(), target_nf_type.c_str(), endpoint.c_str());
        std::unique_lock<std::mutex> lock(m_discovery_mutex);
        m_discovery_cache[cache_key] = endpoint;
        return true;
      }
    }
  }

  oai::logger::logger_common::common().warn(
      "NRF discovery: no usable endpoint for %s/%s", target_nf_type.c_str(),
      service_name.c_str());
  return false;
}
