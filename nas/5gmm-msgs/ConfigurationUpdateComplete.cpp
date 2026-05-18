/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

// TS 24.501 §8.2.20 — Configuration Update Complete
// The UE sends this message to acknowledge a Configuration Update Command
// that had the ACK bit set.  The message body contains only the NAS header.

#include "ConfigurationUpdateComplete.hpp"

#include "NasHelper.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
ConfigurationUpdateComplete::ConfigurationUpdateComplete()
    : ie_header_(
          k5gsMobilityManagementMessages, kPlain5gsMessage,
          kConfigurationUpdateComplete) {}

//------------------------------------------------------------------------------
ConfigurationUpdateComplete::~ConfigurationUpdateComplete() {}

//------------------------------------------------------------------------------
uint32_t ConfigurationUpdateComplete::GetLength() const {
  return ie_header_.GetLength();
}

//------------------------------------------------------------------------------
void ConfigurationUpdateComplete::SetHeader(uint8_t security_header_type) {
  ie_header_.SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
int ConfigurationUpdateComplete::Encode(uint8_t* buf, int len) {
  oai::logger::logger_common::nas().debug(
      "Encoding ConfigurationUpdateComplete message");

  int encoded_size    = 0;
  int encoded_ie_size = 0;

  if ((encoded_ie_size = ie_header_.Encode(buf, len)) == KEncodeDecodeError) {
    oai::logger::logger_common::nas().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  oai::logger::logger_common::nas().debug(
      "Encoded ConfigurationUpdateComplete message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int ConfigurationUpdateComplete::Decode(uint8_t* buf, int len) {
  oai::logger::logger_common::nas().debug(
      "Decoding ConfigurationUpdateComplete message");

  int decoded_size    = 0;
  int decoded_ie_size = 0;

  // Header
  decoded_ie_size = ie_header_.Decode(buf, len);
  if (decoded_ie_size == KEncodeDecodeError) {
    oai::logger::logger_common::nas().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_ie_size;

  // No optional IEs defined for this message (Table 8.2.20.1.1).
  oai::logger::logger_common::nas().debug(
      "Decoded ConfigurationUpdateComplete message (len %d)", decoded_size);
  return decoded_size;
}
