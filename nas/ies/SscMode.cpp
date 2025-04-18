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

#include "SscMode.hpp"

#include "logger_base.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
SscMode::SscMode() : Type1NasIe() {}

//------------------------------------------------------------------------------
SscMode::SscMode(uint8_t iei) : Type1NasIe(iei) {}

//------------------------------------------------------------------------------
SscMode::SscMode(uint8_t iei, uint8_t value) : Type1NasIe(iei) {
  Type1NasIe::SetValue(value & 0x07);
}

//------------------------------------------------------------------------------
SscMode::~SscMode() {}

//------------------------------------------------------------------------------
void SscMode::Set(bool high_pos) {
  Type1NasIe::Set(high_pos);
}

//------------------------------------------------------------------------------
void SscMode::SetSscMode(uint8_t value) {
  ssc_mode_ = value & 0x07;
  Type1NasIe::SetValue(ssc_mode_);
}

//------------------------------------------------------------------------------
uint8_t SscMode::GetSscMode() {
  GetValue();
  return ssc_mode_;
}

//------------------------------------------------------------------------------
void SscMode::SetValue() {
  Type1NasIe::SetValue(ssc_mode_);
}

//------------------------------------------------------------------------------
void SscMode::GetValue() {
  ssc_mode_ = value_ & 0x07;  // 3 bits
}
