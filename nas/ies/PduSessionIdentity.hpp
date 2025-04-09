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

#ifndef _PDU_SESSION_IDENTITY_H_
#define _PDU_SESSION_IDENTITY_H_

#include "Type3NasIe.hpp"

constexpr uint8_t kPduSessionIdentityLength = 1;
constexpr auto kPduSessionIdentityIeName    = "PDU Session Identity";

namespace oai::nas {

class PduSessionIdentity : public NasIe {
 public:
  PduSessionIdentity();
  PduSessionIdentity(uint8_t value);
  virtual ~PduSessionIdentity();

  bool Validate(int len) const override;

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = false) override;

  static std::string GetIeName() { return kPduSessionIdentityIeName; }
  uint32_t GetIeLength() const override;

  void Set(uint8_t value);
  uint8_t Get() const;

 private:
  uint8_t value_;
};
}  // namespace oai::nas

#endif
