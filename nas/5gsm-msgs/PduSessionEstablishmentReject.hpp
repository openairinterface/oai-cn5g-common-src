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

#ifndef _PDU_SESSION_ESTABLISHMENT_REJECT_H_
#define _PDU_SESSION_ESTABLISHMENT_REJECT_H_

#include <bstrlib.h>

#include <vector>

#include "NasIeHeader.hpp"
#include "Nas5gsmHeader.hpp"

namespace oai::nas {

class PduSessionEstablishmentReject : public Nas5gsmHeader {
 public:
  PduSessionEstablishmentReject();
  ~PduSessionEstablishmentReject();

  int Encode(uint8_t* buf, int len) override;
  int Decode(uint8_t* buf, int len) override;

  uint32_t GetLength() const override;

 private:
  // Mandatory
  Nas5gsmHeader ie_header_;
  // 5GSM cause

  // Optional
  // Back-off timer value
  // Allowed SSC mode
  // EAP message
  // 5GSM congestion re-attempt indicator
  // Extended protocol configuration options
  // Re-attempt indicator
};

}  // namespace oai::nas

#endif
