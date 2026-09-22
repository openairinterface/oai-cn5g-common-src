/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _NPN_ACCESS_INFORMATION_H_
#define _NPN_ACCESS_INFORMATION_H_

#include "CellCagList.hpp"

extern "C" {
#include "Ngap_NPN-AccessInformation.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-NPNAccessInformation (9.3.3.46, Rel-17).
 * Opaque container: encode/decode pass the raw ASN.1 struct through.
 */
class NpnAccessInformation {
 public:
  NpnAccessInformation()  = default;
  ~NpnAccessInformation() = default;

  bool encode(Ngap_NPN_AccessInformation_t& value) const;
  bool decode(const Ngap_NPN_AccessInformation_t& value);

  const CellCagList& getCellCagList();

 private:
  CellCagList m_CagIdList{};
};

}  // namespace oai::ngap
#endif
