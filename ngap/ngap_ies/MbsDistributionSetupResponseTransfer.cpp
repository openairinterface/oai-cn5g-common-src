/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "MbsDistributionSetupResponseTransfer.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
MbsDistributionSetupResponseTransfer::MbsDistributionSetupResponseTransfer() {
  m_Ie = static_cast<Ngap_MBS_DistributionSetupResponseTransfer_t*>(
      calloc(1, sizeof(Ngap_MBS_DistributionSetupResponseTransfer_t)));
}

//------------------------------------------------------------------------------
MbsDistributionSetupResponseTransfer::~MbsDistributionSetupResponseTransfer() {
  if (m_Ie) {
    ASN_STRUCT_FREE(asn_DEF_Ngap_MBS_DistributionSetupResponseTransfer, m_Ie);
    m_Ie = nullptr;
  }
}

//------------------------------------------------------------------------------
int MbsDistributionSetupResponseTransfer::encode(
    uint8_t* buf, int bufSize) const {
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_MBS_DistributionSetupResponseTransfer, nullptr, m_Ie, buf,
      bufSize);
  return (er.encoded > 0) ? er.encoded : -1;
}

//------------------------------------------------------------------------------
bool MbsDistributionSetupResponseTransfer::decode(
    const uint8_t* buf, int bufSize) {
  if (m_Ie) {
    ASN_STRUCT_FREE(asn_DEF_Ngap_MBS_DistributionSetupResponseTransfer, m_Ie);
    m_Ie = nullptr;
  }
  asn_dec_rval_t rc = asn_decode(
      nullptr, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_MBS_DistributionSetupResponseTransfer,
      reinterpret_cast<void**>(&m_Ie), buf, bufSize);
  return (rc.code == RC_OK);
}

}  // namespace oai::ngap
