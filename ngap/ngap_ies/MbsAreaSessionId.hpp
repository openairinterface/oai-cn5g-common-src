/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MBS_AREA_SESSION_ID_H_
#define _MBS_AREA_SESSION_ID_H_

#include <cstdint>

extern "C" {
#include "Ngap_MBS-AreaSessionID.h"
}

namespace oai::ngap {

class MbsAreaSessionId {
 public:
  MbsAreaSessionId()  = default;
  ~MbsAreaSessionId() = default;

  bool encode(Ngap_MBS_AreaSessionID_t& value) const;
  bool decode(const Ngap_MBS_AreaSessionID_t& value);

  long get() const { return m_Value; }
  void set(long v) { m_Value = v; }

 private:
  long m_Value{0};
};

}  // namespace oai::ngap
#endif
