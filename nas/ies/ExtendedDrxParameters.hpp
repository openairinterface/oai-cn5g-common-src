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

#ifndef _EXTENDED_DRX_PARAMETERS_H_
#define _EXTENDED_DRX_PARAMETERS_H_

#include "Type4NasIe.hpp"

constexpr uint8_t kExtendedDrxParametersLength = 3;
constexpr uint8_t kExtendedDrxParametersContentLength =
    kExtendedDrxParametersLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr auto kExtendedDrxParametersIeName = "Extended DRX Parameters";

namespace oai::nas {

class ExtendedDrxParameters : public Type4NasIe {
 public:
  ExtendedDrxParameters();
  ExtendedDrxParameters(uint8_t paging_time, uint8_t value);
  ~ExtendedDrxParameters();

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = false) override;

  static std::string GetIeName() { return kExtendedDrxParametersIeName; }

  void SetValue(uint8_t value);
  uint8_t GetValue() const;

  void SetPagingTime(uint8_t value);
  uint8_t GetPagingTime() const;

 private:
  uint8_t paging_time_;
  uint8_t e_drx_value_;
};
}  // namespace oai::nas

#endif
