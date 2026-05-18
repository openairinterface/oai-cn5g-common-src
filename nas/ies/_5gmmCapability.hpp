/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _5GMM_CAPABILITY_H_
#define _5GMM_CAPABILITY_H_

#include <vector>

#include "Type4NasIe.hpp"

constexpr uint8_t k5gmmCapabilityMinimumLength = 3;
constexpr uint8_t k5gmmCapabilityContentMinimumLength =
    k5gmmCapabilityMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t k5gmmCapabilityMaximumLength = 15;
constexpr auto k5gmmCapabilityIeName           = "5GMM Capability";

namespace oai::nas {

class _5gmmCapability : public Type4NasIe {
 public:
  _5gmmCapability();
  _5gmmCapability(uint8_t iei, uint8_t octet3);
  virtual ~_5gmmCapability();

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = true) override;

  static std::string GetIeName() { return k5gmmCapabilityIeName; }

  void SetOctet3(uint8_t iei, uint8_t octet3);
  uint8_t GetOctet3() const;
  // Rel 17.10 IE
  bool SupportsNssrg() const;               // octet 7 bit 1
  bool SupportsNsag() const;                // octet 7 bit 6
  bool SupportsUas() const;                 // octet 7 bit 7
  bool SupportsMpsIndicatorUpdate() const;  // octet 7 bit 8

  // Release 17 capability accessors (TS 24.501 table 9.11.3.1.1 octet 7)
  // Returns false (unsupported) if octet 7 was not present in the message.
  bool SupportsNssrg() const;               // octet 7 bit 1
  bool SupportsNsag() const;                // octet 7 bit 6
  bool SupportsUas() const;                 // octet 7 bit 7
  bool SupportsMpsIndicatorUpdate() const;  // octet 7 bit 8

 private:
  uint8_t octet3_;  // minimum length of 3 octets
  std::optional<uint8_t> octet4_;
  std::optional<uint8_t> octet5_;
  std::optional<uint8_t> octet6_;
  std::optional<uint8_t> octet7_;
  // Raw bytes for octets 8-15
  std::vector<uint8_t> extra_octets_;
};

}  // namespace oai::nas

#endif
