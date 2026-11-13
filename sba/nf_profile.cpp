/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "nf_profile.hpp"

#include <unistd.h>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "logger_base.hpp"
#include "string.hpp"

using namespace oai::sba;

namespace oai::sba {

std::string& sba_logger_name() {
  static std::string name = LOGGER_COMMON;
  return name;
}

void set_sba_logger(const std::string& logger_name) {
  sba_logger_name() = logger_name;
}

const oai::logger::printf_logger& sba_logger() {
  return oai::logger::logger_registry::get_logger(sba_logger_name());
}

}  // namespace oai::sba

//------------------------------------------------------------------------------
void nf_profile::set_nf_instance_id(const std::string& instance_id) {
  nf_instance_id = instance_id;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_instance_id(std::string& instance_id) const {
  instance_id = nf_instance_id;
}

//------------------------------------------------------------------------------
std::string nf_profile::get_nf_instance_id() const {
  return nf_instance_id;
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_instance_name(const std::string& instance_name) {
  nf_instance_name = instance_name;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_instance_name(std::string& instance_name) const {
  instance_name = nf_instance_name;
}

//------------------------------------------------------------------------------
std::string nf_profile::get_nf_instance_name() const {
  return nf_instance_name;
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_type(const std::string& type) {
  nf_type = type;
}

//------------------------------------------------------------------------------
std::string nf_profile::get_nf_type() const {
  return nf_type;
}

//------------------------------------------------------------------------------
bool nf_profile::set_nf_status(const std::string& status) {
  sba_logger().debug("Set NF status to %s", status);
  if (!(boost::iequals(status, "REGISTERED") or
        boost::iequals(status, "UNDISCOVERABLE") or
        boost::iequals(status, "SUSPENDED"))) {
    return false;
  }
  std::unique_lock lock(nf_profile_mutex);
  nf_status = status;
  return true;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_status(std::string& status) const {
  std::shared_lock lock(nf_profile_mutex);
  status = nf_status;
}

//------------------------------------------------------------------------------
std::string nf_profile::get_nf_status() const {
  std::shared_lock lock(nf_profile_mutex);
  return nf_status;
}

//------------------------------------------------------------------------------
bool nf_profile::is_nf_active() const {
  std::shared_lock lock(nf_profile_mutex);
  if (boost::iequals(nf_status, "REGISTERED")) {
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_heartBeat_timer(const int32_t& timer) {
  heartBeat_timer = timer;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_heartBeat_timer(int32_t& timer) const {
  timer = heartBeat_timer;
}

//------------------------------------------------------------------------------
int32_t nf_profile::get_nf_heartBeat_timer() const {
  return heartBeat_timer;
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_priority(const uint16_t& p) {
  priority = p;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_priority(uint16_t& p) const {
  p = priority;
}

//------------------------------------------------------------------------------
uint16_t nf_profile::get_nf_priority() const {
  return priority;
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_capacity(const uint16_t& c) {
  capacity = c;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_capacity(uint16_t& c) const {
  c = capacity;
}

//------------------------------------------------------------------------------
uint16_t nf_profile::get_nf_capacity() const {
  return capacity;
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_snssais(const std::vector<snssai_t>& s) {
  snssais = s;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_snssais(std::vector<snssai_t>& s) const {
  s = snssais;
}

//------------------------------------------------------------------------------
void nf_profile::add_snssai(const snssai_t& s) {
  snssais.push_back(s);
}

//------------------------------------------------------------------------------
void nf_profile::set_fqdn(const std::string& fqdN) {
  fqdn = fqdN;
}

//------------------------------------------------------------------------------
std::string nf_profile::get_fqdn() const {
  return fqdn;
}

//------------------------------------------------------------------------------
void nf_profile::set_plmn_list(const std::vector<plmn_t>& s) {
  plmn_list = s;
}

//------------------------------------------------------------------------------
void nf_profile::get_plmn_list(std::vector<plmn_t>& s) const {
  s = plmn_list;
}

//------------------------------------------------------------------------------
void nf_profile::add_plmn_list(const plmn_t& s) {
  plmn_list.push_back(s);
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_ipv4_addresses(const std::vector<struct in_addr>& a) {
  ipv4_addresses = a;
}

//------------------------------------------------------------------------------
void nf_profile::add_nf_ipv4_addresses(const struct in_addr& a) {
  ipv4_addresses.push_back(a);
}
//------------------------------------------------------------------------------
void nf_profile::set_nf_ipv6_addresses(const std::vector<struct in6_addr>& a) {
  ipv6_addresses = a;
}

//------------------------------------------------------------------------------
void nf_profile::add_nf_ipv6_addresses(const struct in6_addr& a) {
  ipv6_addresses.push_back(a);
}
//------------------------------------------------------------------------------
void nf_profile::get_nf_ipv4_addresses(std::vector<struct in_addr>& a) const {
  a = ipv4_addresses;
}

//------------------------------------------------------------------------------
void nf_profile::set_json_data(const nlohmann::json& data) {
  json_data = data;
}

//------------------------------------------------------------------------------
void nf_profile::get_json_data(nlohmann::json& data) const {
  data = json_data;
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_services(const std::vector<nf_service_t>& n) {
  nf_services = n;
}

//------------------------------------------------------------------------------
void nf_profile::add_nf_service(const nf_service_t& n) {
  nf_services.push_back(n);
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_services(std::vector<nf_service_t>& n) const {
  n = nf_services;
}

//------------------------------------------------------------------------------
void nf_profile::set_custom_info(const nlohmann::json& c) {
  custom_info = c;
}

//------------------------------------------------------------------------------
void nf_profile::get_custom_info(nlohmann::json& c) const {
  c = custom_info;
}

//------------------------------------------------------------------------------
void nf_profile::display() {
  sba_logger().debug("NF instance info");
  sba_logger().debug("\tInstance ID: %s", nf_instance_id);
  sba_logger().debug("\tInstance name: %s", nf_instance_name);
  sba_logger().debug("\tInstance type: %s", nf_type);
  sba_logger().debug("\tStatus: %s", nf_status);
  sba_logger().debug("\tHeartBeat timer: %d", heartBeat_timer);
  sba_logger().debug("\tPriority: %d", priority);
  sba_logger().debug("\tCapacity: %d", capacity);
  // SNSSAIs
  if (!custom_info.empty())
    sba_logger().debug("\tCustomInfo: %s", custom_info.dump());
  if (!plmn_list.empty()) {
    for (auto s : plmn_list) {
      sba_logger().debug("\tPLMN List(MCC, MNC): %d, %s", s.mcc, s.mnc);
    }
  }
  for (auto s : snssais) {
    sba_logger().debug("\tNNSSAI(SST, SD): %d, %s", s.sst, s.sd);
  }
  if (!fqdn.empty()) {
    sba_logger().debug("\tFQDN: %s", fqdn);
  }
  // IPv4 Addresses
  for (auto address : ipv4_addresses) {
    sba_logger().debug("\tIPv4 Addr: %s", inet_ntoa(address));
  }
  // ToDo : For ipv6 addresses
  if (!json_data.empty()) {
    sba_logger().debug("\tJson Data: %s", json_data.dump());
  }

  // NF services
  for (auto service : nf_services) {
    sba_logger().debug("\tNF Service: %s", service.to_string());
  }
}

//------------------------------------------------------------------------------
bool nf_profile::replace_profile_info(
    const std::string& path, const std::string& value) {
  sba_logger().debug("Replace member %s with new value %s", path, value);
  if (path.compare("nfInstanceName") == 0) {
    nf_instance_name = value;
    return true;
  }

  if (path.compare("nfStatus") == 0) {
    nf_status = value;
    return true;
  }

  if (path.compare("nfType") == 0) {
    nf_type = value;
    return true;
  }

  if (path.compare("heartBeatTimer") == 0) {
    try {
      heartBeat_timer = std::stoi(value);
      return true;
    } catch (const std::exception& err) {
      sba_logger().debug("Bad value!");
      return false;
    }
  }

  if (path.compare("priority") == 0) {
    try {
      priority = (uint16_t) std::stoi(value);
      return true;
    } catch (const std::exception& err) {
      sba_logger().debug("Bad value!");
      return false;
    }
  }

  if (path.compare("capacity") == 0) {
    try {
      capacity = (uint16_t) std::stoi(value);
      return true;
    } catch (const std::exception& err) {
      sba_logger().debug("Bad value!");
      return false;
    }
  }

  if (path.compare("fqdn") == 0) {
    fqdn = value;
    return true;
  }

  // Replace an array
  if (path.compare("plmnList") == 0) {
    sba_logger().info("Does not support this operation for ipv4Addresses");
    return false;
  }

  if (path.compare("ipv4Addresses") == 0) {
    sba_logger().info("Does not support this operation for ipv4Addresses");
    return false;
  }

  if (path.compare("ipv6Addresses") == 0) {
    sba_logger().info("Does not support this operation for ipv6Addresses");
    return false;
  }

  if (path.compare("sNssais") == 0) {
    sba_logger().info("Does not support this operation for sNssais");
    return false;
  }

  if (path.compare("nfServices") == 0) {
    sba_logger().info("Does not support this operation for nfServices");
    return false;
  }

  return false;
}

//------------------------------------------------------------------------------
bool nf_profile::add_profile_info(
    const std::string& path, const std::string& value) {
  sba_logger().debug(
      "Add an array element (value, array member), or a new member (value, "
      "member):  %s, %s",
      value, path);

  // update an existing member
  if (path.compare("nfInstanceName") == 0) {
    nf_instance_name = value;
    return true;
  }

  if (path.compare("nfStatus") == 0) {
    nf_status = value;
    return true;
  }

  if (path.compare("nfType") == 0) {
    nf_type = value;
    return true;
  }

  if (path.compare("heartBeatTimer") == 0) {
    try {
      heartBeat_timer = std::stoi(value);
      return true;
    } catch (const std::exception& err) {
      sba_logger().debug("Bad value!");
      return false;
    }
  }

  if (path.compare("priority") == 0) {
    try {
      priority = (uint16_t) std::stoi(value);
      return true;
    } catch (const std::exception& err) {
      sba_logger().debug("Bad value!");
      return false;
    }
  }

  if (path.compare("capacity") == 0) {
    try {
      capacity = (uint16_t) std::stoi(value);
      return true;
    } catch (const std::exception& err) {
      sba_logger().debug("Bad value!");
      return false;
    }
  }

  if (path.compare("fqdn") == 0) {
    fqdn = value;
    return true;
  }

  // add an element to a list
  if (path.compare("ipv4Addresses") == 0) {
    std::string address  = value;
    struct in_addr addr4 = {};
    unsigned char buf_in_addr[sizeof(struct in_addr)];
    if (inet_pton(AF_INET, oai::utils::trim(address).c_str(), buf_in_addr) ==
        1) {
      memcpy(&addr4, buf_in_addr, sizeof(struct in_addr));
    } else {
      sba_logger().warn(
          "Address conversion: Bad value %s", oai::utils::trim(address));
      return false;
    }
    sba_logger().debug("Added IPv4 Addr: %s", address);
    ipv4_addresses.push_back(addr4);
    return true;
  }

  // add an element to a list
  if (path.compare("ipv6Addresses") == 0) {
    std::string address   = value;
    struct in6_addr addr6 = {};
    unsigned char buf_in_addr[sizeof(struct in6_addr)];
    if (inet_pton(AF_INET6, oai::utils::trim(address).c_str(), buf_in_addr) ==
        1) {
      memcpy(&addr6, buf_in_addr, sizeof(struct in6_addr));
    } else {
      sba_logger().warn(
          "Address conversion: Bad value %s", oai::utils::trim(address));
      return false;
    }
    sba_logger().debug("Added IPv6 Addr: %s", address);
    ipv6_addresses.push_back(addr6);
    return true;
  }

  // add an element to a list of json object
  if (path.compare("sNssais") == 0) {
    sba_logger().info("Does not support this operation for sNssais");
    return false;
  }

  if (path.compare("nfServices") == 0) {
    sba_logger().info("Does not support this operation for nfServices");
    return false;
  }

  if (path.compare("plmnList") == 0) {
    sba_logger().info("Does not support this operation for plmnList");
    return false;
  }

  return false;
}

//------------------------------------------------------------------------------
bool nf_profile::remove_profile_info(const std::string& path) {
  sba_logger().debug("Remove an array element or a member: %s", path);
  if (path.compare("nfInstanceName") == 0) {
    nf_instance_name = "";
    return true;
  }

  if (path.compare("nfStatus") == 0) {
    nf_status = "";
    return true;
  }

  if (path.compare("nfType") == 0) {
    nf_type = {};
    return true;
  }

  if (path.compare("heartBeatTimer") == 0) {
    heartBeat_timer = 0;
    return true;
  }

  if (path.compare("priority") == 0) {
    priority = 0;
    return true;
  }

  if (path.compare("capacity") == 0) {
    priority = 0;
    return true;
  }

  if (path.compare("fqdn") == 0) {
    fqdn = "";
    return true;
  }

  // path: e.g., /ipv4Addresses/4
  if (path.find("ipv4Addresses") != std::string::npos) {
    std::vector<std::string> parts;
    boost::split(parts, path, boost::is_any_of("/"), boost::token_compress_on);
    if (parts.size() != 2) {
      sba_logger().warn("Bad value for path: %s ", path);
      return false;
    }
    // get and check index
    uint32_t index = 0;
    try {
      index = std::stoi(parts.at(1));
    } catch (const std::exception& err) {
      sba_logger().warn("Bad value for path: %s ", path);
      return false;
    }

    if (index >= ipv4_addresses.size()) {
      sba_logger().warn("Bad value for path: %s ", path);
      return false;
    } else {
      sba_logger().debug(
          "Removed IPv4 Addr: %s", inet_ntoa(ipv4_addresses[index]));
      ipv4_addresses.erase(ipv4_addresses.begin() + index);
      return true;
    }
  }

  if (path.find("sNssais") != std::string::npos) {
    sba_logger().info("Does not support this operation for sNssais");
    return false;
  }

  if (path.find("nfServices") != std::string::npos) {
    sba_logger().info("Does not support this operation for nfServices");
    return false;
  }

  if (path.find("plmnList") != std::string::npos) {
    sba_logger().info("Does not support this operation for plmnList");
    return false;
  }

  sba_logger().debug("Member (%s) not found!", path);
  return false;
}

//------------------------------------------------------------------------------
void nf_profile::to_json(nlohmann::json& data) const {
  data["nfInstanceId"]   = nf_instance_id;
  data["nfInstanceName"] = nf_instance_name;
  data["nfType"]         = nf_type;
  data["nfStatus"]       = nf_status;
  data["heartBeatTimer"] = heartBeat_timer;
  // SNSSAIs
  if (!snssais.empty()) {
    data["sNssais"] = nlohmann::json::array();
    for (auto s : snssais) {
      nlohmann::json tmp = {};
      tmp["sst"]         = s.sst;
      tmp["sd"]          = s.sd;
      data["sNssais"].push_back(tmp);
    }
  }
  if (!fqdn.empty()) {
    data["fqdn"] = fqdn;
  }
  if (!plmn_list.empty()) {
    data["plmnList"] = nlohmann::json::array();
    for (auto s : plmn_list) {
      nlohmann::json tmp = {};
      tmp["mcc"]         = s.mcc;
      tmp["mnc"]         = s.mnc;
      data["plmnList"].push_back(tmp);
    }
  }
  // ipv4_addresses
  data["ipv4Addresses"] = nlohmann::json::array();
  for (auto address : ipv4_addresses) {
    nlohmann::json tmp = inet_ntoa(address);
    data["ipv4Addresses"].push_back(tmp);
  }
  // // ipv6_addresses
  // data["ipv6Addresses"] = nlohmann::json::array();
  // for (auto address : ipv6_addresses) {
  //   nlohmann::json tmp = inet_ntoa(address);
  //   data["ipv6Addresses"].push_back(tmp);
  // }
  data["priority"] = priority;
  data["capacity"] = capacity;
  // CustomInfo
  if (!custom_info.empty()) data["customInfo"] = custom_info;
  // NF services
  data["nfServices"] = nlohmann::json::array();
  for (auto service : nf_services) {
    nlohmann::json srv_tmp       = {};
    srv_tmp["serviceInstanceId"] = service.service_instance_id;
    srv_tmp["serviceName"]       = service.service_name;
    srv_tmp["versions"]          = nlohmann::json::array();
    for (auto v : service.versions) {
      nlohmann::json v_tmp     = {};
      v_tmp["apiVersionInUri"] = v.api_version_in_uri;
      v_tmp["apiFullVersion"]  = v.api_full_version;
      srv_tmp["versions"].push_back(v_tmp);
    }
    srv_tmp["scheme"]          = service.scheme;
    srv_tmp["nfServiceStatus"] = service.nf_service_status;
    if (!service.ip_endpoints.empty()) {
      // IP endpoints
      srv_tmp["ipEndPoints"] = nlohmann::json::array();
      for (auto endpoint : service.ip_endpoints) {
        nlohmann::json ep_tmp = {};
        ep_tmp["ipv4Address"] = inet_ntoa(endpoint.ipv4_address);
        ep_tmp["transport"]   = endpoint.transport;
        ep_tmp["port"]        = endpoint.port;
        srv_tmp["ipEndPoints"].push_back(ep_tmp);
      }
    }
    data["nfServices"].push_back(srv_tmp);
  }
  data["json_data"] = json_data;
}

//------------------------------------------------------------------------------
void nf_profile::set_status_updated(bool status) {
  std::unique_lock lock(nf_profile_mutex);
  is_updated = status;
}

//------------------------------------------------------------------------------
void nf_profile::get_status_updated(bool& status) {
  std::shared_lock lock(nf_profile_mutex);
  status = is_updated;
}

//------------------------------------------------------------------------------
bool nf_profile::get_status_updated() {
  std::shared_lock lock(nf_profile_mutex);
  return is_updated;
}

//------------------------------------------------------------------------------
void amf_profile::add_amf_info(const amf_info_t& info) {
  amf_info = info;
}

//------------------------------------------------------------------------------
void amf_profile::get_amf_info(amf_info_t& info) const {
  info = amf_info;
}

//------------------------------------------------------------------------------
void amf_profile::display() {
  nf_profile::display();
  sba_logger().debug("\tAMF Info");
  sba_logger().debug(
      "\t\tAMF Set ID: %s, AMF Region ID: %s", amf_info.amf_set_id,
      amf_info.amf_region_id);

  for (auto g : amf_info.guami_list) {
    sba_logger().debug("\t\tAMF GUAMI List, AMF_ID: %s", g.amf_id);
    sba_logger().debug(
        "\t\tAMF GUAMI List, PLMN (MCC: %s, MNC: %s)", g.plmn.mcc, g.plmn.mnc);
  }
}

//------------------------------------------------------------------------------
bool amf_profile::replace_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::replace_profile_info(path, value);
  if (result) return true;
  // for AMF info
  if (path.compare("amfInfo") == 0) {
    sba_logger().debug("Do not support this operation for amfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and
      (path.compare("sNssais") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("amfInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }

  return false;
}

//------------------------------------------------------------------------------
bool amf_profile::add_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::add_profile_info(path, value);
  if (result) return true;

  // add an element to a list of json object
  if (path.compare("amfInfo") == 0) {
    sba_logger().info("Do not support this operation for amfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and
      (path.compare("sNssais") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("amfInfo") != 0)) {
    sba_logger().debug("Add new member: %s", path);
    // add new member
    json_data[path] = value;
    return true;
  }

  return false;
}

//------------------------------------------------------------------------------
bool amf_profile::remove_profile_info(const std::string& path) {
  bool result = nf_profile::remove_profile_info(path);
  if (result) return true;
  // for AMF info
  if (path.compare("amfInfo") == 0) {
    sba_logger().debug("Do not support this operation for amfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and
      (path.compare("sNssais") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("amfInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }

  return false;
}

//------------------------------------------------------------------------------
void amf_profile::to_json(nlohmann::json& data) const {
  nf_profile::to_json(data);
  // AMF Info
  data["amfInfo"]["amfSetId"]    = amf_info.amf_set_id;
  data["amfInfo"]["amfRegionId"] = amf_info.amf_region_id;
  // guamiList
  data["amfInfo"]["guamiList"] = nlohmann::json::array();
  for (auto guami : amf_info.guami_list) {
    nlohmann::json tmp   = {};
    tmp["amfId"]         = guami.amf_id;
    tmp["plmnId"]["mnc"] = guami.plmn.mnc;
    tmp["plmnId"]["mcc"] = guami.plmn.mcc;
    data["amfInfo"]["guamiList"].push_back(tmp);
  }
}

//------------------------------------------------------------------------------
void smf_profile::add_smf_info(const smf_info_t& info) {
  smf_info = info;
}

//------------------------------------------------------------------------------
void smf_profile::get_smf_info(smf_info_t& infos) const {
  infos = smf_info;
}

//------------------------------------------------------------------------------
void smf_profile::display() {
  nf_profile::display();
  sba_logger().debug("\tSMF Info");
  for (auto s : smf_info.snssai_smf_info_list) {
    sba_logger().debug(
        "\t\tSNSSAI SMF Info List, SNSSAI (SD: %s, SST: %d)", s.snssai.sd,
        s.snssai.sst);
    for (auto d : s.dnn_smf_info_list) {
      sba_logger().debug("\t\tSNSSAI SMF Info List, DNN List: %s", d.dnn);
    }
  }
}

//------------------------------------------------------------------------------
bool smf_profile::add_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::add_profile_info(path, value);
  if (result) return true;

  // add an element to a list of json object
  if (path.compare("smfInfo") == 0) {
    sba_logger().info("Does not support this operation for smfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and
      (path.compare("sNssais") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("smfInfo") != 0)) {
    sba_logger().debug("Add new member: %s", path);
    // add new member
    json_data[path] = value;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool smf_profile::replace_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::replace_profile_info(path, value);
  if (result) return true;
  // for SMF info
  if (path.compare("smfInfo") == 0) {
    sba_logger().debug("Does not support this operation for amfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and
      (path.compare("sNssais") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("amfInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }

  return false;
}

//------------------------------------------------------------------------------
bool smf_profile::remove_profile_info(const std::string& path) {
  bool result = nf_profile::remove_profile_info(path);
  if (result) return true;
  // for SMF info
  if (path.compare("smfInfo") == 0) {
    sba_logger().debug("Do not support this operation for smfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and
      (path.compare("sNssais") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("smfInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }

  return false;
}

//------------------------------------------------------------------------------
void smf_profile::to_json(nlohmann::json& data) const {
  nf_profile::to_json(data);
  // SMF Info
  data["smfInfo"]["sNssaiSmfInfoList"] = nlohmann::json::array();
  for (auto snssai : smf_info.snssai_smf_info_list) {
    nlohmann::json tmp    = {};
    tmp["sNssai"]["sst"]  = snssai.snssai.sst;
    tmp["sNssai"]["sd"]   = snssai.snssai.sd;
    tmp["dnnSmfInfoList"] = nlohmann::json::array();
    for (auto d : snssai.dnn_smf_info_list) {
      nlohmann::json tmp_dnn = {};
      tmp_dnn["dnn"]         = d.dnn;
      tmp["dnnSmfInfoList"].push_back(tmp_dnn);
    }
    data["smfInfo"]["sNssaiSmfInfoList"].push_back(tmp);
  }
}

//------------------------------------------------------------------------------
void upf_profile::add_upf_info(const upf_info_t& info) {
  upf_info = info;
}

//------------------------------------------------------------------------------
void upf_profile::get_upf_info(upf_info_t& infos) const {
  infos = upf_info;
}

//------------------------------------------------------------------------------
void upf_profile::display() {
  nf_profile::display();
  sba_logger().debug("\tUPF Info");
  for (auto s : upf_info.snssai_upf_info_list) {
    sba_logger().debug(
        "\t\tSNSSAI UPF Info List, SNSSAI (SD: %s, SST: %d)", s.snssai.sd,
        s.snssai.sst);
    for (auto d : s.dnn_upf_info_list) {
      sba_logger().debug("\t\tSNSSAI UPF Info List, DNN List: %s", d.dnn);
      for (auto dnai : d.dnai_list) {
        sba_logger().debug(
            "\t\tSNSSAI UPF Info List, DNN List, DNAI List: %s", dnai);
      }
      for (auto nwinstance : d.dnai_nw_instance_list) {
        sba_logger().debug(
            "\t\tSNSSAI UPF Info List, DNN List, DNAI NW Instance List: %s : "
            "%s",
            nwinstance.first, nwinstance.second);
      }
    }
  }
  if (!upf_info.interface_upf_info_list.empty()) {
    for (auto s : upf_info.interface_upf_info_list) {
      sba_logger().debug(
          "\t\tInterface UPF Info List, Interface Type : %s, Network Instance "
          "%s",
          s.interface_type, s.network_instance);
    }
  }
}

//------------------------------------------------------------------------------
bool upf_profile::add_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::add_profile_info(path, value);
  if (result) return true;

  // add an element to a list of json object
  if (path.compare("upfInfo") == 0) {
    sba_logger().info("Does not support this operation for upfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and
      (path.compare("sNssais") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("upfInfo") != 0)) {
    sba_logger().debug("Add new member: %s", path);
    // add new member
    json_data[path] = value;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool upf_profile::replace_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::replace_profile_info(path, value);
  if (result) return true;
  // for UPF info
  if (path.compare("upfInfo") == 0) {
    sba_logger().debug("Does not support this operation for amfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and
      (path.compare("sNssais") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("amfInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }

  return false;
}

//------------------------------------------------------------------------------
bool upf_profile::remove_profile_info(const std::string& path) {
  bool result = nf_profile::remove_profile_info(path);
  if (result) return true;
  // for UPF info
  if (path.compare("upfInfo") == 0) {
    sba_logger().debug("Do not support this operation for upfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and
      (path.compare("sNssais") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("upfInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }

  return false;
}

//------------------------------------------------------------------------------
void upf_profile::to_json(nlohmann::json& data) const {
  nf_profile::to_json(data);
  // UPF Info
  data["upfInfo"]["sNssaiUpfInfoList"] = nlohmann::json::array();
  for (auto snssai : upf_info.snssai_upf_info_list) {
    nlohmann::json tmp    = {};
    tmp["sNssai"]["sst"]  = snssai.snssai.sst;
    tmp["sNssai"]["sd"]   = snssai.snssai.sd;
    tmp["dnnUpfInfoList"] = nlohmann::json::array();
    for (auto d : snssai.dnn_upf_info_list) {
      nlohmann::json tmp_upf_info_item = {};
      tmp_upf_info_item["dnn"]         = d.dnn;
      if (d.dnai_list.size() > 0) {
        tmp_upf_info_item["dnaiList"] = nlohmann::json::array();
        for (auto dnai : d.dnai_list) {
          tmp_upf_info_item["dnaiList"].push_back(dnai);
        }
      }
      if (d.dnai_nw_instance_list.size() > 0) {
        for (auto it_dnai_nw : d.dnai_nw_instance_list) {
          tmp_upf_info_item["dnaiNwInstanceList"][it_dnai_nw.first] =
              it_dnai_nw.second;
        }
      }

      tmp["dnnUpfInfoList"].push_back(tmp_upf_info_item);
    }
    data["upfInfo"]["sNssaiUpfInfoList"].push_back(tmp);
  }
  if (!upf_info.interface_upf_info_list.empty()) {
    data["upfInfo"]["interfaceUpfInfoList"] = nlohmann::json::array();
    for (auto s : upf_info.interface_upf_info_list) {
      nlohmann::json tmp   = {};
      tmp["interfaceType"] = s.interface_type;
      if (!s.endpoint_fqdn.empty()) tmp["endpointFqdn"] = s.endpoint_fqdn;
      if (!s.network_instance.empty())
        tmp["networkInstance"] = s.network_instance;
      if (!s.ipv4_addresses.empty()) {
        tmp["ipv4EndpointAddresses"] = nlohmann::json::array();
        for (auto address : s.ipv4_addresses) {
          nlohmann::json addr = inet_ntoa(address);
          tmp["ipv4EndpointAddresses"].push_back(addr);
        }
      }
      // ToDo for ipv6
      data["upfInfo"]["interfaceUpfInfoList"].push_back(tmp);
    }
  }
}

//------------------------------------------------------------------------------
void ausf_profile::add_ausf_info(const ausf_info_t& info) {
  ausf_info = info;
}

//------------------------------------------------------------------------------
void ausf_profile::get_ausf_info(ausf_info_t& infos) const {
  infos = ausf_info;
}

//------------------------------------------------------------------------------
void ausf_profile::display() {
  nf_profile::display();
  sba_logger().debug("\tAUSF Info");
  sba_logger().debug("\t\tGroupId: %s", ausf_info.groupid);
  for (auto supi : ausf_info.supi_ranges) {
    sba_logger().debug(
        "\t\t SupiRanges: Start - %s, End - %s, Pattern - %s",
        supi.supi_range.start, supi.supi_range.end, supi.supi_range.pattern);
  }
  for (auto route_ind : ausf_info.routing_indicators) {
    sba_logger().debug("\t\t Routing Indicators: %s", route_ind);
  }
}

//------------------------------------------------------------------------------
bool ausf_profile::add_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::add_profile_info(path, value);
  if (result) return true;

  // add an element to a list of json object
  if (path.compare("ausfInfo") == 0) {
    sba_logger().info("Does not support this operation for ausfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and (path.compare("fqdn") != 0) and
      (path.compare("plmnList") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("ausfInfo") != 0)) {
    sba_logger().debug("Add new member: %s", path);
    // add new member
    json_data[path] = value;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool ausf_profile::replace_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::replace_profile_info(path, value);
  if (result) return true;
  // for AUSF info
  if (path.compare("ausfInfo") == 0) {
    sba_logger().debug("Does not support this operation for ausfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and (path.compare("fqdn") != 0) and
      (path.compare("plmnList") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("ausfInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }

  return false;
}

//------------------------------------------------------------------------------
bool ausf_profile::remove_profile_info(const std::string& path) {
  bool result = nf_profile::remove_profile_info(path);
  if (result) return true;
  // for AUSF info
  if (path.compare("ausfInfo") == 0) {
    sba_logger().debug("Do not support this operation for ausfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and (path.compare("fqdn") != 0) and
      (path.compare("plmnList") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("ausfInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }

  return false;
}

//------------------------------------------------------------------------------
void ausf_profile::to_json(nlohmann::json& data) const {
  nf_profile::to_json(data);
  // AUSF Info
  data["ausfInfo"]["groupId"]           = ausf_info.groupid;
  data["ausfInfo"]["supiRanges"]        = nlohmann::json::array();
  data["ausfInfo"]["routingIndicators"] = nlohmann::json::array();
  for (auto supi : ausf_info.supi_ranges) {
    nlohmann::json tmp = {};
    tmp["start"]       = supi.supi_range.start;
    tmp["end"]         = supi.supi_range.end;
    tmp["pattern"]     = supi.supi_range.pattern;
    data["ausfInfo"]["supiRanges"].push_back(tmp);
  }
  for (auto route_ind : ausf_info.routing_indicators) {
    data["ausfInfo"]["routingIndicators"].push_back(route_ind);
  }
}

//------------------------------------------------------------------------------
void udm_profile::add_udm_info(const udm_info_t& info) {
  udm_info = info;
}

//------------------------------------------------------------------------------
void udm_profile::get_udm_info(udm_info_t& infos) const {
  infos = udm_info;
}

//------------------------------------------------------------------------------
void udm_profile::display() {
  nf_profile::display();
  sba_logger().debug("\tUDM Info");
  sba_logger().debug("\t\tGroupId: %s", udm_info.groupid);
  for (auto supi : udm_info.supi_ranges) {
    sba_logger().debug(
        "\t\t SupiRanges: Start - %s, End - %s, Pattern - %s",
        supi.supi_range.start, supi.supi_range.end, supi.supi_range.pattern);
  }
  for (auto gpsiRange : udm_info.gpsi_ranges) {
    sba_logger().debug(
        "\t\t GpsiRanges: Start - %s, End - %s, Pattern - %s",
        gpsiRange.identity_range.start, gpsiRange.identity_range.end,
        gpsiRange.identity_range.pattern);
  }
  for (auto ext_grp_id : udm_info.ext_grp_id_ranges) {
    sba_logger().debug(
        "\t\t externalGroupIdentifiersRanges: Start - %s, End - %s, Pattern - "
        "%s",
        ext_grp_id.identity_range.start, ext_grp_id.identity_range.end,
        ext_grp_id.identity_range.pattern);
  }
  for (auto int_grp_id : udm_info.int_grp_id_ranges) {
    sba_logger().debug(
        "\t\t internalGroupIdentifiersRanges: Start - %s, End - %s, Pattern - "
        "%s",
        int_grp_id.int_grpid_range.start, int_grp_id.int_grpid_range.end,
        int_grp_id.int_grpid_range.pattern);
  }
  for (auto route_ind : udm_info.routing_indicator) {
    sba_logger().debug("\t\t Routing Indicators: %s", route_ind);
  }
}

//------------------------------------------------------------------------------
bool udm_profile::add_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::add_profile_info(path, value);
  if (result) return true;

  // add an element to a list of json object
  if (path.compare("udmInfo") == 0) {
    sba_logger().info("Does not support this operation for udmInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and (path.compare("fqdn") != 0) and
      (path.compare("plmnList") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("udmInfo") != 0)) {
    sba_logger().debug("Add new member: %s", path);
    // add new member
    json_data[path] = value;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool udm_profile::replace_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::replace_profile_info(path, value);
  if (result) return true;
  // for UDMAUSF info
  if (path.compare("udmInfo") == 0) {
    sba_logger().debug("Does not support this operation for udmInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and (path.compare("fqdn") != 0) and
      (path.compare("plmnList") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("udmInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }
  return false;
}

//------------------------------------------------------------------------------
bool udm_profile::remove_profile_info(const std::string& path) {
  bool result = nf_profile::remove_profile_info(path);
  if (result) return true;
  // for UDM info
  if (path.compare("udmInfo") == 0) {
    sba_logger().debug("Do not support this operation for udmInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and (path.compare("fqdn") != 0) and
      (path.compare("plmnList") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("udmInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }
  return false;
}

//------------------------------------------------------------------------------
void udm_profile::to_json(nlohmann::json& data) const {
  nf_profile::to_json(data);
  // UDM Info
  data["udmInfo"]["groupId"]                        = udm_info.groupid;
  data["udmInfo"]["supiRanges"]                     = nlohmann::json::array();
  data["udmInfo"]["gpsiRanges"]                     = nlohmann::json::array();
  data["udmInfo"]["externalGroupIdentifiersRanges"] = nlohmann::json::array();
  data["udmInfo"]["routingIndicators"]              = nlohmann::json::array();
  data["udmInfo"]["internalGroupIdentifiersRanges"] = nlohmann::json::array();

  for (auto supi : udm_info.supi_ranges) {
    nlohmann::json tmp = {};
    tmp["start"]       = supi.supi_range.start;
    tmp["end"]         = supi.supi_range.end;
    tmp["pattern"]     = supi.supi_range.pattern;
    data["udmInfo"]["supiRanges"].push_back(tmp);
  }
  for (auto gpsi : udm_info.gpsi_ranges) {
    nlohmann::json tmp = {};
    tmp["start"]       = gpsi.identity_range.start;
    tmp["end"]         = gpsi.identity_range.end;
    tmp["pattern"]     = gpsi.identity_range.pattern;
    data["udmInfo"]["gpsiRanges"].push_back(tmp);
  }
  for (auto ext_grp_id : udm_info.ext_grp_id_ranges) {
    nlohmann::json tmp = {};
    tmp["start"]       = ext_grp_id.identity_range.start;
    tmp["end"]         = ext_grp_id.identity_range.end;
    tmp["pattern"]     = ext_grp_id.identity_range.pattern;
    data["udmInfo"]["externalGroupIdentifiersRanges"].push_back(tmp);
  }
  for (auto route_ind : udm_info.routing_indicator) {
    data["udmInfo"]["routingIndicators"].push_back(route_ind);
  }
  for (auto int_grp_id : udm_info.int_grp_id_ranges) {
    nlohmann::json tmp = {};
    tmp["start"]       = int_grp_id.int_grpid_range.start;
    tmp["end"]         = int_grp_id.int_grpid_range.end;
    tmp["pattern"]     = int_grp_id.int_grpid_range.pattern;
    data["udmInfo"]["internalGroupIdentifiersRanges"].push_back(tmp);
  }
}

//------------------------------------------------------------------------------
void udr_profile::add_udr_info(const udr_info_t& info) {
  udr_info = info;
}

//------------------------------------------------------------------------------
void udr_profile::get_udr_info(udr_info_t& infos) const {
  infos = udr_info;
}

//------------------------------------------------------------------------------
void udr_profile::display() {
  nf_profile::display();
  sba_logger().debug("\tUDR Info");
  sba_logger().debug("\t\tGroupId: %s", udr_info.groupid);
  for (auto supi : udr_info.supi_ranges) {
    sba_logger().debug(
        "\t\t SupiRanges: Start - %s, End - %s, Pattern - %s",
        supi.supi_range.start, supi.supi_range.end, supi.supi_range.pattern);
  }
  for (auto gpsiRange : udr_info.gpsi_ranges) {
    sba_logger().debug(
        "\t\t GpsiRanges: Start - %s, End - %s, Pattern - %s",
        gpsiRange.identity_range.start, gpsiRange.identity_range.end,
        gpsiRange.identity_range.pattern);
  }
  for (auto ext_grp_id : udr_info.ext_grp_id_ranges) {
    sba_logger().debug(
        "\t\t externalGroupIdentifiersRanges: Start - %s, End - %s, Pattern - "
        "%s",
        ext_grp_id.identity_range.start, ext_grp_id.identity_range.end,
        ext_grp_id.identity_range.pattern);
  }
  for (auto data_set_id : udr_info.data_set_id) {
    sba_logger().debug("\t\t DataSetId: %s", data_set_id);
  }
}

//------------------------------------------------------------------------------
bool udr_profile::add_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::add_profile_info(path, value);
  if (result) return true;

  // add an element to a list of json object
  if (path.compare("udrInfo") == 0) {
    sba_logger().info("Does not support this operation for udrInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and (path.compare("fqdn") != 0) and
      (path.compare("plmnList") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("udrInfo") != 0)) {
    sba_logger().debug("Add new member: %s", path);
    // add new member
    json_data[path] = value;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool udr_profile::replace_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::replace_profile_info(path, value);
  if (result) return true;
  // for UDMAUSF info
  if (path.compare("udrInfo") == 0) {
    sba_logger().debug("Does not support this operation for udrInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and (path.compare("fqdn") != 0) and
      (path.compare("plmnList") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("udrInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }

  return false;
}

//------------------------------------------------------------------------------
bool udr_profile::remove_profile_info(const std::string& path) {
  bool result = nf_profile::remove_profile_info(path);
  if (result) return true;
  // for UDM info
  if (path.compare("udrInfo") == 0) {
    sba_logger().debug("Do not support this operation for udrInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and (path.compare("fqdn") != 0) and
      (path.compare("plmnList") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("udrInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }
  return false;
}

//------------------------------------------------------------------------------
void udr_profile::to_json(nlohmann::json& data) const {
  nf_profile::to_json(data);
  // UDR Info
  data["udrInfo"]["groupId"]                        = udr_info.groupid;
  data["udrInfo"]["supiRanges"]                     = nlohmann::json::array();
  data["udrInfo"]["gpsiRanges"]                     = nlohmann::json::array();
  data["udrInfo"]["externalGroupIdentifiersRanges"] = nlohmann::json::array();
  data["udrInfo"]["routingIndicators"]              = nlohmann::json::array();

  for (auto supi : udr_info.supi_ranges) {
    nlohmann::json tmp = {};
    tmp["start"]       = supi.supi_range.start;
    tmp["end"]         = supi.supi_range.end;
    tmp["pattern"]     = supi.supi_range.pattern;
    data["udrInfo"]["supiRanges"].push_back(tmp);
  }
  for (auto gpsi : udr_info.gpsi_ranges) {
    nlohmann::json tmp = {};
    tmp["start"]       = gpsi.identity_range.start;
    tmp["end"]         = gpsi.identity_range.end;
    tmp["pattern"]     = gpsi.identity_range.pattern;
    data["udrInfo"]["gpsiRanges"].push_back(tmp);
  }
  for (auto ext_grp_id : udr_info.ext_grp_id_ranges) {
    nlohmann::json tmp = {};
    tmp["start"]       = ext_grp_id.identity_range.start;
    tmp["end"]         = ext_grp_id.identity_range.end;
    tmp["pattern"]     = ext_grp_id.identity_range.pattern;
    data["udrInfo"]["externalGroupIdentifiersRanges"].push_back(tmp);
  }
  for (auto data_set_id : udr_info.data_set_id) {
    data["udrInfo"]["routingIndicators"].push_back(data_set_id);
  }
}

//------------------------------------------------------------------------------
void pcf_profile::add_pcf_info(const pcf_info_t& info) {
  pcf_info = info;
}

//------------------------------------------------------------------------------
void pcf_profile::get_pcf_info(pcf_info_t& infos) const {
  infos = pcf_info;
}

//------------------------------------------------------------------------------
void pcf_profile::display() {
  nf_profile::display();
  sba_logger().debug("\tUDR Info");
  sba_logger().debug("\t\tGroupId: %s", pcf_info.groupid);
  for (auto dnn : pcf_info.dnn_list) {
    sba_logger().debug("\t\t DNN: %s", dnn);
  }
  for (auto supi : pcf_info.supi_ranges) {
    sba_logger().debug(
        "\t\t SupiRanges: Start - %s, End - %s, Pattern - %s",
        supi.supi_range.start, supi.supi_range.end, supi.supi_range.pattern);
  }
  for (auto gpsiRange : pcf_info.gpsi_ranges) {
    sba_logger().debug(
        "\t\t GpsiRanges: Start - %s, End - %s, Pattern - %s",
        gpsiRange.identity_range.start, gpsiRange.identity_range.end,
        gpsiRange.identity_range.pattern);
  }
}

//------------------------------------------------------------------------------
bool pcf_profile::add_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::add_profile_info(path, value);
  if (result) return true;

  // add an element to a list of json object
  if (path.compare("pcfInfo") == 0) {
    sba_logger().info("Does not support this operation for pcfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and (path.compare("fqdn") != 0) and
      (path.compare("plmnList") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("pcfInfo") != 0)) {
    sba_logger().debug("Add new member: %s", path);
    // add new member
    json_data[path] = value;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool pcf_profile::replace_profile_info(
    const std::string& path, const std::string& value) {
  bool result = nf_profile::replace_profile_info(path, value);
  if (result) return true;
  // for UDMAUSF info
  if (path.compare("pcfInfo") == 0) {
    sba_logger().debug("Does not support this operation for pcfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and (path.compare("fqdn") != 0) and
      (path.compare("plmnList") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("pcfInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }

  return false;
}

//------------------------------------------------------------------------------
bool pcf_profile::remove_profile_info(const std::string& path) {
  bool result = nf_profile::remove_profile_info(path);
  if (result) return true;
  // for UDM info
  if (path.compare("pcfInfo") == 0) {
    sba_logger().debug("Do not support this operation for pcfInfo");
    return false;
  }

  if ((path.compare("nfInstanceId") != 0) and
      (path.compare("nfInstanceName") != 0) and
      (path.compare("nfType") != 0) and (path.compare("nfStatus") != 0) and
      (path.compare("heartBeatTimer") != 0) and (path.compare("fqdn") != 0) and
      (path.compare("plmnList") != 0) and
      (path.compare("ipv4Addresses") != 0) and
      (path.compare("priority") != 0) and (path.compare("capacity") != 0) and
      (path.compare("priority") != 0) and (path.compare("nfServices") != 0) and
      (path.compare("pcfInfo") != 0)) {
    sba_logger().debug("Member (%s) not found!", path);
    return false;
  }
  return false;
}

//------------------------------------------------------------------------------
void pcf_profile::to_json(nlohmann::json& data) const {
  nf_profile::to_json(data);
  // UDR Info
  data["pcfInfo"]["groupId"]    = pcf_info.groupid;
  data["pcfInfo"]["dnnList"]    = nlohmann::json::array();
  data["pcfInfo"]["supiRanges"] = nlohmann::json::array();
  data["pcfInfo"]["gpsiRanges"] = nlohmann::json::array();

  for (auto supi : pcf_info.supi_ranges) {
    nlohmann::json tmp = {};
    tmp["start"]       = supi.supi_range.start;
    tmp["end"]         = supi.supi_range.end;
    tmp["pattern"]     = supi.supi_range.pattern;
    data["pcfInfo"]["supiRanges"].push_back(tmp);
  }
  for (auto gpsi : pcf_info.gpsi_ranges) {
    nlohmann::json tmp = {};
    tmp["start"]       = gpsi.identity_range.start;
    tmp["end"]         = gpsi.identity_range.end;
    tmp["pattern"]     = gpsi.identity_range.pattern;
    data["pcfInfo"]["gpsiRanges"].push_back(tmp);
  }
  for (auto dnn : pcf_info.dnn_list) {
    data["pcfInfo"]["dnnList"].push_back(dnn);
  }
}
