/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _RED_CAP_INDICATION_H_
#define _RED_CAP_INDICATION_H_

extern "C" {
#include "Ngap_RedCapIndication.h"
}

namespace oai::ngap {

class RedCapIndication {
 public:
  RedCapIndication()  = default;
  ~RedCapIndication() = default;

  bool encode(Ngap_RedCapIndication_t& value) const;
  bool decode(const Ngap_RedCapIndication_t& value);

  Ngap_RedCapIndication_t get() const { return m_Value; }
  void set(Ngap_RedCapIndication_t v) { m_Value = v; }

 private:
  Ngap_RedCapIndication_t m_Value{};
};

}  // namespace oai::ngap
#endif
