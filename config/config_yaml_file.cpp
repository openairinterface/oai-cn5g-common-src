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

/*! \file config_yaml_file.cpp
 \brief
 \author  Stefan Spettel
 \company phine.tech
 \date 2022
 \email: stefan.spettel@phine.tech
*/

#include "config_yaml_file.hpp"
#include "logger_base.hpp"

using namespace oai::logger;

void oai::config::yaml_file::read_from_file(
    const std::string& file_path, oai::config::config& config) {
  try {
    YAML::Node node = YAML::LoadFile(file_path);
    for (const auto& elem : node) {
      auto key = elem.first.as<std::string>();
      if (config.m_used_config_values.find(key) ==
          config.m_used_config_values.end()) {
        continue;
      }
      try {
        if (key == LOG_LEVEL_CONFIG_NAME) {
          config.m_log_level_feature.from_yaml(elem.second);
        } else if (key == REGISTER_NF_CONFIG_NAME) {
          config.m_register_nrf_feature.from_yaml(elem.second);
        } else if (key == LOCAL_POLICY_CONFIG_NAME) {
          config.m_policy.from_yaml(elem.second);
        } else if (key == NF_LIST_CONFIG_NAME) {
          for (auto yaml_nf : elem.second) {
            auto nf_name = yaml_nf.first.as<std::string>();

            const auto nf_ptr = config.m_nf_map.find(nf_name);
            if (nf_ptr == config.m_nf_map.end()) {
              logger_registry::get_logger(LOGGER_NAME)
                  .info("Unknown NF %s in configuration. Ignored", nf_name);
              continue;
            }
            if (config.m_used_sbi_values.find(nf_name) ==
                config.m_used_sbi_values.end()) {
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
    logger_registry::get_logger(LOGGER_NAME)
        .error(
            "Could not read YAML configuration file, please ensure that it "
            "exists: %s",
            ex.what());
    throw std::runtime_error(ex.what());
  } catch (std::exception& ex) {
    logger_registry::get_logger(LOGGER_NAME)
        .error("Could not parse YAML configuration file: %s", ex.what());
    throw std::runtime_error(ex.what());
  }
  config.update_used_nfs();
}