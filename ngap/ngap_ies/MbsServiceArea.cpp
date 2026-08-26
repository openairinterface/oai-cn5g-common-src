/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MbsServiceArea.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool MbsServiceArea::encode(Ngap_MBS_ServiceArea_t& ie) const {
  ie.present = m_Present;
  return true;
}

//------------------------------------------------------------------------------
bool MbsServiceArea::decode(const Ngap_MBS_ServiceArea_t& ie) {
  m_Present = ie.present;
  return (m_Present != Ngap_MBS_ServiceArea_PR_NOTHING);
}

}  // namespace oai::ngap
