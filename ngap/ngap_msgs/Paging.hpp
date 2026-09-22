/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _PAGING_H_
#define _PAGING_H_

#include <optional>
#include <vector>
#include <cstdint>

#include "NgapMessage.hpp"
#include "NrPagingEDrxInformation.hpp"
#include "PagingCause.hpp"
#include "PagingDrx.hpp"
#include "PagingPriority.hpp"
#include "PeipsAssistanceInformation.hpp"
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

  void setNrPagingEDrxInformation(const NrPagingEDrxInformation& value);
  void setPagingCause(const PagingCause& value);
  void setPeipsAssistanceInformation(const PeipsAssistanceInformation& value);

  // Paging Origin — not set for 3GPP paging (current scope)
  // void setPagingOrigin(e_Ngap_PagingOrigin origin); // TODO: non-3GPP

 private:
  Ngap_Paging_t* m_PagingIes;

  UePagingIdentity m_UePagingIdentity;             // Mandatory
  std::optional<PagingDrx> m_pagingDRX;            // Optional
  TaiListForPaging m_TaiListForPaging;             // Mandatory
  std::optional<PagingPriority> m_pagingPriority;  // Optional
  std::optional<NrPagingEDrxInformation> m_NrPagingEDrxInformation;  // Rel-17
  std::optional<PagingCause> m_PagingCause;                          // Rel-17
  std::optional<PeipsAssistanceInformation>
      m_PeipsAssistanceInformation;  // Rel-17
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
