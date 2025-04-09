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

#ifndef _UP_NGU_UP_TNL_MODIFY_ITEM_H_
#define _UP_NGU_UP_TNL_MODIFY_ITEM_H_

#include "UpTransportLayerInformation.hpp"

extern "C" {
#include "Ngap_UPTransportLayerInformation.h"
#include "Ngap_UL-NGU-UP-TNLModifyItem.h"
}

namespace oai::ngap {

class UlNgUUpTnlModifyItem {
 public:
  UlNgUUpTnlModifyItem();
  virtual ~UlNgUUpTnlModifyItem();

  void set(
      const UpTransportLayerInformation& ulNgUUpTnlInformation,
      const UpTransportLayerInformation& dlNgUUpTnlInformation);
  void get(
      UpTransportLayerInformation& ulNgUUpTnlInformation,
      UpTransportLayerInformation& dlNgUUpTnlInformation) const;

  void setUlNgUUpTnlInformation(
      const UpTransportLayerInformation& ulNgUUpTnlInformation);
  void getUlNgUUpTnlInformation(
      UpTransportLayerInformation& ulNgUUpTnlInformation) const;

  void setDlNgUUpTnlInformation(
      const UpTransportLayerInformation& dlNgUUpTnlInformation);
  void getDlNgUUpTnlInformation(
      UpTransportLayerInformation& dlNgUUpTnlInformation) const;

  bool encode(Ngap_UL_NGU_UP_TNLModifyItem_t& ulNgUUpTnlModifyItem) const;
  bool decode(const Ngap_UL_NGU_UP_TNLModifyItem_t& ulNgUUpTnlModifyItem);

 private:
  // UL NG-U UP TNL Information (Mandatory)
  UpTransportLayerInformation m_UlNgUUpTnlInformation;
  // UL NG-U UP TNL Information (Mandatory)
  UpTransportLayerInformation m_DlNgUUpTnlInformation;
  // TODO: Redundant UL NG-U UP TNL Information (Optional)
  // TODO: Redundant DL NG-U UP TNL Information (Optional)
};

}  // namespace oai::ngap

#endif
