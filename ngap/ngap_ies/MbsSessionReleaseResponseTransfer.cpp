/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MbsSessionReleaseResponseTransfer.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
MbsSessionReleaseResponseTransfer::MbsSessionReleaseResponseTransfer() {
  m_Ie = static_cast<Ngap_MBSSessionReleaseResponseTransfer_t*>(
      calloc(1, sizeof(Ngap_MBSSessionReleaseResponseTransfer_t)));
}

//------------------------------------------------------------------------------
MbsSessionReleaseResponseTransfer::~MbsSessionReleaseResponseTransfer() {
  if (m_Ie) {
    ASN_STRUCT_FREE(asn_DEF_Ngap_MBSSessionReleaseResponseTransfer, m_Ie);
    m_Ie = nullptr;
  }
}

//------------------------------------------------------------------------------
int MbsSessionReleaseResponseTransfer::encode(uint8_t* buf, int bufSize) const {
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_MBSSessionReleaseResponseTransfer, nullptr, m_Ie, buf,
      bufSize);
  return (er.encoded > 0) ? er.encoded : -1;
}

//------------------------------------------------------------------------------
bool MbsSessionReleaseResponseTransfer::decode(
    const uint8_t* buf, int bufSize) {
  if (m_Ie) {
    ASN_STRUCT_FREE(asn_DEF_Ngap_MBSSessionReleaseResponseTransfer, m_Ie);
    m_Ie = nullptr;
  }
  asn_dec_rval_t rc = asn_decode(
      nullptr, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_MBSSessionReleaseResponseTransfer,
      reinterpret_cast<void**>(&m_Ie), buf, bufSize);
  return (rc.code == RC_OK);
}

}  // namespace oai::ngap
