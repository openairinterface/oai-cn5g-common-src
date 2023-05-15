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

/*! \file config.hpp
 \brief
 \author  Stefan Spettel
 \company phine.tech
 \date 2022
 \email: stefan.spettel@phine.tech
*/

#pragma once

#include "config_types.hpp"
#include "logger_base.hpp"

#include <string>
#include <map>
#include <memory>
#include <shared_mutex>
#include <unordered_set>

namespace oai::config {

const int COLUMN_WIDTH           = 30;
const int INDENT_WIDTH           = 2;
const std::string BASE_FORMATTER = "{} {:.<{}}: {}\n";
const std::string LOGGER_NAME    = "config ";

// common validator REGEX

const std::string LOG_LVL_VALIDATOR_REGEX = "debug|info|warning|error|off";
const std::string IPV4_ADDRESS_VALIDATOR_REGEX =
    "^((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)\\.?\\b){4}$";
const std::string IPV6_ADDRESS_VALIDATOR_REGEX = ".*";  // TODO
const std::string HOSTNAME_VALIDATOR_REGEX =
    "^([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]{0,61}[a-zA-Z0-9])(\\.([a-zA-Z0-9]|"
    "[a-zA-Z0-9][a-zA-Z0-9\\-]{0,61}[a-zA-Z0-9]))*$";
const std::string API_VERSION_REGEX = "v1|v2";
const std::string HOST_VALIDATOR_REGEX =
    IPV4_ADDRESS_VALIDATOR_REGEX + "|" + HOSTNAME_VALIDATOR_REGEX;

const uint16_t PORT_MIN_VALUE = 1;
const uint16_t PORT_MAX_VALUE = 65535;

const std::string PCF_CONFIG_NAME  = "pcf";
const std::string NRF_CONFIG_NAME  = "nrf";
const std::string SMF_CONFIG_NAME  = "smf";
const std::string AMF_CONFIG_NAME  = "amf";
const std::string UDM_CONFIG_NAME  = "udm";
const std::string UDR_CONFIG_NAME  = "udr";
const std::string NSSF_CONFIG_NAME = "nssf";
const std::string AUSF_CONFIG_NAME = "ausf";

const std::string LOG_LEVEL_CONFIG_NAME    = "log_level";
const std::string REGISTER_NF_CONFIG_NAME  = "register_nf";
const std::string NF_LIST_CONFIG_NAME      = "nfs";
const std::string LOCAL_POLICY_CONFIG_NAME = "local_policy";

// NF
constexpr auto NF_CONFIG_HOST_NAME = "host";

// Database (AMF/UDR)
constexpr auto DATABASE_CONFIG                    = "database";
constexpr auto DATABASE_CONFIG_USER               = "user";
constexpr auto DATABASE_CONFIG_PASSWORD           = "password";
constexpr auto DATABASE_CONFIG_DATABASE_NAME      = "database_name";
constexpr auto DATABASE_CONFIG_DATABASE_TYPE      = "database_type";
constexpr auto DATABASE_CONFIG_RANDOM             = "random";
constexpr auto DATABASE_CONFIG_CONNECTION_TIMEOUT = "connection_timeout";

class config_iface {
 public:
  /**
   * Validates the configuration:
   *  - All configurations set as mandatory must be present
   *  - All present configurations must pass their type-specific validation
   * @return True if validation passed, false otherwise
   */
  [[nodiscard]] virtual bool validate() = 0;

  /**
   * Returns a string representation of the configuration
   * @return string representation
   */
  [[nodiscard]] virtual std::string to_string() const = 0;

  /**
   * Displays the to_string method to the config logger
   */
  virtual void display() const = 0;

  virtual ~config_iface() = default;

  [[nodiscard]] virtual bool register_nrf() const = 0;

  [[nodiscard]] virtual const std::string& log_level() const = 0;

  [[nodiscard]] virtual const nf& local() const = 0;

  [[nodiscard]] virtual std::shared_ptr<nf> get_local() = 0;

  [[nodiscard]] virtual const policy_config& get_pcf_policy() const        = 0;
  [[nodiscard]] virtual const database_config& get_database_config() const = 0;

  /**
   * Initializes the configuration, reads YAML configuration file and validates
   * the configuration
   * @return True on success
   */
  virtual bool init() = 0;
};

class config : public config_iface {
  // friend class yaml_file;

 public:
  explicit config(
      const std::string& config_path, const std::string& nf_name,
      bool log_stdout, bool log_rot_file);

  [[nodiscard]] bool validate() override;

  [[nodiscard]] std::string to_string() const override;

  [[nodiscard]] bool register_nrf() const override;

  [[nodiscard]] const std::string& log_level() const override;

  [[nodiscard]] const nf& local() const override;
  [[nodiscard]] std::shared_ptr<nf> get_local() override;

  [[nodiscard]] const policy_config& get_pcf_policy() const override;
  [[nodiscard]] const database_config& get_database_config() const override;

  bool init() override;

  void display() const override;

  void read_from_file(const std::string& file_path);

 protected:
  // to define for each NF which values are used
  std::unordered_set<std::string> m_used_config_values;
  std::unordered_set<std::string> m_used_sbi_values;
  std::string m_nf_name;

  void update_used_nfs();
  bool add_nf(const std::string& name, const std::shared_ptr<nf>& nf_ptr);

 private:
  std::string m_config_path;

  nf_features_config m_log_level_feature;
  nf_features_config m_register_nrf_feature;

  std::shared_ptr<nf> m_local_nf;

  // TODO: should not included in common Config
  policy_config m_pcf_policy;
  database_config m_database;

  std::unordered_map<std::string, std::shared_ptr<nf>> m_nf_map;

  mutable std::shared_mutex m_config_mutex;

  static bool safe_validate_field(config_type& config);
};

}  // namespace oai::config
