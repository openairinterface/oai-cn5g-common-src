/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "UserLocationInformationNr.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
UserLocationInformationNr::UserLocationInformationNr() {}

//------------------------------------------------------------------------------
UserLocationInformationNr::~UserLocationInformationNr() {}

//------------------------------------------------------------------------------
void UserLocationInformationNr::set(const NrCgi& nrCgi, const Tai& tai) {
  m_NrCgi = nrCgi;
  m_Tai   = tai;
}

//------------------------------------------------------------------------------
bool UserLocationInformationNr::encode(
    Ngap_UserLocationInformationNR_t& userLocationInformation) const {
  if (!userLocationInformation.nR_CGI) userLocationInformation.nR_CGI = (Ngap_NR_CGI_t*) calloc(1, sizeof(Ngap_NR_CGI_t));
  if (!m_NrCgi.encode(*userLocationInformation.nR_CGI)) {
    return false;
  }
  if (!userLocationInformation.tAI) userLocationInformation.tAI = (Ngap_TAI_t*) calloc(1, sizeof(Ngap_TAI_t));
  if (!m_Tai.encode(*userLocationInformation.tAI)) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UserLocationInformationNr::decode(
    const Ngap_UserLocationInformationNR_t& userLocationInformation) {
  if (!userLocationInformation.nR_CGI) return false;
  if (!m_NrCgi.decode(*userLocationInformation.nR_CGI)) {
    return false;
  }

  if (!userLocationInformation.tAI) return false;
  if (!m_Tai.decode(*userLocationInformation.tAI)) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void UserLocationInformationNr::get(NrCgi& nrCgi, Tai& tai) const {
  nrCgi = m_NrCgi;
  tai   = m_Tai;
}
}  // namespace oai::ngap
