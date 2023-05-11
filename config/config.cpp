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
      m_pcf_policy(
          "/openair-pcf/policies/policy_decisions",
          "/openair-pcf/policies/pcc_rules",
          "/openair-pcf/policies/traffic_rules") {
  logger::logger_registry::register_logger(
      nf_name, LOGGER_NAME, log_stdout, log_rot_file);

  m_log_level_feature.set_validation_regex(LOG_LVL_VALIDATOR_REGEX);

  m_config_path = config_path;
  m_nf_name     = nf_name;

  /*
  //ONlY create and store necessary NFs
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
  */
}

void config::read_from_file(const std::string& file_path) {
  try {
    YAML::Node node = YAML::LoadFile(file_path);
    for (const auto& elem : node) {
      auto key = elem.first.as<std::string>();
      if (m_used_config_values.find(key) == m_used_config_values.end()) {
        continue;
      }
      try {
        if (key == LOG_LEVEL_CONFIG_NAME) {
          m_log_level_feature.from_yaml(elem.second);
        } else if (key == REGISTER_NF_CONFIG_NAME) {
          m_register_nrf_feature.from_yaml(elem.second);
        } else if (key == PCF_CONFIG_NAME) {
          // TODO FOR PCF: read_pcf_config(elem.second, config);
        } else if (key == AMF_CONFIG_NAME) {
          const auto nf_ptr = m_nf_map.find(AMF_CONFIG_NAME);
          if (nf_ptr == m_nf_map.end()) {
            logger::logger_registry::get_logger(LOGGER_NAME)
                .info(
                    "Unknown NF %s in configuration. Ignored", AMF_CONFIG_NAME);
            continue;
          }

          try {
            nf_ptr->second->from_yaml(elem.second);
          } catch (std::exception& e) {
            logger::logger_registry::get_logger(LOGGER_NAME)
                .warn("Could not parse %s: %s", AMF_CONFIG_NAME, e.what());
          }
        } else if (key == NF_LIST_CONFIG_NAME) {
          for (auto yaml_nf : elem.second) {
            auto nf_name = yaml_nf.first.as<std::string>();

            const auto nf_ptr = m_nf_map.find(nf_name);
            if (nf_ptr == m_nf_map.end()) {
              logger::logger_registry::get_logger(LOGGER_NAME)
                  .info("Unknown NF %s in configuration. Ignored", nf_name);
              continue;
            }
            if (m_used_sbi_values.find(nf_name) == m_used_sbi_values.end()) {
              // we unset the values that are not used by this NF -> they are
              // not validated and not printed
              nf_ptr->second->m_set = false;
              continue;
            }
            try {
              nf_ptr->second->from_yaml(yaml_nf.second);
            } catch (std::exception& e) {
              logger::logger_registry::get_logger(LOGGER_NAME)
                  .warn("Could not parse %s: %s", nf_name, e.what());
            }
          }
        }
      } catch (std::exception& e) {
        logger::logger_registry::get_logger(LOGGER_NAME)
            .warn("Could not parse %s: %s", key, e.what());
      }
    }
  } catch (YAML::BadFile& ex) {
    logger::logger_registry::get_logger(LOGGER_NAME)
        .error(
            "Could not read YAML configuration file, please ensure that it "
            "exists: %s",
            ex.what());
    throw std::runtime_error(ex.what());
  } catch (std::exception& ex) {
    logger::logger_registry::get_logger(LOGGER_NAME)
        .error("Could not parse YAML configuration file: %s", ex.what());
    throw std::runtime_error(ex.what());
  }
  update_used_nfs();
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
  out.append(m_pcf_policy.to_string(indent));

  // TODO rest of the fields

  return out;
}

void config::display() const {
  logger::logger_registry::set_level(spdlog::level::info);
  std::stringstream ss(to_string());
  std::string line;

  logger::logger_registry::get_logger(LOGGER_NAME)
      .info("==== OPENAIRINTERFACE %s v%s ====", m_nf_name, PACKAGE_VERSION);

  while (std::getline(ss, line)) {
    logger::logger_registry::get_logger(LOGGER_NAME).info(line);
  }
  logger::logger_registry::set_level(spdlog::level::from_str(log_level()));
}

bool config::init() {
  try {
    logger::logger_registry::get_logger(LOGGER_NAME)
        .info("Reading NF configuration from %s", m_config_path);
    read_from_file(m_config_path);
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

const amf& config::get_amf() const {
  return *m_amf;
}

const nf& config::get_smf() const {
  return *m_smf;
}

const nf& config::get_nrf() const {
  return *m_nrf;
}

const nf& config::get_pcf() const {
  return *m_pcf;
}

const nf& config::get_ausf() const {
  return *m_ausf;
}

const nf& config::get_udm() const {
  return *m_udm;
}

const nf& config::get_udr() const {
  return *m_udr;
}

const nf& config::get_nssf() const {
  return *m_nssf;
}

const nf& config::local() const {
  return *m_local_nf;
}

const class policy_config& config::get_pcf_policy() const {
  return m_pcf_policy;
}

void config::update_used_nfs() {
  // TODO with NF_Type and switch
  if (!m_nf_name.compare(AMF_CONFIG_NAME)) {
    logger::logger_registry::get_logger(LOGGER_NAME)
        .warn("NF Name %s", AMF_CONFIG_NAME);
    m_amf = std::make_shared<amf>(
        "AMF", "oai-amf", sbi_interface("SBI", "oai-amf", 80, 0, "v1", "eth0"),
        local_interface("N1", "oai-amf", 38412, "eth0"), interface_type_e::n1);
    m_nf_map.insert(std::make_pair(AMF_CONFIG_NAME, m_amf));
  }
  for (auto& used_nf : m_used_sbi_values) {
    if (!used_nf.compare(SMF_CONFIG_NAME)) {
      m_smf = std::make_shared<nf>(
          "SMF", "oai-smf",
          sbi_interface("SBI", "oai-smf", 80, 0, "v1", "eth0"));
      m_nf_map.insert(std::make_pair(SMF_CONFIG_NAME, m_smf));
    }
  }

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
