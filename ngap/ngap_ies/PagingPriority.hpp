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

#ifndef _PAGING_PRIORITY_H_
#define _PAGING_PRIORITY_H_

#include <cstdint>

extern "C" {
#include "Ngap_PagingPriority.h"
}

namespace oai::ngap {

class PagingPriority {
 public:
  PagingPriority();
  virtual ~PagingPriority();

  void set(e_Ngap_PagingPriority priority);
  // PPI 0 (highest, TS 23.501 §5.6.3) → priolevel1; PPI 7 → priolevel8
  void setFromPpi(uint8_t ppi);
  e_Ngap_PagingPriority get() const;

  bool encode(Ngap_PagingPriority_t&) const;
  bool decode(const Ngap_PagingPriority_t&);

 private:
  e_Ngap_PagingPriority m_PagingPriority;
};

}  // namespace oai::ngap

#endif
