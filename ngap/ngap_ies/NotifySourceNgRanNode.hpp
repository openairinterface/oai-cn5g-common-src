/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _NOTIFY_SOURCE_NG_RAN_NODE_H_
#define _NOTIFY_SOURCE_NG_RAN_NODE_H_

extern "C" {
#include "Ngap_NotifySourceNGRANNode.h"
}

namespace oai::ngap {

class NotifySourceNgRanNode {
 public:
  NotifySourceNgRanNode()  = default;
  ~NotifySourceNgRanNode() = default;

  bool encode(Ngap_NotifySourceNGRANNode_t& value) const;
  bool decode(const Ngap_NotifySourceNGRANNode_t& value);

  Ngap_NotifySourceNGRANNode_t get() const { return m_Value; }
  void set(Ngap_NotifySourceNGRANNode_t v) { m_Value = v; }

 private:
  Ngap_NotifySourceNGRANNode_t m_Value{};
};

}  // namespace oai::ngap
#endif
