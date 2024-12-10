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

#ifndef _GPRS_TIMER_H_
#define _GPRS_TIMER_H_

#include "Type3NasIe.hpp"

constexpr uint8_t kGprsTimerLength = 2;
constexpr uint8_t kGprsTimerContentLength =
    kGprsTimerLength - 1;  // Length - 1 octets for IEI
constexpr auto kGprsTimerIeName = "GPRS Timer";

namespace oai::nas {

class GprsTimer : public Type3NasIe {
 public:
  GprsTimer(uint8_t iei);
  GprsTimer(uint8_t iei, uint8_t value);
  virtual ~GprsTimer();

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = false) override;

  static std::string GetIeName() { return kGprsTimerIeName; }
  uint32_t GetIeLength() const override;

  void Set(uint8_t _iei, uint8_t value);

  void SetValue(uint8_t value);
  uint8_t GetValue() const;

 private:
  uint8_t value_;
};
}  // namespace oai::nas

#endif
