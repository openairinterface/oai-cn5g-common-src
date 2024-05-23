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

#include "NasKeySetIdentifier.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger_base.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
NasKeySetIdentifier::NasKeySetIdentifier()
    : Type1NasIe(), tsc_(false), key_id_() {}

//------------------------------------------------------------------------------
NasKeySetIdentifier::NasKeySetIdentifier(uint8_t iei, bool tsc, uint8_t key_id)
    : Type1NasIe(iei) {
  tsc_    = 0x01 & tsc;
  key_id_ = 0x07 & key_id;
  NasKeySetIdentifier::SetValue();
}

//------------------------------------------------------------------------------
NasKeySetIdentifier::NasKeySetIdentifier(bool tsc, uint8_t key_id)
    : Type1NasIe(false) {
  tsc_    = 0x01 & tsc;
  key_id_ = 0x07 & key_id;
  NasKeySetIdentifier::SetValue();
}

//------------------------------------------------------------------------------
NasKeySetIdentifier::~NasKeySetIdentifier(){};

//------------------------------------------------------------------------------
void NasKeySetIdentifier::Set(bool high_pos) {
  Type1NasIe::Set(high_pos);
}

//------------------------------------------------------------------------------
void NasKeySetIdentifier::SetValue() {
  if (tsc_)
    value_ = 0b1000 | (0x07 & key_id_);
  else
    value_ = 0x07 & key_id_;
}

//------------------------------------------------------------------------------
void NasKeySetIdentifier::GetValue() {
  tsc_    = (0b1000 & value_) >> 3;
  key_id_ = value_ & 0b00000111;
}

//------------------------------------------------------------------------------
void NasKeySetIdentifier::SetTypeOfSecurityContext(bool type) {
  tsc_ = type;
  NasKeySetIdentifier::SetValue();  // Update value
}

//------------------------------------------------------------------------------
void NasKeySetIdentifier::SetNasKeyIdentifier(uint8_t id) {
  key_id_ = 0x07 & id;
  NasKeySetIdentifier::SetValue();  // Update value
}

//------------------------------------------------------------------------------
bool NasKeySetIdentifier::GetTypeOfSecurityContext() const {
  return tsc_;
}

//------------------------------------------------------------------------------
uint8_t NasKeySetIdentifier::GetNasKeyIdentifier() const {
  return key_id_;
}
