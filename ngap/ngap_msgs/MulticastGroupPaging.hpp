/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MULTICAST_GROUP_PAGING_H_
#define _MULTICAST_GROUP_PAGING_H_

#include <optional>

#include "MbsServiceArea.hpp"
#include "MbsSessionId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_MulticastGroupPaging.h"
}

namespace oai::ngap {

class MulticastGroupPagingMsg : public NgapMessage {
 public:
  MulticastGroupPagingMsg();
  ~MulticastGroupPagingMsg() = default;

  void initialize();

  void setMbsSessionId(const MbsSessionId& v);
  bool getMbsSessionId(MbsSessionId& v) const;

  void setMbsServiceArea(const MbsServiceArea& v);
  bool getMbsServiceArea(MbsServiceArea& v) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_MulticastGroupPaging_t* m_MulticastGroupPagingIes;

  MbsSessionId m_MbsSessionId;                          // Mandatory
  std::optional<MbsServiceArea> m_MbsServiceArea;        // Optional
};

}  // namespace oai::ngap
#endif
