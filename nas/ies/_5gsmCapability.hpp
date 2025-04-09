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

#ifndef _5GSM_CAPABILITY_H_
#define _5GSM_CAPABILITY_H_

#include "Type4NasIe.hpp"

constexpr uint8_t k5gsmCapabilityMinimumLength = 3;
constexpr uint8_t k5gsmCapabilityContentMinimumLength =
    k5gsmCapabilityMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t k5gsmCapabilityMaximumLength = 15;
constexpr auto k5gsmCapabilityIeName           = "5GSM Capability";

namespace oai::nas {

class _5gsmCapability : public Type4NasIe {
 public:
  _5gsmCapability();
  _5gsmCapability(uint8_t iei, uint8_t octet3);
  virtual ~_5gsmCapability();

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = true) override;

  static std::string GetIeName() { return k5gsmCapabilityIeName; }

  void SetOctet3(uint8_t iei, uint8_t octet3);
  uint8_t GetOctet3() const;

 private:
  uint8_t octet3_;  // minimum length of 3 octets
  // TODO: octets 4-15 (Spare)
};

}  // namespace oai::nas

#endif
