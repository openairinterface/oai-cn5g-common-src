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

#ifndef _PAGING_H_
#define _PAGING_H_

#include <optional>
#include <vector>
#include <cstdint>

#include "PagingDrx.hpp"
#include "NgapMessage.hpp"
#include "PagingPriority.hpp"
#include "TaiListforPaging.hpp"
#include "UePagingIdentity.hpp"

extern "C" {
#include "Ngap_NGAP-PDU.h"
#include "Ngap_Paging.h"
#include "Ngap_PagingDRX.h"
#include "Ngap_PagingOrigin.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace oai::ngap {

class PagingMsg : public NgapMessage {
 public:
  PagingMsg();
  virtual ~PagingMsg();

  void initialize();
  bool decode(Ngap_NGAP_PDU_t* ngap_msg_pdu) override;

  void setUePagingIdentity(
      const std::string& setId, const std::string& pointer,
      const std::string tmsi);
  void getUePagingIdentity(std::string& _5g_s_tmsi) const;
  void getUePagingIdentity(
      std::string& setId, std::string& pointer, std::string& tmsi) const;

  void setTaiListForPaging(const std::vector<Tai_t>& list);
  void getTaiListForPaging(std::vector<Tai_t>& list) const;

  // Paging DRX (Optional) — per-UE override of the gNB default
  void setPagingDrx(e_Ngap_PagingDRX drx);

  // Paging Priority (Optional) — mapped from Paging Policy Indicator
  void setPagingPriority(uint8_t ppi);

  // Paging Origin — not set for 3GPP paging (current scope)
  // void setPagingOrigin(e_Ngap_PagingOrigin origin); // TODO: non-3GPP

 private:
  Ngap_Paging_t* m_PagingIes;

  UePagingIdentity m_UePagingIdentity;             // Mandatory
  std::optional<PagingDrx> m_pagingDRX;            // Optional
  TaiListForPaging m_TaiListForPaging;             // Mandatory
  std::optional<PagingPriority> m_pagingPriority;  // Optional
  // TODO: UE Radio Capability for Paging (Optional)
  // TODO: PagingOrigin not set (3GPP-only paging, current scope)
  // TODO: Assistance Data for Paging (Optional)
  // TODO: NB-IoT Paging eDRX Information (Optional, Rel 16.14.0)
  // TODO: NB-IoT Paging DRX (Optional, Rel 16.14.0)
  // TODO: Enhanced Coverage Restriction (Optional, Rel 16.14.0)
  // TODO: WUS Assistance Information (Optional, Rel 16.14.0)
  // TODO: Paging eDRX Information (Optional, Rel 16.14.0)
  // TODO: CE-mode-B Restricted (Optional, Rel 16.14.0)
};

}  // namespace oai::ngap

#endif
