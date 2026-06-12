/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _NSSRG_INFORMATION_H_
#define _NSSRG_INFORMATION_H_

#include <vector>

#include "Type6NasIe.hpp"

// TS 24.501 clause 9.11.3.82 – NSSRG information
constexpr uint8_t kNssrgInformationMinimumContentLength  = 7;
constexpr uint16_t kNssrgInformationMaximumContentLength = 4099;
constexpr auto kNssrgInformationIeName                   = "NSSRG Information";

namespace oai::nas {

class NssrgInformation : public Type6NasIe {
 public:
  NssrgInformation();
  explicit NssrgInformation(uint8_t iei);
  ~NssrgInformation() override = default;

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = false) override;

  static std::string GetIeName() { return kNssrgInformationIeName; }

  void SetValue(const std::vector<uint8_t>& data);
  const std::vector<uint8_t>& GetValue() const;

 private:
  std::vector<uint8_t> value_;  // raw content bytes
};

}  // namespace oai::nas

#endif
