/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MBS_SESSION_ID_H_
#define _MBS_SESSION_ID_H_

#include <cstdint>
#include <optional>
#include <vector>

extern "C" {
#include "Ngap_MBS-SessionID.h"
}

namespace oai::ngap {

class MbsSessionId {
 public:
  MbsSessionId()  = default;
  ~MbsSessionId() = default;

  // TMGI is a 6-byte OCTET_STRING (3-byte PLMN + 3-byte service ID)
  void setTmgi(const uint8_t* buf, size_t len);
  bool getTmgi(uint8_t*& buf, size_t& len) const;

  // NID is optional (44-bit BIT_STRING)
  void setNid(const uint8_t* buf, size_t len, uint8_t bitsUnused);
  bool getNid(uint8_t*& buf, size_t& len, uint8_t& bitsUnused) const;
  bool hasNid() const { return m_HasNid; }

  bool encode(Ngap_MBS_SessionID_t& ie) const;
  bool decode(const Ngap_MBS_SessionID_t& ie);

 private:
  std::vector<uint8_t> m_Tmgi;
  bool m_HasNid{false};
  std::vector<uint8_t> m_NidBuf;
  uint8_t m_NidBitsUnused{0};
};

}  // namespace oai::ngap
#endif
