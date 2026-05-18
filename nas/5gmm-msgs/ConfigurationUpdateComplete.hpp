/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef CONFIGURATION_UPDATE_COMPLETE_H_
#define CONFIGURATION_UPDATE_COMPLETE_H_

// TS 24.501 §8.2.20 — Configuration Update Complete
// The message body contains only the NAS header; there are no optional IEs
// defined in Table 8.2.20.1.1.

#include "NasIeHeader.hpp"
#include "NasMmPlainHeader.hpp"

namespace oai::nas {
using namespace oai::nas;

class ConfigurationUpdateComplete : public Nas5gmmMessage {
 public:
  ConfigurationUpdateComplete();
  ~ConfigurationUpdateComplete();

  int Encode(uint8_t* buf, int len) override;
  int Decode(uint8_t* buf, int len) override;

  uint32_t GetLength() const override;

  void SetHeader(uint8_t security_header_type);

 private:
  NasMmPlainHeader ie_header_;  // Mandatory
};

}  // namespace oai::nas

#endif
