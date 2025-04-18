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

#ifndef _UP_NGU_UP_TNL_MODIFY_LIST_H_
#define _UP_NGU_UP_TNL_MODIFY_LIST_H_

#include <vector>
#include "UlNgUUpTnlModifyItem.hpp"

extern "C" {
#include "Ngap_UL-NGU-UP-TNLModifyList.h"
}

namespace oai::ngap {

class UlNgUUpTnlModifyList {
 public:
  UlNgUUpTnlModifyList();
  virtual ~UlNgUUpTnlModifyList();

  void set(const std::vector<UlNgUUpTnlModifyItem>& list);
  void get(std::vector<UlNgUUpTnlModifyItem>& list) const;

  void addItem(const UlNgUUpTnlModifyItem& item);

  bool encode(Ngap_UL_NGU_UP_TNLModifyList_t& list) const;
  bool decode(const Ngap_UL_NGU_UP_TNLModifyList_t& list);

 private:
  std::vector<UlNgUUpTnlModifyItem> m_ItemList;
  constexpr static uint8_t KMaxNoOfMultiConnectivity = 4;
};

}  // namespace oai::ngap

#endif
