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

/*! \file config.cpp
 \brief
 \author  Stefan Spettel
 \company phine.tech
 \date 2022
 \email: stefan.spettel@phine.tech
*/

#include "config.hpp"
#include "if.hpp"
#include "config_yaml_file.hpp"

#include <fmt/format.h>
#include <stdexcept>
#include <regex>

using namespace oai::config;

config::config(
    const std::string& config_path, const std::string& nf_name, bool log_stdout,
    bool log_rot_file)
    : m_log_level_feature("Log Level", nf_name, std::string("info")),
      m_register_nrf_feature("Register NF", nf_name, false),
      m_policy(
          "/openair-pcf/policies/policy_decisions",
          "/openair-pcf/policies/pcc_rules",
          "/openair-pcf/policies/traffic_rules") {
  logger::logger_registry::register_logger(
      nf_name, LOGGER_NAME, log_stdout, log_rot_file);

  m_log_level_feature.set_validation_regex(LOG_LVL_VALIDATOR_REGEX);

  m_config_path = config_path;
  m_nf_name     = nf_name;

  m_amf = std::make_shared<nf>(
      "AMF", "oai-amf", sbi_interface("SBI", "oai-amf", 80, 0, "v1", "eth0"),
      local_interface("N1", "oai-amf", 38412, "eth0"), interface_type_e::n1);

  m_smf = std::make_shared<nf>(
      "SMF", "oai-smf", sbi_interface("SBI", "oai-smf", 80, 0, "v1", "eth0"),
      local_interface("N4", "oai-smf", 8805, "eth0"), interface_type_e::n4);

  m_nrf = std::make_shared<nf>(
      "NRF", "oai-nrf", sbi_interface("SBI", "oai-nrf", 80, 0, "v1", "eth0"));

  m_udm = std::make_shared<nf>(
      "UDM", "oai-udm", sbi_interface("SBI", "oai-udm", 80, 0, "v1", "eth0"));

  m_udr = std::make_shared<nf>(
      "UDR", "oai-udr", sbi_interface("SBI", "oai-udr", 80, 0, "v1", "eth0"));

  m_pcf = std::make_shared<nf>(
      "PCF", "oai-pcf", sbi_interface("SBI", "oai-pcf", 80, 0, "v1", "eth0"));

  m_ausf = std::make_shared<nf>(
      "AUSF", "oai-ausf",
      sbi_interface("SBI", "oai-ausf", 80, 0, "v1", "eth0"));

  m_nssf = std::make_shared<nf>(
      "NSSF", "oai-nssf",
      sbi_interface("SBI", "oai-nssf", 80, 0, "v1", "eth0"));

  // we use a map to have easy mapping from string value to pointer
  m_nf_map.insert(std::make_pair(AMF_CONFIG_NAME, m_amf));
  m_nf_map.insert(std::make_pair(SMF_CONFIG_NAME, m_smf));
  m_nf_map.insert(std::make_pair(NRF_CONFIG_NAME, m_nrf));
  m_nf_map.insert(std::make_pair(AUSF_CONFIG_NAME, m_ausf));
  m_nf_map.insert(std::make_pair(UDM_CONFIG_NAME, m_udm));
  m_nf_map.insert(std::make_pair(UDR_CONFIG_NAME, m_udr));
  m_nf_map.insert(std::make_pair(NSSF_CONFIG_NAME, m_nssf));
  m_nf_map.insert(std::make_pair(PCF_CONFIG_NAME, m_pcf));
}

bool config::validate() {
  std::shared_lock lock(m_config_mutex);
  bool success = true;

  success &= safe_validate_field(m_log_level_feature);
  // we set log level here to not print debug here, but first debug message is
  // printed
  if (success) {
    logger::logger_registry::set_level(spdlog::level::from_str(log_level()));
  }
  success &= safe_validate_field(m_register_nrf_feature);
  for (auto& nf : m_nf_map) {
    success &= safe_validate_field(*nf.second);
  }

  return success;
}

bool config::safe_validate_field(config_type& config) {
  try {
    logger::logger_registry::get_logger(LOGGER_NAME)
        .debug("Validating configuration of %s", config.get_config_name());
    config.validate();
    return true;
  } catch (std::exception& e) {
    logger::logger_registry::get_logger(LOGGER_NAME)
        .error(
            "Validation of %s not successful: %s", config.get_config_name(),
            e.what());
    return false;
  }
}

std::string config::to_string() const {
  std::shared_lock lock(m_config_mutex);

  std::string base_conf_out;
  std::string support_features_out;
  std::string sbi_out;
  std::string local_iface_out;
  std::string others_out;

  std::string out;
  out.append("Basic Configuration:\n");
  std::string indent = fmt::format("{:<{}}", "", INDENT_WIDTH);
  out.append(m_log_level_feature.to_string(indent));
  out.append(m_register_nrf_feature.to_string(indent));
  out.append("Local NF Configuration:\n");
  out.append(m_local_nf->to_string(indent));
  out.append("Peer NF Configuration:\n");
  for (const auto& nf : m_nf_map) {
    if (nf.first != m_nf_name) {
      out.append(nf.second->to_string(indent));
    }
  }
  out.append(m_policy.to_string(indent));

  // TODO rest of the fields

  return out;
}

void config::display() const {
  std::stringstream ss(to_string());
  std::string line;

  logger::logger_registry::get_logger(LOGGER_NAME)
      .info("==== OPENAIRINTERFACE %s v%s ====", m_nf_name, PACKAGE_VERSION);

  while (std::getline(ss, line)) {
    logger::logger_registry::get_logger(LOGGER_NAME).info(line);
  }
}

bool config::init() {
  yaml_file file;
  try {
    logger::logger_registry::get_logger(LOGGER_NAME)
        .info("Reading NF configuration from %s", m_config_path);
    file.read_from_file(m_config_path, *this);
  } catch (std::runtime_error& err) {
    return false;
  }

  if (!validate()) {
    logger::logger_registry::get_logger(LOGGER_NAME)
        .error("Configuration validation not successful!");
    return false;
  }
  return true;
}

bool config::register_nrf() const {
  return m_register_nrf_feature.get_option();
}

const std::string& config::log_level() const {
  return m_log_level_feature.get_string();
}

const nf& config::amf() const {
  return *m_amf;
}

const nf& config::smf() const {
  return *m_smf;
}

const nf& config::nrf() const {
  return *m_nrf;
}

const nf& config::pcf() const {
  return *m_pcf;
}

const nf& config::ausf() const {
  return *m_ausf;
}

const nf& config::udm() const {
  return *m_udm;
}

const nf& config::udr() const {
  return *m_udr;
}

const nf& config::nssf() const {
  return *m_nssf;
}

const nf& config::local() const {
  return *m_local_nf;
}

const class policy_config& config::get_policy() const {
  return m_policy;
}

void config::update_used_nfs() {
  for (auto& nf : m_nf_map) {
    if (nf.first == m_nf_name) {
      m_local_nf = nf.second;
      m_local_nf->m_sbi.set_is_local_interface(true);
      m_local_nf->m_n1.set_is_local_interface(true);
      m_local_nf->m_n4.set_is_local_interface(true);
    } else {
      auto used_nf = m_used_sbi_values.find(nf.first);
      if (used_nf == m_used_sbi_values.end()) {
        nf.second->m_set = false;
      }
      if (register_nrf() && nf.first != "nrf") {
        nf.second->m_set = false;
      }
    }
  }
}
