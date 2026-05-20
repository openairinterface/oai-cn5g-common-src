/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MbsSessionSetupResponseTransfer.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
MbsSessionSetupResponseTransfer::MbsSessionSetupResponseTransfer() {
  m_Ie = static_cast<Ngap_MBSSessionSetupOrModResponseTransfer_t*>(
      calloc(1, sizeof(Ngap_MBSSessionSetupOrModResponseTransfer_t)));
}

//------------------------------------------------------------------------------
MbsSessionSetupResponseTransfer::~MbsSessionSetupResponseTransfer() {
  if (m_Ie) {
    ASN_STRUCT_FREE(asn_DEF_Ngap_MBSSessionSetupOrModResponseTransfer, m_Ie);
    m_Ie = nullptr;
  }
}

//------------------------------------------------------------------------------
int MbsSessionSetupResponseTransfer::encode(
    uint8_t* buf, int bufSize) const {
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_MBSSessionSetupOrModResponseTransfer, nullptr, m_Ie, buf,
      bufSize);
  return (er.encoded > 0) ? er.encoded : -1;
}

//------------------------------------------------------------------------------
bool MbsSessionSetupResponseTransfer::decode(
    const uint8_t* buf, int bufSize) {
  if (m_Ie) {
    ASN_STRUCT_FREE(asn_DEF_Ngap_MBSSessionSetupOrModResponseTransfer, m_Ie);
    m_Ie = nullptr;
  }
  asn_dec_rval_t rc = asn_decode(
      nullptr, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_MBSSessionSetupOrModResponseTransfer,
      reinterpret_cast<void**>(&m_Ie), buf, bufSize);
  return (rc.code == RC_OK);
}

}  // namespace oai::ngap
