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

#ifndef _UE_STATUS_H_
#define _UE_STATUS_H_

#include "Type4NasIe.hpp"

constexpr uint8_t kUeStatusIeLength = 3;
constexpr uint8_t kUeStatusIeContentLength =
    kUeStatusIeLength - 2;  // Minimum length - 2 octets for IEI/Length
constexpr auto kUeStatusIeName = "UE Status";

namespace oai::nas {

class UeStatus : public Type4NasIe {
 public:
  UeStatus();
  UeStatus(bool n1, bool s1);
  ~UeStatus();

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = false) override;

  static std::string GetIeName() { return kUeStatusIeName; }

  void SetN1(bool value);
  bool GetN1() const;

  void SetS1(bool value);
  bool GetS1() const;

 private:
  bool n1_;
  bool s1_;
};
}  // namespace oai::nas

#endif
