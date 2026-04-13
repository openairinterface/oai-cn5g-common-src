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

#include "PagingPriority.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
PagingPriority::PagingPriority() {
  m_PagingPriority = Ngap_PagingPriority_priolevel1;
}

//------------------------------------------------------------------------------
PagingPriority::~PagingPriority() {}

//------------------------------------------------------------------------------
void PagingPriority::set(e_Ngap_PagingPriority priority) {
  m_PagingPriority = priority;
}

//------------------------------------------------------------------------------
void PagingPriority::setFromPpi(uint8_t ppi) {
  // TS 23.501 §5.6.3: PPI 0 = highest priority → priolevel1 (enum 0)
  // PPI 7 = lowest priority → priolevel8 (enum 7); direct mapping
  if (ppi > 7) ppi = 7;
  m_PagingPriority = static_cast<e_Ngap_PagingPriority>(ppi);
}

//------------------------------------------------------------------------------
e_Ngap_PagingPriority PagingPriority::get() const {
  return m_PagingPriority;
}

//------------------------------------------------------------------------------
bool PagingPriority::encode(Ngap_PagingPriority_t& pagingPriority) const {
  pagingPriority = m_PagingPriority;
  return true;
}

//------------------------------------------------------------------------------
bool PagingPriority::decode(const Ngap_PagingPriority_t& pagingPriority) {
  m_PagingPriority = static_cast<e_Ngap_PagingPriority>(pagingPriority);
  return true;
}

}  // namespace oai::ngap
