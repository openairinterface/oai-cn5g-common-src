/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MbsDistributionReleaseRequestTransfer.hpp"

#include "ngap_utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
MbsDistributionReleaseRequestTransfer::MbsDistributionReleaseRequestTransfer() {
  m_Ie = static_cast<Ngap_MBS_DistributionReleaseRequestTransfer_t*>(
      calloc(1, sizeof(Ngap_MBS_DistributionReleaseRequestTransfer_t)));
}

//------------------------------------------------------------------------------
MbsDistributionReleaseRequestTransfer::
    ~MbsDistributionReleaseRequestTransfer() {
  if (m_Ie) {
    ASN_STRUCT_FREE(asn_DEF_Ngap_MBS_DistributionReleaseRequestTransfer, m_Ie);
    m_Ie = nullptr;
  }
}

//------------------------------------------------------------------------------
int MbsDistributionReleaseRequestTransfer::encode(
    uint8_t* buf, int bufSize) const {
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_MBS_DistributionReleaseRequestTransfer, nullptr, m_Ie, buf,
      bufSize);
  return ngap_utils::aper_encoded_bits_to_bytes(er.encoded);
}

//------------------------------------------------------------------------------
bool MbsDistributionReleaseRequestTransfer::decode(
    const uint8_t* buf, int bufSize) {
  if (m_Ie) {
    ASN_STRUCT_FREE(asn_DEF_Ngap_MBS_DistributionReleaseRequestTransfer, m_Ie);
    m_Ie = nullptr;
  }
  asn_dec_rval_t rc = asn_decode(
      nullptr, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_MBS_DistributionReleaseRequestTransfer,
      reinterpret_cast<void**>(&m_Ie), buf, bufSize);
  return (rc.code == RC_OK);
}

}  // namespace oai::ngap
