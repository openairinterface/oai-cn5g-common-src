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

#ifndef _EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_H_
#define _EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_H_

#include "Type6NasIe.hpp"
#include "3gpp_24.008.h"

constexpr uint8_t kExtendedProtocolConfigurationOptionsMinimumLength = 4;
constexpr uint8_t kExtendedProtocolConfigurationOptionsContentMinimumLength =
    kExtendedProtocolConfigurationOptionsMinimumLength -
    3;  // Minimum length - 3 octets for IEI/Length
constexpr uint32_t kExtendedProtocolConfigurationOptionsMaximumLength = 65538;
constexpr auto kExtendedProtocolConfigurationOptionsIeName =
    "Extended Protocol Configuration Options";

namespace oai::nas {

class ExtendedProtocolConfigurationOptions : public Type6NasIe {
 public:
  ExtendedProtocolConfigurationOptions();
  virtual ~ExtendedProtocolConfigurationOptions() = default;

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = false) override;

  static std::string GetIeName() {
    return kExtendedProtocolConfigurationOptionsIeName;
  }

  void SetLength();

  void SetConfigurationProtocol(uint8_t configuration_protocol);
  void GetConfigurationProtocol(uint8_t& configuration_protocol) const;

  bool AddProtocolOrContainerId(const pco_protocol_or_container_id_t& id);
  void GetProtocolOrContainerIds(
      std::vector<pco_protocol_or_container_id_t>& ids) const;
  void SetProtocolOrContainerIds(
      const std::vector<pco_protocol_or_container_id_t>& ids);

  void Set(const protocol_configuration_options_t& conf_opt);
  void Get(protocol_configuration_options_t& conf_opt) const;
  protocol_configuration_options_t Get() const;

 private:
  uint8_t ext_;                     // 1 bit
  uint8_t spare_;                   // 4 bits
  uint8_t configuration_protocol_;  // 3 bits
  std::vector<pco_protocol_or_container_id_t> protocol_or_container_ids;
};

}  // namespace oai::nas

#endif
