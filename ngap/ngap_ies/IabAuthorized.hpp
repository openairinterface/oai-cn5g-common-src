/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _IAB_AUTHORIZED_H_
#define _IAB_AUTHORIZED_H_

extern "C" {
#include "Ngap_IAB-Authorized.h"
}

namespace oai::ngap {

class IabAuthorized {
 public:
  IabAuthorized()  = default;
  ~IabAuthorized() = default;

  bool encode(Ngap_IAB_Authorized_t& value) const;
  bool decode(const Ngap_IAB_Authorized_t& value);

  Ngap_IAB_Authorized_t get() const { return m_Value; }
  void set(Ngap_IAB_Authorized_t v) { m_Value = v; }

 private:
  Ngap_IAB_Authorized_t m_Value{};
};

}  // namespace oai::ngap
#endif
