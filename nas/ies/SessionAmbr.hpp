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

#ifndef _SESSION_AMBR_H_
#define _SESSION_AMBR_H_

#include "Type4NasIe.hpp"

constexpr uint8_t kSessionAmbrLength = 8;
constexpr uint8_t kSessionAmbrContentLength =
    kSessionAmbrLength - 2;  // length - 2 octets for IEI/Length
constexpr auto kSessionAmbrIeName = "Session-AMBR";

namespace oai::nas {

class SessionAmbr : public Type4NasIe {
 public:
  SessionAmbr();
  SessionAmbr(uint8_t iei);
  SessionAmbr(
      uint8_t unit_for_downlink, uint16_t session_ambr_for_downlink,
      uint8_t unit_for_uplink, uint16_t session_ambr_for_uplink);
  SessionAmbr(
      uint8_t iei, uint8_t unit_for_downlink,
      uint16_t session_ambr_for_downlink, uint8_t unit_for_uplink,
      uint16_t session_ambr_for_uplink);
  virtual ~SessionAmbr();

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = true) override;

  static std::string GetIeName() { return kSessionAmbrIeName; }

  void SetUnitForDownlink(uint8_t unit_for_downlink);
  uint8_t GetUnitForDownlink() const;

  void SetSessionAmbrForDownlink(uint16_t session_ambr_for_downlink);
  uint16_t GetSessionAmbrForDownlink() const;

  void SetUnitForUplink(uint8_t unit_for_uplink);
  uint8_t GetUnitForUplink() const;

  void SetSessionAmbrForUplink(uint16_t session_ambr_for_uplink);
  uint16_t GetSessionAmbrForUplink() const;

 private:
  uint8_t unit_for_downlink_;
  uint16_t session_ambr_for_downlink_;
  uint8_t unit_for_uplink_;
  uint16_t session_ambr_for_uplink_;
};

}  // namespace oai::nas

#endif
