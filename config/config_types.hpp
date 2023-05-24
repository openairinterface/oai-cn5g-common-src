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

/*! \file config_types.hpp
 \brief
 \author  Stefan Spettel
 \company phine.tech
 \date 2023
 \email: stefan.spettel@phine.tech
*/

#pragma once

#include "string"
#include <netinet/in.h>
#include <vector>
#include <memory>
#include <yaml-cpp/yaml.h>

namespace oai::config {
const std::string INNER_LIST_ELEM = "+";
const std::string OUTER_LIST_ELEM = "-";
const std::string EMPTY_LIST_ELEM = " ";

class config_type {
  friend class yaml_file_iface;

 public:
  /**
   * Returns a string representation of the config. The indent is prepended at
   * each line
   * @param indent to be prepended
   * @return string representation
   */
  [[nodiscard]] virtual std::string to_string(
      const std::string& indent) const = 0;

  /**
   * Validates the configuration
   * @throws std::runtime_error if validation is not successful
   */
  virtual void validate(){};

  /**
   * Overwrites the values from a YAML node
   * @param node YAML node, non-existing values are not evaluated, but types
   * must be correct
   * @throws YAML::Exception if YAML conversion failed
   */
  virtual void from_yaml(const YAML::Node& node) = 0;

  /**
   * Convert to JSON format
   * @return void
   */
  virtual void to_json(){};

  /**
   * Checks if the configuration is set. Configuration is not set if it has not
   * been validated.
   * @return true if set, false otherwise
   */
  [[nodiscard]] virtual bool is_set() const;

  /**
   * Set the configuration
   * @return true if set, false otherwise
   */
  virtual void set_config();

  /**
   * Unset the configuration
   * @return true if set, false otherwise
   */
  virtual void unset_config();

  /**
   * Returns the name of this configuration in a user-friendly representation
   * @return
   */
  [[nodiscard]] virtual const std::string& get_config_name() const;

  virtual ~config_type() = default;

 protected:
  bool m_set = false;
  std::string m_config_name;

  static unsigned int get_inner_width(unsigned int indent_length);
};

class string_config_value : public config_type {
 private:
  std::string m_value;
  std::string m_regex = ".*";

 public:
  explicit string_config_value(
      const std::string& name, const std::string& value);
  string_config_value() = default;

  void from_yaml(const YAML::Node& node) override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  void validate() override;

  [[nodiscard]] const std::string& get_value() const;
  void set_validation_regex(const std::string& regex);
};

class option_config_value : public config_type {
 private:
  bool m_value = false;

 public:
  explicit option_config_value(const std::string& name, bool value);
  option_config_value() = default;

  void from_yaml(const YAML::Node& node) override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;

  [[nodiscard]] bool get_value() const;
};

class int_config_value : public config_type {
 private:
  int m_value     = 0;
  int m_min_value = 0;
  int m_max_value = INT32_MAX;

 public:
  explicit int_config_value(const std::string& name, int value);
  int_config_value() = default;

  void from_yaml(const YAML::Node& node) override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  void validate() override;

  [[nodiscard]] int get_value() const;
  void set_validation_interval(int min, int max);
};

class local_interface : public config_type {
 protected:
  string_config_value m_host{};
  int_config_value m_port{};
  [[nodiscard]] std::string to_string_for_local(
      const std::string& indent) const;

 private:
  string_config_value m_if_name{};
  // these values are read from the m_if_name
  in_addr m_addr4{};
  in6_addr m_addr6{};
  unsigned int m_mtu{};
  bool m_is_local_interface = false;

 public:
  explicit local_interface(
      const std::string& name, const std::string& host, uint16_t port,
      const std::string& if_name);

  local_interface() = default;

  void from_yaml(const YAML::Node& node) override;
  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  void validate() override;

  [[nodiscard]] const std::string& get_host() const;
  [[nodiscard]] const std::string& get_if_name() const;
  [[nodiscard]] const in_addr& get_addr4() const;
  [[nodiscard]] const in6_addr& get_addr6() const;
  [[nodiscard]] unsigned int get_mtu() const;
  [[nodiscard]] uint16_t get_port() const;

  void set_is_local_interface(bool val);
  bool is_local_interface() const;
};

class sbi_interface : public local_interface {
 private:
  string_config_value m_api_version;
  int_config_value m_port_http1;
  int_config_value m_port_http2;
  std::string m_url;

  void set_url();

 public:
  explicit sbi_interface(
      const std::string& name, const std::string& host, uint16_t port_http1,
      uint16_t port_http2, const std::string& api_version,
      const std::string& interface_name);

  sbi_interface() = default;

  void from_yaml(const YAML::Node& node) override;
  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  void validate() override;

  [[nodiscard]] const std::string& get_api_version() const;
  [[nodiscard]] const std::string& get_url() const;
  [[nodiscard]] uint16_t get_port_http1() const;
  [[nodiscard]] uint16_t get_port_http2() const;
  [[nodiscard]] bool use_http2() const;
};

enum class interface_type_e { n1, n4 };

class nf : public config_type {
  friend class yaml_file;
  friend class config;

 private:
  sbi_interface m_sbi;
  local_interface m_nx;
  string_config_value m_host;

  std::string m_url;  // Moved from SBI interface
  void set_url();     // Moved from SBI interface

 public:
  explicit nf(
      const std::string& name, const std::string& host,
      const sbi_interface& sbi, const local_interface& local);
  explicit nf(
      const std::string& name, const std::string& host,
      const sbi_interface& sbi);
  explicit nf() = default;

  void from_yaml(const YAML::Node& node) override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  void validate() override;
  [[nodiscard]] const sbi_interface& get_sbi() const;
  [[nodiscard]] const local_interface& get_nx() const;
  [[nodiscard]] const std::string& get_host() const;
  [[nodiscard]] const std::string& get_url() const;
};

class policy_config : public config_type {
 private:
  string_config_value m_pcc_rules_path;
  string_config_value m_policy_decisions_path;
  string_config_value m_traffic_rules_path;

 public:
  explicit policy_config();
  explicit policy_config(
      const std::string& policy_decisions_path,
      const std::string& pcc_rules_path, const std::string& traffic_rules_path);

  void from_yaml(const YAML::Node& node) override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  [[nodiscard]] const std::string& get_pcc_rules_path() const;
  [[nodiscard]] const std::string& get_policy_decisions_path() const;
  [[nodiscard]] const std::string& get_traffic_rules_path() const;
};

class nf_features_config : public config_type {
 private:
  // TODO: either have string or option
  string_config_value m_string_value{};
  option_config_value m_option_value{};
  std::string m_nf_name;

  void set_value(const YAML::Node& node);

 public:
  explicit nf_features_config(
      const std::string& name, const std::string& nf_name, bool value);

  explicit nf_features_config(
      const std::string& name, const std::string& nf_name,
      const std::string& value);

  void from_yaml(const YAML::Node& node) override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  void validate() override;
  void set_validation_regex(const std::string& regex);
  [[nodiscard]] bool get_option() const;
  [[nodiscard]] const std::string& get_string() const;
};

class database_config : public config_type {
 private:
  string_config_value m_host;
  string_config_value m_user;
  string_config_value m_pass;
  string_config_value m_database_name;
  string_config_value m_database_type;
  option_config_value m_random;
  int_config_value m_connection_timeout;

 public:
  explicit database_config();

  void from_yaml(const YAML::Node& node) override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  [[nodiscard]] const std::string& get_host() const;
  [[nodiscard]] const std::string& get_user() const;
  [[nodiscard]] const std::string& get_pass() const;
  [[nodiscard]] const std::string& get_database_name() const;
  [[nodiscard]] const std::string& get_database_type() const;
  [[nodiscard]] const bool get_random() const;
  [[nodiscard]] const int get_connection_timeout() const;
};
}  // namespace oai::config
