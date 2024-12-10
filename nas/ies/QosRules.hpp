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

#ifndef _QOS_RULES_H_
#define _QOS_RULES_H_

#include "QosRule.hpp"
#include "Type6NasIe.hpp"

constexpr uint8_t kQosRulesMinimumLength = 7;
constexpr uint8_t kQosRulesContentMinimumLength =
    kQosRulesMinimumLength - 3;  // Minimum length - 3 octets for IEI/Length
constexpr uint32_t kQosRulesMaximumLength = 65538;
constexpr auto kQosRulesIeName            = "QoS Rules";

namespace oai::nas {
using namespace oai::nas;
class QosRules : public Type6NasIe {
 public:
  QosRules();
  QosRules(uint8_t iei);
  QosRules(uint8_t iei, const std::vector<QosRule>& qos_rules);
  QosRules(const std::vector<QosRule>& qos_rules);
  virtual ~QosRules();

  int Encode(uint8_t* buf, int len) const;
  int Decode(const uint8_t* const buf, int len, bool is_iei);

  static std::string GetIeName() { return kQosRulesIeName; }

  void Set(const std::vector<QosRule>& rules);
  void Get(std::vector<QosRule>& rules) const;

  void AddQosRule(const QosRule& rule);

 private:
  std::vector<QosRule> qos_rules_;
};

}  // namespace oai::nas

#endif
