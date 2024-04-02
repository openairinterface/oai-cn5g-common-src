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

#ifndef _ADDITIONAL_5G_SECURITY_INFORMATION_H_
#define _ADDITIONAL_5G_SECURITY_INFORMATION_H_

#include "Type4NasIe.hpp"

constexpr uint8_t kAdditional5gSecurityInformationLength = 3;
constexpr uint8_t kAdditional5gSecurityInformationContentLength =
    kAdditional5gSecurityInformationLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr auto kAdditional5gSecurityInformationIeName =
    "Additional 5G Security Information";

namespace oai::nas {

class Additional5gSecurityInformation : public Type4NasIe {
 public:
  Additional5gSecurityInformation();
  Additional5gSecurityInformation(bool RINMR, bool HDP);
  ~Additional5gSecurityInformation();

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = false) override;

  static std::string GetIeName() {
    return kAdditional5gSecurityInformationIeName;
  }

  void SetRinmr(bool value);
  bool GetRinmr() const;

  void SetHdp(bool value);
  bool GetHdp() const;

 private:
  bool rinmr_;
  bool hdp_;
};
}  // namespace oai::nas

#endif
