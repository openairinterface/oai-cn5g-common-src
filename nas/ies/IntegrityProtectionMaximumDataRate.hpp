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

#ifndef _INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_H
#define _INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_H

#include "Type3NasIe.hpp"

constexpr uint8_t kIntegrityProtectionMaximumDataRateMinimumLength = 2;
constexpr uint8_t kIntegrityProtectionMaximumDataRateMaximumLength = 3;

constexpr auto kIntegrityProtectionMaximumDataRateIeName =
    "Integrity Protection Maximum Data Rate";

namespace oai::nas {

class IntegrityProtectionMaximumDataRate : public Type3NasIe {
 public:
  IntegrityProtectionMaximumDataRate();
  IntegrityProtectionMaximumDataRate(uint8_t iei);
  IntegrityProtectionMaximumDataRate(
      uint8_t iei, uint8_t uplink, uint8_t downlink);
  virtual ~IntegrityProtectionMaximumDataRate();

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = false) override;

  static std::string GetIeName() {
    return kIntegrityProtectionMaximumDataRateIeName;
  }
  uint32_t GetIeLength() const override;

  void Set(uint8_t iei, uint8_t uplink, uint8_t downlink);

  void SetUplink(uint8_t uplink);
  uint8_t GetUplink() const;

  void SetDownlink(uint8_t downlink);
  uint8_t GetDownlink() const;

 private:
  uint8_t uplink_;
  uint8_t downlink_;
};

}  // namespace oai::nas

#endif
