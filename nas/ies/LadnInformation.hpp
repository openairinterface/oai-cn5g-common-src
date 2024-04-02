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

#ifndef _LADN_INFORMATION_H_
#define _LADN_INFORMATION_H_

#include "Dnn.hpp"
#include "Ladn.hpp"
#include "Type6NasIe.hpp"
#include "_5gsTrackingAreaIdList.hpp"

constexpr uint8_t kLadnInformationMinimumLength = 3;
constexpr uint8_t kLadnInformationContentMinimumLength =
    kLadnInformationMinimumLength -
    3;  // Minimum length - 3 octets for IEI/Length
constexpr uint16_t kLadnInformationMaximumLength        = 1715;
constexpr uint8_t kLadnInformationMaximumSupportedLadns = 8;
constexpr auto kLadnInformationIeName                   = "LADN Information";

namespace oai::nas {

class LadnInformation : public Type6NasIe {
 public:
  LadnInformation();
  ~LadnInformation();

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = false) override;

  static std::string GetIeName() { return kLadnInformationIeName; }

  void Set(const std::vector<Ladn>& value);
  void Add(const Ladn& value);

 private:
  std::vector<Ladn> ladn_list_;
};

}  // namespace oai::nas

#endif
