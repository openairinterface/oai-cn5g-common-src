/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "RanStatusTransferTransparentContainer.hpp"

#include "logger_base.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
RanStatusTransferTransparentContainer::RanStatusTransferTransparentContainer() {
}

//------------------------------------------------------------------------------
RanStatusTransferTransparentContainer::
    ~RanStatusTransferTransparentContainer() {}

//------------------------------------------------------------------------------
void RanStatusTransferTransparentContainer::getDrbSubjectList(
    DrbSubjectToStatusTransferList& drblist) const {
  drblist = m_DrbList;
}

//------------------------------------------------------------------------------
void RanStatusTransferTransparentContainer::setDrbSubjectList(
    const DrbSubjectToStatusTransferList& drblist) {
  m_DrbList = drblist;
}

//------------------------------------------------------------------------------
bool RanStatusTransferTransparentContainer::encode(
    Ngap_RANStatusTransfer_TransparentContainer_t& ranStatusTransfer) const {
  if (!ranStatusTransfer.dRBsSubjectToStatusTransferList)
    ranStatusTransfer.dRBsSubjectToStatusTransferList =
        (Ngap_DRBsSubjectToStatusTransferList_t*) calloc(
            1, sizeof(Ngap_DRBsSubjectToStatusTransferList_t));
  if (!m_DrbList.encode(*ranStatusTransfer.dRBsSubjectToStatusTransferList)) {
    oai::logger::logger_common::ngap().error(
        "Encode RANStatusTransferTransparentContainer IE error!");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool RanStatusTransferTransparentContainer::decode(
    const Ngap_RANStatusTransfer_TransparentContainer_t& ranStatusTransfer) {
  if (!ranStatusTransfer.dRBsSubjectToStatusTransferList) return false;
  if (!m_DrbList.decode(*ranStatusTransfer.dRBsSubjectToStatusTransferList)) {
    oai::logger::logger_common::ngap().error(
        "Decode RANStatusTransferTransparentContainer IE error!");
    return false;
  }
  return true;
}
}  // namespace oai::ngap
