/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _NR_PAGING_EDRX_INFORMATION_H_
#define _NR_PAGING_EDRX_INFORMATION_H_

extern "C" {
#include "Ngap_NR-PagingeDRXInformation.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-NRPagingeDRXInformation (9.3.1.227, Rel-17).
 */
class NrPagingEDrxInformation {
 public:
  NrPagingEDrxInformation()  = default;
  ~NrPagingEDrxInformation() = default;

  bool encode(Ngap_NR_PagingeDRXInformation_t& value) const;
  bool decode(const Ngap_NR_PagingeDRXInformation_t& value);

  const Ngap_NR_PagingeDRXInformation_t& get() const { return m_Value; }
  void set(const Ngap_NR_PagingeDRXInformation_t& v) { m_Value = v; }

 private:
  Ngap_NR_PagingeDRXInformation_t m_Value{};
};

}  // namespace oai::ngap
#endif
