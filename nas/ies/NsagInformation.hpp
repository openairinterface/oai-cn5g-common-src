/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _NSAG_INFORMATION_H_
#define _NSAG_INFORMATION_H_

#include <vector>

#include "Type6NasIe.hpp"

// TS 24.501 – NSAG information
// Format: Type 6 TLV-E
//   IEI 0x7C in Registration Accept (table 8.2.7.1.1)
//   IEI 0x73 in Configuration Update Command (table 8.2.19.1.1)
// Minimum content length: 9 octets; maximum: 3143 octets.
// Maximum 32 NSAG entries; maximum 4 entries with TAI list.
constexpr uint8_t kNsagInformationMinimumContentLength  = 9;
constexpr uint16_t kNsagInformationMaximumContentLength = 3143;
constexpr uint8_t kNsagInformationMaxEntries            = 32;
constexpr auto kNsagInformationIeName                   = "NSAG Information";

namespace oai::nas {

class NsagInformation : public Type6NasIe {
 public:
  NsagInformation();
  explicit NsagInformation(uint8_t iei);
  ~NsagInformation() override = default;

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = false) override;

  static std::string GetIeName() { return kNsagInformationIeName; }

  void SetValue(const std::vector<uint8_t>& data);
  const std::vector<uint8_t>& GetValue() const;

 private:
  std::vector<uint8_t> value_;  // raw content bytes (opaque container)
};

}  // namespace oai::nas

#endif
