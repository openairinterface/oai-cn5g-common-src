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

#ifndef _SSC_MODE_H_
#define _SSC_MODE_H_

#include "Type1NasIe.hpp"

constexpr auto kSscModeName = "SSC Mode";

namespace oai::nas {

class SscMode : public Type1NasIe {
 public:
  SscMode();
  SscMode(uint8_t type);
  SscMode(uint8_t iei, uint8_t type);
  virtual ~SscMode();

  static std::string GetIeName() { return kSscModeName; }

  void Set(bool high_pos);

  void SetSscMode(uint8_t value);
  uint8_t GetSscMode();

 private:
  void SetValue() override;
  void GetValue() override;
  uint8_t ssc_mode_;  // 3 bits
};

}  // namespace oai::nas

#endif
