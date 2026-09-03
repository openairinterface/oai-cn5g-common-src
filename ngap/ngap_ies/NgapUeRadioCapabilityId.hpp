/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _NGAP_UE_RADIO_CAPABILITY_ID_H_
#define _NGAP_UE_RADIO_CAPABILITY_ID_H_

#include <cstdint>
#include <vector>

extern "C" {
#include "Ngap_UERadioCapabilityID.h"
}

namespace oai::ngap {

/**
 * Wrapper for id-UERadioCapabilityID (9.3.1.113, Rel-16).
 * The IE is an OCTET STRING carrying an opaque capability identifier
 * assigned by the AMF and stored in the UE context.
 *
 * Named NgapUeRadioCapabilityId to avoid collision with
 * oai::nas::UeRadioCapabilityId.
 */
class NgapUeRadioCapabilityId {
 public:
  NgapUeRadioCapabilityId()  = default;
  ~NgapUeRadioCapabilityId() = default;

  bool encode(Ngap_UERadioCapabilityID_t& value) const;
  bool decode(const Ngap_UERadioCapabilityID_t& value);

  void set(const std::vector<uint8_t>& data);
  void get(std::vector<uint8_t>& data) const;

 private:
  std::vector<uint8_t> m_Data;
};

}  // namespace oai::ngap
#endif
