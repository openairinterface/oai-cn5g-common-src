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

#ifndef _ALWAYS_ON_PDU_SESSION_INDICATION_H_
#define _ALWAYS_ON_PDU_SESSION_INDICATION_H_

#include "Type1NasIeFormatTv.hpp"

namespace oai::nas {

constexpr auto kAlwaysOnPduSessionIndicationIeName =
    "Always-on PDU Session Indication";
constexpr uint8_t kAlwaysOnPduSessionNotAllowed = 0;
constexpr uint8_t kAlwaysOnPduSessionRequired   = 1;

class AlwaysOnPduSessionIndication : public Type1NasIeFormatTv {
 public:
  AlwaysOnPduSessionIndication();
  AlwaysOnPduSessionIndication(uint8_t iei);
  AlwaysOnPduSessionIndication(uint8_t iei, uint8_t type);
  virtual ~AlwaysOnPduSessionIndication();

  static std::string GetIeName() { return kAlwaysOnPduSessionIndicationIeName; }

  void SetValue();
  void GetValue();

  void Set(uint8_t iei, bool apsi);

  void SetApsi(bool apsi);
  bool IsApsi();

 private:
  bool apsi_;
};

}  // namespace oai::nas

#endif
