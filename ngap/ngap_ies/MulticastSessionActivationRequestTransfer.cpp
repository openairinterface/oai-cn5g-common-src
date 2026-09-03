/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MulticastSessionActivationRequestTransfer.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
MulticastSessionActivationRequestTransfer::
    MulticastSessionActivationRequestTransfer() {
  m_Ie = static_cast<Ngap_MulticastSessionActivationRequestTransfer_t*>(
      calloc(1, sizeof(Ngap_MulticastSessionActivationRequestTransfer_t)));
}

//------------------------------------------------------------------------------
MulticastSessionActivationRequestTransfer::
    ~MulticastSessionActivationRequestTransfer() {
  if (m_Ie) {
    ASN_STRUCT_FREE(
        asn_DEF_Ngap_MulticastSessionActivationRequestTransfer, m_Ie);
    m_Ie = nullptr;
  }
}

//------------------------------------------------------------------------------
int MulticastSessionActivationRequestTransfer::encode(
    uint8_t* buf, int bufSize) const {
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_MulticastSessionActivationRequestTransfer, nullptr, m_Ie,
      buf, bufSize);
  return (er.encoded > 0) ? er.encoded : -1;
}

//------------------------------------------------------------------------------
bool MulticastSessionActivationRequestTransfer::decode(
    const uint8_t* buf, int bufSize) {
  if (m_Ie) {
    ASN_STRUCT_FREE(
        asn_DEF_Ngap_MulticastSessionActivationRequestTransfer, m_Ie);
    m_Ie = nullptr;
  }
  asn_dec_rval_t rc = asn_decode(
      nullptr, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_MulticastSessionActivationRequestTransfer,
      reinterpret_cast<void**>(&m_Ie), buf, bufSize);
  return (rc.code == RC_OK);
}

}  // namespace oai::ngap
