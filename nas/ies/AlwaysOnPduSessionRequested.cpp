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

#include "AlwaysOnPduSessionRequested.hpp"

#include "logger_base.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
AlwaysOnPduSessionRequested::AlwaysOnPduSessionRequested()
    : Type1NasIeFormatTv() {}

//------------------------------------------------------------------------------
AlwaysOnPduSessionRequested::AlwaysOnPduSessionRequested(uint8_t iei)
    : Type1NasIeFormatTv(iei) {}

//------------------------------------------------------------------------------
AlwaysOnPduSessionRequested::AlwaysOnPduSessionRequested(
    uint8_t iei, uint8_t value)
    : Type1NasIeFormatTv(iei) {
  Type1NasIeFormatTv::SetValue(value & 0x0f);
}

//------------------------------------------------------------------------------
AlwaysOnPduSessionRequested::~AlwaysOnPduSessionRequested() {}

//------------------------------------------------------------------------------
void AlwaysOnPduSessionRequested::SetValue() {
  if (apsr_) value_ = 0x01;
}

//------------------------------------------------------------------------------
void AlwaysOnPduSessionRequested::GetValue() {
  apsr_ = (0x01 & value_);
}

//------------------------------------------------------------------------------
void AlwaysOnPduSessionRequested::Set(uint8_t iei, bool apsr) {
  apsr_ = apsr;
  SetValue();
  SetIei(iei);
}

//------------------------------------------------------------------------------
void AlwaysOnPduSessionRequested::SetApsr(bool apsr) {
  apsr_ = apsr;
  SetValue();
}

//------------------------------------------------------------------------------
bool AlwaysOnPduSessionRequested::IsApsr() {
  GetValue();
  return apsr_;
}
