/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _MBS_SERVICE_AREA_H_
#define _MBS_SERVICE_AREA_H_

extern "C" {
#include "Ngap_MBS-ServiceArea.h"
}

namespace oai::ngap {

class MbsServiceArea {
 public:
  MbsServiceArea()  = default;
  ~MbsServiceArea() = default;

  bool encode(Ngap_MBS_ServiceArea_t& ie) const;
  bool decode(const Ngap_MBS_ServiceArea_t& ie);

  Ngap_MBS_ServiceArea_PR getPresent() const { return m_Present; }
  void setPresent(Ngap_MBS_ServiceArea_PR p) { m_Present = p; }

 private:
  // Opaque handle; real field deconstruction is Stage 8 territory.
  Ngap_MBS_ServiceArea_PR m_Present{Ngap_MBS_ServiceArea_PR_NOTHING};
};

}  // namespace oai::ngap
#endif
