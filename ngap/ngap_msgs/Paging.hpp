/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
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
#include "UeRadioCapabilityForPaging.hpp"
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

  // Paging Origin (Optional) — set when the pending data is associated with
  // non-3GPP access.
  void setPagingOrigin(e_Ngap_PagingOrigin origin);

  void setUeRadioCapabilityForPaging(
      const OCTET_STRING_t& ue_radio_capability_for_paging_of_nr,
      const OCTET_STRING_t& ue_radio_capability_for_paging_of_eutra);

 private:
  Ngap_Paging_t* m_PagingIes;

  UePagingIdentity m_UePagingIdentity;                // Mandatory
  std::optional<PagingDrx> m_pagingDRX;               // Optional
  TaiListForPaging m_TaiListForPaging;                // Mandatory
  std::optional<PagingPriority> m_pagingPriority;     // Optional
  std::optional<e_Ngap_PagingOrigin> m_pagingOrigin;  // Optional
  std::optional<UeRadioCapabilityForPaging>
      m_ueRadioCapabilityForPaging;  // Optional
  // TODO: Assistance Data for Paging
  // TODO: NB-IoT paging eDRX/DRX
  // TODO: Enhanced Coverage Restriction;
  // TODO: WUS Assistance Information
  // TODO: Paging eDRX Information
  // TODO: CE-mode-B Restricted
};

}  // namespace oai::ngap

#endif
