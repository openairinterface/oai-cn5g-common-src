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

#ifndef _ALLOWED_SSC_MODE_H_
#define _ALLOWED_SSC_MODE_H_

#include "Type1NasIeFormatTv.hpp"

constexpr auto kAllowedSscModeName = "Allowed SSC Mode";

namespace oai::nas {

class AllowedSscMode : public Type1NasIeFormatTv {
 public:
  AllowedSscMode();
  AllowedSscMode(uint8_t type);
  AllowedSscMode(uint8_t iei, uint8_t type);
  virtual ~AllowedSscMode();

  static std::string GetIeName() { return kAllowedSscModeName; }

  void SetValue(uint8_t value);
  uint8_t GetValue() const;

  bool IsSscMode1Allowed() const;
  bool IsSscMode2Allowed() const;
  bool IsSscMode3Allowed() const;
};

}  // namespace oai::nas

#endif
