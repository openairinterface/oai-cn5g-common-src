/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "NotifySourceNgRanNode.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
bool NotifySourceNgRanNode::encode(Ngap_NotifySourceNGRANNode_t& value) const {
  value = m_Value;
  return true;
}

//------------------------------------------------------------------------------
bool NotifySourceNgRanNode::decode(const Ngap_NotifySourceNGRANNode_t& value) {
  m_Value = value;
  return true;
}

}  // namespace oai::ngap
