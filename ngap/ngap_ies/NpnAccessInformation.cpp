/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "NpnAccessInformation.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool NpnAccessInformation::encode(Ngap_NPN_AccessInformation_t& value) const {
  value.present = Ngap_NPN_AccessInformation_PR_pNI_NPN_Access_Information;
  value.choice.pNI_NPN_Access_Information =
      (Ngap_CellCAGList*) calloc(1, sizeof(Ngap_CellCAGList));
  if (!value.choice.pNI_NPN_Access_Information) return false;
  return m_CagIdList.encode(*value.choice.pNI_NPN_Access_Information);
}

//------------------------------------------------------------------------------
bool NpnAccessInformation::decode(const Ngap_NPN_AccessInformation_t& value) {
  if (!value.choice.pNI_NPN_Access_Information) return false;
  return m_CagIdList.decode(*value.choice.pNI_NPN_Access_Information);
}

}  // namespace oai::ngap
