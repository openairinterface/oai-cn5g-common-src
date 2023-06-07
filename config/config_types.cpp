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

/*! \file config_types.cpp
 \brief
 \author  Stefan Spettel
 \company phine.tech
 \date 2022
 \email: stefan.spettel@phine.tech
*/

#include "config_types.hpp"
#include "config.hpp"
#include "conversions.hpp"
#include "if.hpp"
#include "common_defs.h"

#include <fmt/format.h>
#include <string>
#include <regex>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace oai::config;

bool config_type::is_set() const {
  return m_set;
}

void config_type::set_config() {
  m_set = true;
}

void config_type::unset_config() {
  m_set = false;
}

const std::string& config_type::get_config_name() const {
  return m_config_name;
}

unsigned int config_type::get_inner_width(unsigned int indent_length) {
  unsigned int inner_width = COLUMN_WIDTH;
  if (indent_length < COLUMN_WIDTH) {
    inner_width = COLUMN_WIDTH - indent_length;
  }

  return inner_width;
}

in_addr config_type::safe_convert_ip(const std::string& ipv4_string) {
  in_addr ip = conv::fromString(ipv4_string);

  if (!ip.s_addr) {
    throw std::runtime_error(
        fmt::format(+"The IP address {} is not valid", ipv4_string));
  }
  return ip;
}

in6_addr config_type::safe_convert_ip6(const std::string& ipv6_string) {
  in6_addr ip = conv::fromStringV6(ipv6_string);
  if (IN6_IS_ADDR_UNSPECIFIED(&ip)) {
    throw std::runtime_error(
        fmt::format(+"The IPv6 address {} is not valid", ipv6_string));
  }
  return ip;
}

std::string config_type::add_indent(const std::string& indent) {
  std::string base_indent = fmt::format("{:<{}}", "", INDENT_WIDTH);
  return base_indent + indent;
}

string_config_value::string_config_value(
    const std::string& name, const std::string& value) {
  m_config_name = name;
  m_value       = value;
  m_set         = true;
}

void string_config_value::from_yaml(const YAML::Node& node) {
  m_value = node.as<std::string>();
  m_set   = true;
}

std::string string_config_value::to_string(const std::string&) const {
  std::string out;
  return out.append(m_value);
}

void string_config_value::validate() {
  if (!m_set) return;
  std::regex re(m_regex);
  if (!std::regex_match(m_value, re)) {
    throw std::runtime_error(fmt::format(
        "{} does not follow the regex specification: {}", m_value, m_regex));
  }
}

const std::string& string_config_value::get_value() const {
  return m_value;
}

void string_config_value::set_validation_regex(const std::string& regex) {
  m_regex = regex;
}

option_config_value::option_config_value(const std::string& name, bool value) {
  m_config_name = name;
  m_value       = value;
  m_set         = true;
}

void option_config_value::from_yaml(const YAML::Node& node) {
  m_value = node.as<bool>();
  m_set   = true;
}

std::string option_config_value::to_string(const std::string&) const {
  std::string val = m_value ? "Yes" : "No";
  return val;
}

bool option_config_value::get_value() const {
  return m_value;
}

int_config_value::int_config_value(const std::string& name, int value) {
  m_config_name = name;
  m_value       = value;
  m_set         = true;
}

void int_config_value::from_yaml(const YAML::Node& node) {
  m_value = node.as<int>();
  m_set   = true;
}

std::string int_config_value::to_string(const std::string&) const {
  return std::to_string(m_value);
}

void int_config_value::validate() {
  if (!m_set) return;
  if (m_value < m_min_value || m_value > m_max_value) {
    throw std::runtime_error(fmt::format(
        "Value {} must be in interval [{},{}]", m_value, m_min_value,
        m_max_value));
  }
}

int int_config_value::get_value() const {
  return m_value;
}

void int_config_value::set_validation_interval(int min, int max) {
  m_min_value = min;
  m_max_value = max;
}

local_interface::local_interface(
    const std::string& name, const std::string& host, uint16_t port,
    const std::string& if_name) {
  m_host        = string_config_value("Host", host);
  m_config_name = name;
  m_if_name     = string_config_value("Interface", if_name);
  m_port        = int_config_value("Port", port);
  m_port.set_validation_interval(PORT_MIN_VALUE, PORT_MAX_VALUE);
  m_host.set_validation_regex(HOST_VALIDATOR_REGEX);
  m_set                = true;
  m_is_local_interface = true;
}

void local_interface::from_yaml(const YAML::Node& node) {
  if (node["port"]) {
    m_port.from_yaml(node["port"]);
  }
  if (node["interface_name"]) {
    m_if_name.from_yaml(node["interface_name"]);
  }
  m_set                = true;
  m_is_local_interface = true;
}

std::string local_interface::to_string(const std::string& indent) const {
  std::string out;
  unsigned int inner_width = get_inner_width(indent.length());

  out.append(indent).append(fmt::format(
      BASE_FORMATTER, INNER_LIST_ELEM, "Port", inner_width,
      m_port.get_value()));

  if (!m_is_local_interface) return out;
  out.append(to_string_for_local(indent));

  return out;
}

std::string local_interface::to_string_for_local(
    const std::string& indent) const {
  unsigned int inner_width = get_inner_width(indent.length());
  std::string out;

  std::string ip4 = conv::toString(m_addr4);
  std::string ip6 = conv::toString(m_addr6);

  out.append(indent).append(fmt::format(
      BASE_FORMATTER, INNER_LIST_ELEM, "IPv4 Address ", inner_width, ip4));
  if (ip6 != "::") {
    out.append(indent).append(fmt::format(
        BASE_FORMATTER, INNER_LIST_ELEM, "IPv6 Address", inner_width, ip6));
  }
  out.append(indent).append(
      fmt::format(BASE_FORMATTER, INNER_LIST_ELEM, "MTU", inner_width, m_mtu));
  out.append(indent).append(fmt::format(
      BASE_FORMATTER, INNER_LIST_ELEM, "Interface name: ", inner_width,
      m_if_name.get_value()));
  return out;
}

void local_interface::validate() {
  if (!m_set) return;
  m_host.validate();
  m_port.validate();
  if (!m_is_local_interface) return;

  m_if_name.validate();

  unsigned int _mtu{};
  in_addr _addr4{};
  in_addr _netmask{};
  if (get_inet_addr_infos_from_iface(
          m_if_name.get_value(), _addr4, _netmask, _mtu) == RETURNerror) {
    throw std::runtime_error(fmt::format(
        "Error in reading network interface {}. Make sure it exists",
        m_if_name.get_value()));
  }
  m_mtu   = _mtu;
  m_addr4 = _addr4;
}

const std::string& local_interface::get_host() const {
  return m_host.get_value();
}

const std::string& local_interface::get_if_name() const {
  return m_if_name.get_value();
}

const in_addr& local_interface::get_addr4() const {
  return m_addr4;
}

const in6_addr& local_interface::get_addr6() const {
  return m_addr6;
}

unsigned int local_interface::get_mtu() const {
  return m_mtu;
}

uint16_t local_interface::get_port() const {
  return m_port.get_value();
}

void local_interface::set_is_local_interface(bool val) {
  m_is_local_interface = val;
}

bool local_interface::is_local_interface() const {
  return m_is_local_interface;
}

sbi_interface::sbi_interface(
    const std::string& name, const std::string& host, uint16_t port,
    const std::string& api_version, const std::string& interface_name)
    : local_interface(name, host, port, interface_name) {
  m_config_name = name;
  m_host        = string_config_value("Host", host);
  m_api_version = string_config_value("API Version", api_version);

  m_host.set_validation_regex(HOST_VALIDATOR_REGEX);
  m_api_version.set_validation_regex(API_VERSION_REGEX);
  m_set = true;
  set_is_local_interface(false);
  set_url();
}

void sbi_interface::from_yaml(const YAML::Node& node) {
  local_interface::from_yaml(node);
  set_is_local_interface(false);

  if (node["api_version"]) {
    m_api_version.from_yaml(node["api_version"]);
  }
  set_url();
  m_set = true;
}

std::string sbi_interface::to_string(const std::string& indent) const {
  std::string out;
  unsigned int inner_width = get_inner_width(indent.length());

  out.append(indent).append(
      fmt::format(BASE_FORMATTER, INNER_LIST_ELEM, "URL", inner_width, m_url));
  out.append(indent).append(fmt::format(
      BASE_FORMATTER, INNER_LIST_ELEM, "API Version", inner_width,
      m_api_version.get_value()));

  return out;
}

void sbi_interface::validate() {
  if (!m_set) return;
  local_interface::validate();
  m_api_version.validate();
}

const std::string& sbi_interface::get_api_version() const {
  return m_api_version.get_value();
}

const std::string& sbi_interface::get_url() const {
  return m_url;
}

void sbi_interface::set_url() {
  m_url = "";
  // this is easily adaptable to HTTPS, just add a flag, and we change the URL
  m_url.append("http://")
      .append(get_host())
      .append(":")
      .append(std::to_string(get_port()));
}

nf::nf(
    const std::string& name, const std::string& host, const sbi_interface& sbi,
    const local_interface& local)
    : nf(name, host, sbi) {
  m_nx = local;
  set_url();
}

nf::nf(
    const std::string& name, const std::string& host, const sbi_interface& sbi)
    : m_nx() {
  m_config_name = name;
  m_host        = string_config_value("Host", host);
  m_sbi         = sbi;
  m_set         = true;
  m_host.set_validation_regex(HOST_VALIDATOR_REGEX);
  set_url();
}

void nf::from_yaml(const YAML::Node& node) {
  if (node[NF_CONFIG_HOST_NAME]) {
    m_host.from_yaml(node[NF_CONFIG_HOST_NAME]);
  }
  if (node["sbi"]) {
    m_sbi.m_host = m_host;
    m_sbi.from_yaml(node["sbi"]);
  }
  if (node["n2"] and m_nx.is_set()) {
    m_nx.m_host = m_host;
    m_nx.from_yaml(node["n2"]);
  }
  if (node["n4"] and m_nx.is_set()) {
    m_nx.m_host = m_host;
    m_nx.from_yaml(node["n4"]);
  }
  m_set = true;
  set_url();
}

std::string nf::to_string(const std::string& indent) const {
  std::string out;
  if (!is_set()) {
    return "";
  }
  std::string inner_indent = add_indent(indent);
  unsigned int inner_width = get_inner_width(inner_indent.length());

  out.append(indent).append(m_config_name).append(":\n");
  out.append(inner_indent)
      .append(fmt::format(
          BASE_FORMATTER, OUTER_LIST_ELEM, m_host.get_config_name(),
          inner_width, m_host.get_value()));

  if (m_sbi.is_set()) {
    out.append(inner_indent)
        .append(
            fmt::format("{} {}\n", OUTER_LIST_ELEM, m_sbi.get_config_name()));
    out.append(m_sbi.to_string(add_indent(inner_indent)));
  }
  if (m_nx.is_set()) {
    out.append(inner_indent)
        .append(
            fmt::format("{} {}\n", OUTER_LIST_ELEM, m_nx.get_config_name()));
    out.append(m_nx.to_string(add_indent(inner_indent)));
  }

  return out;
}

void nf::validate() {
  if (!m_set) return;
  m_host.validate();
  m_sbi.validate();
  m_nx.validate();
}

const sbi_interface& nf::get_sbi() const {
  return m_sbi;
}

const local_interface& nf::get_nx() const {
  return m_nx;
}

const std::string& nf::get_host() const {
  return m_host.get_value();
}

const std::string& nf::get_url() const {
  return m_url;
}

void nf::set_url() {
  m_url = m_sbi.get_url();
}

nf_features_config::nf_features_config(
    const std::string& name, const std::string& nf_name, bool value) {
  m_option_value = option_config_value("", value);
  m_config_name  = name;
  m_nf_name      = nf_name;
  m_set          = true;
}

nf_features_config::nf_features_config(
    const std::string& name, const std::string& nf_name,
    const std::string& value) {
  m_string_value = string_config_value("", value);
  m_config_name  = name;
  m_nf_name      = nf_name;
  m_set          = true;
}

void nf_features_config::from_yaml(const YAML::Node& node) {
  if (node["general"]) {
    set_value(node["general"]);
  }

  // first we handle the NF-specific configuration
  if (node[m_nf_name]) {
    set_value(node[m_nf_name]);
  }
  m_set = true;
}

void nf_features_config::set_value(const YAML::Node& node) {
  try {
    m_option_value.from_yaml(node);
  } catch (YAML::Exception&) {
    m_string_value.from_yaml(node);
  }
}

std::string nf_features_config::to_string(const std::string& indent) const {
  if (!m_set) return "";
  std::string out;
  unsigned int inner_width = get_inner_width(indent.length());
  if (m_string_value.get_value().empty()) {
    out.append(indent).append(fmt::format(
        BASE_FORMATTER, OUTER_LIST_ELEM, m_config_name, inner_width,
        m_option_value.to_string("")));
  } else {
    out.append(indent).append(fmt::format(
        BASE_FORMATTER, OUTER_LIST_ELEM, m_config_name, inner_width,
        m_string_value.to_string("")));
  }
  return out;
}

void nf_features_config::validate() {
  if (!m_set) return;
  m_string_value.validate();
}

void nf_features_config::set_validation_regex(const std::string& regex) {
  m_string_value.set_validation_regex(regex);
}

bool nf_features_config::get_option() const {
  return m_option_value.get_value();
}

const std::string& nf_features_config::get_string() const {
  return m_string_value.get_value();
}

database_config::database_config() {
  m_set = false;
}

void database_config::from_yaml(const YAML::Node& node) {
  m_set = true;
  // TODO:
  if (node[NF_CONFIG_HOST_NAME]) {
    m_host.from_yaml(node[NF_CONFIG_HOST_NAME]);
  }
  if (node[DATABASE_CONFIG_USER]) {
    m_user.from_yaml(node[DATABASE_CONFIG_USER]);
  }
  if (node[DATABASE_CONFIG_PASSWORD]) {
    m_pass.from_yaml(node[DATABASE_CONFIG_PASSWORD]);
  }
  if (node[DATABASE_CONFIG_DATABASE_NAME]) {
    m_database_name.from_yaml(node[DATABASE_CONFIG_DATABASE_NAME]);
  }
  if (node[DATABASE_CONFIG_RANDOM]) {
    m_random.from_yaml(node[DATABASE_CONFIG_RANDOM]);
  }
  if (node[DATABASE_CONFIG_CONNECTION_TIMEOUT]) {
    m_connection_timeout.from_yaml(node[DATABASE_CONFIG_CONNECTION_TIMEOUT]);
  }
}

std::string database_config::to_string(const std::string& indent) const {
  std::string out;
  unsigned int inner_width = get_inner_width(indent.length());
  out.append(indent).append(fmt::format(
      BASE_FORMATTER, OUTER_LIST_ELEM, NF_CONFIG_HOST_NAME_LABEL, inner_width,
      m_host.get_value()));
  out.append(indent).append(fmt::format(
      BASE_FORMATTER, OUTER_LIST_ELEM, DATABASE_CONFIG_USER_LABEL, inner_width,
      m_user.get_value()));
  out.append(indent).append(fmt::format(
      BASE_FORMATTER, OUTER_LIST_ELEM, DATABASE_CONFIG_PASSWORD_LABEL,
      inner_width, m_pass.get_value()));
  out.append(indent).append(fmt::format(
      BASE_FORMATTER, OUTER_LIST_ELEM, DATABASE_CONFIG_DATABASE_NAME_LABEL,
      inner_width, m_database_name.get_value()));
  std::string database_config_random_string =
      m_random.get_value() ? "Yes" : "No";
  out.append(indent).append(fmt::format(
      BASE_FORMATTER, OUTER_LIST_ELEM, DATABASE_CONFIG_RANDOM_LABEL,
      inner_width, database_config_random_string));
  out.append(indent).append(fmt::format(
      BASE_FORMATTER, OUTER_LIST_ELEM, DATABASE_CONFIG_CONNECTION_TIMEOUT_LABEL,
      inner_width, m_connection_timeout.get_value()));
  return out;
}

const std::string& database_config::get_host() const {
  return m_host.get_value();
}

const std::string& database_config::get_user() const {
  return m_user.get_value();
}

const std::string& database_config::get_pass() const {
  return m_pass.get_value();
}

const std::string& database_config::get_database_name() const {
  return m_database_name.get_value();
}

const std::string& database_config::get_database_type() const {
  return m_database_type.get_value();
}

bool database_config::get_random() const {
  return m_random.get_value();
}

int database_config::get_connection_timeout() const {
  return m_connection_timeout.get_value();
}

dnn_config::dnn_config(
    const std::string& dnn, const std::string& pdu_type,
    const std::string& ipv4_pool, const std::string& ipv6_prefix) {
  m_config_name      = "DNN";
  m_dnn              = string_config_value("DNN", dnn);
  m_pdu_session_type = string_config_value("PDU session type", pdu_type);
  m_ipv4_pool        = string_config_value("IPv4 pool", ipv4_pool);
  m_ipv6_prefix      = string_config_value("IPv6 prefix", ipv6_prefix);

  m_pdu_session_type.set_validation_regex(PDU_SESSION_TYPE_REGEX);
  m_ipv4_pool.set_validation_regex(
      IPV4_ADDRESS_VALIDATOR_REGEX + "( )*-( )*" +
      IPV4_ADDRESS_VALIDATOR_REGEX);
  m_ipv6_prefix.set_validation_regex(IPV6_ADDRESS_VALIDATOR_REGEX);
  if (ipv6_prefix.empty()) {
    m_ipv6_prefix.unset_config();
  }
  m_set = true;
}

void dnn_config::from_yaml(const YAML::Node& node) {
  if (node["dnn"]) {
    m_dnn.from_yaml(node["dnn"]);
  }
  if (node["pdu_session_type"]) {
    m_pdu_session_type.from_yaml(node["pdu_session_type"]);
  }
  if (node["ipv4_pool"]) {
    m_ipv4_pool.from_yaml(node["ipv4_pool"]);
  }
  if (node["ipv6_prefix"]) {
    m_ipv6_prefix.from_yaml(node["ipv6_prefix"]);
  }
}

[[nodiscard]] std::string dnn_config::to_string(
    const std::string& indent) const {
  std::string out;

  std::string inner_indent = add_indent(indent);
  unsigned int inner_width = get_inner_width(inner_indent.length());

  out.append(fmt::format("{} {}:\n", OUTER_LIST_ELEM, m_dnn.get_config_name()));

  out.append(inner_indent)
      .append(fmt::format(
          BASE_FORMATTER, INNER_LIST_ELEM, m_dnn.get_config_name(), inner_width,
          m_dnn.to_string("")));

  out.append(inner_indent)
      .append(fmt::format(
          BASE_FORMATTER, INNER_LIST_ELEM, m_pdu_session_type.get_config_name(),
          inner_width, m_pdu_session_type.to_string("")));
  if (m_ipv6_prefix.get_value().empty()) {
    out.append(inner_indent)
        .append(fmt::format(
            BASE_FORMATTER, INNER_LIST_ELEM, m_ipv6_prefix.get_config_name(),
            inner_width, m_ipv6_prefix.to_string("")));
  } else {
    out.append(inner_indent)
        .append(fmt::format(
            BASE_FORMATTER, INNER_LIST_ELEM, m_ipv4_pool.get_config_name(),
            inner_width, m_ipv4_pool.to_string("")));
  }
  return out;
}

void dnn_config::validate() {
  m_pdu_session_type_generated =
      pdu_session_type_t(m_pdu_session_type.get_value());

  if (m_pdu_session_type_generated ==
          pdu_session_type_e::PDU_SESSION_TYPE_E_IPV4 ||
      m_pdu_session_type_generated ==
          pdu_session_type_e::PDU_SESSION_TYPE_E_IPV4V6) {
    std::vector<std::string> ips;
    boost::split(
        ips, m_ipv4_pool.get_value(), boost::is_any_of("-"),
        boost::token_compress_on);

    if (ips.size() != 2) {
      throw std::runtime_error(fmt::format(
          "The IP address pool {} is not valid", m_ipv4_pool.get_value()));
    }

    boost::trim_left(ips[0]);
    boost::trim_right(ips[0]);
    boost::trim_left(ips[1]);
    boost::trim_right(ips[1]);

    m_ipv4_pool_start_ip = safe_convert_ip(ips[0]);
    m_ipv4_pool_end_ip   = safe_convert_ip(ips[1]);

    if (htonl(m_ipv4_pool_start_ip.s_addr) >=
        htonl(m_ipv4_pool_end_ip.s_addr)) {
      throw std::runtime_error(fmt::format(
          "The IPv4 range {} is not valid. The start range must be below the "
          "end "
          "range",
          m_ipv4_pool.get_value()));
    }
    if (m_pdu_session_type_generated ==
            pdu_session_type_e::PDU_SESSION_TYPE_E_IPV6 ||
        m_pdu_session_type_generated ==
            pdu_session_type_e::PDU_SESSION_TYPE_E_IPV4V6) {
      std::vector<std::string> ip6s;
      boost::split(
          ip6s, m_ipv6_prefix.get_value(), boost::is_any_of("/"),
          boost::token_compress_on);

      if (ip6s.size() != 2) {
        throw std::runtime_error(fmt::format(
            "The IPv6 prefix / length {} is not valid",
            m_ipv6_prefix.get_value()));
      }

      m_ipv6_prefix_ip     = safe_convert_ip6(ip6s[0]);
      m_ipv6_prefix_length = std::stoi(ip6s[1]);
    }
  }
}

[[nodiscard]] const in_addr& dnn_config::get_ipv4_pool_start() const {
  return m_ipv4_pool_start_ip;
}

[[nodiscard]] const in_addr& dnn_config::get_ipv4_pool_end() const {
  return m_ipv4_pool_end_ip;
}

[[nodiscard]] const in6_addr& dnn_config::get_ipv6_prefix() const {
  return m_ipv6_prefix_ip;
}

[[nodiscard]] uint8_t dnn_config::get_ipv6_prefix_length() const {
  return m_ipv6_prefix_length;
}

[[nodiscard]] const pdu_session_type_t& dnn_config::get_pdu_session_type()
    const {
  return m_pdu_session_type_generated;
}

const std::string& dnn_config::get_dnn() const {
  return m_dnn.get_value();
}

nf_http_version::nf_http_version() {
  m_set     = false;
  m_version = string_config_value(NF_CONFIG_HTTP_NAME, "1.1");
  m_version.set_validation_regex("1|1.1|2|3");
}

void nf_http_version::from_yaml(const YAML::Node& node) {
  m_set = true;
  m_version.from_yaml(node);
}

std::string nf_http_version::to_string(const std::string& indent) const {
  std::string out;
  unsigned int inner_width = get_inner_width(indent.length());
  out.append(indent).append(fmt::format(
      BASE_FORMATTER, OUTER_LIST_ELEM, NF_CONFIG_HTTP_LABEL, inner_width,
      m_version.get_value()));
  return out;
}

void nf_http_version::validate() {
  if (!m_set) return;
  m_version.validate();
}

const std::string& nf_http_version::get_http_version() const {
  return m_version.get_value();
}
