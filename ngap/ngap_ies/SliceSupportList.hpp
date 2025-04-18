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

#ifndef _SLICE_SUPPORT_LIST_H_
#define _SLICE_SUPPORT_LIST_H_

#include <vector>

#include "SNssai.hpp"

extern "C" {
#include "Ngap_SliceSupportList.h"
#include "Ngap_SliceSupportItem.h"
}

namespace oai::ngap {

class SliceSupportList {
 public:
  SliceSupportList();
  virtual ~SliceSupportList();

  void setSliceSupportItems(const std::vector<SNssai>& items);
  void getSliceSupportItems(std::vector<SNssai>& items) const;

  bool encode(Ngap_SliceSupportList_t& SliceSupportList) const;
  bool decode(const Ngap_SliceSupportList_t& SliceSupportList);

 private:
  std::vector<SNssai> m_SliceSupportItems;
};
}  // namespace oai::ngap

#endif
