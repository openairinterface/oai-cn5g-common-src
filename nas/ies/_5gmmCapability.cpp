/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "_5gmmCapability.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.hpp"
#include "logger_base.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
_5gmmCapability::_5gmmCapability(uint8_t iei, uint8_t octet3)
    : Type4NasIe(kIei5gmmCapability) {
  octet3_ = octet3;
  octet4_ = std::nullopt;
  octet5_ = std::nullopt;
  octet6_ = std::nullopt;
  octet7_ = std::nullopt;
  extra_octets_.clear();
  SetLengthIndicator(k5gmmCapabilityContentMinimumLength);
}

//------------------------------------------------------------------------------
_5gmmCapability::_5gmmCapability() : Type4NasIe(kIei5gmmCapability) {
  octet3_ = 0;
  octet4_ = std::nullopt;
  octet5_ = std::nullopt;
  octet6_ = std::nullopt;
  octet7_ = std::nullopt;
  extra_octets_.clear();
  SetLengthIndicator(k5gmmCapabilityContentMinimumLength);
}

//------------------------------------------------------------------------------
_5gmmCapability::~_5gmmCapability() {}

//------------------------------------------------------------------------------
void _5gmmCapability::SetOctet3(uint8_t iei, uint8_t octet3) {
  SetIei(iei);
  SetLengthIndicator(k5gmmCapabilityContentMinimumLength);
  octet3_ = octet3;
}

//------------------------------------------------------------------------------
uint8_t _5gmmCapability::GetOctet3() const {
  return octet3_;
}

//------------------------------------------------------------------------------
int _5gmmCapability::Encode(uint8_t* buf, int len) const {
  oai::logger::logger_common::nas().debug("Encoding %s", GetIeName().c_str());
  int ie_len = GetIeLength();

  int encoded_size = 0;
  // Validate the buffer's length and Encode IEI/Length
  int encoded_header_size = Type4NasIe::Encode(buf + encoded_size, len);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  // Octet 3
  ENCODE_U8(buf + encoded_size, octet3_, encoded_size);
  // TODO: Encode spare for the rest
  uint8_t spare = 0;
  int spare_len = ie_len - encoded_size;
  for (int i = 0; i < spare_len; i++) {
    ENCODE_U8(buf + encoded_size, spare, encoded_size);
  }

  oai::logger::logger_common::nas().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gmmCapability::Decode(const uint8_t* const buf, int len, bool is_iei) {
  if (len < k5gmmCapabilityMinimumLength) {
    oai::logger::logger_common::nas().error(
        "Buffer length is less than the minimum length of this IE (%d "
        "octet)",
        k5gmmCapabilityMinimumLength);
    return KEncodeDecodeError;
  }

  uint8_t decoded_size = 0;
  oai::logger::logger_common::nas().debug("Decoding %s", GetIeName().c_str());

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  uint8_t content_len = GetLengthIndicator();

  DECODE_U8(buf + decoded_size, octet3_, decoded_size);

  // Decode octets 4-7 when present
  if (content_len >= 2) {
    uint8_t v = 0;
    DECODE_U8(buf + decoded_size, v, decoded_size);
    octet4_ = v;
  }
  if (content_len >= 3) {
    uint8_t v = 0;
    DECODE_U8(buf + decoded_size, v, decoded_size);
    octet5_ = v;
  }
  if (content_len >= 4) {
    uint8_t v = 0;
    DECODE_U8(buf + decoded_size, v, decoded_size);
    octet6_ = v;
  }
  if (content_len >= 5) {
    uint8_t v = 0;
    DECODE_U8(buf + decoded_size, v, decoded_size);
    octet7_ = v;
  }
  // Preserve remaining octets (8–15) raw for diagnostics
  extra_octets_.clear();
  for (int i = 5; i < content_len; i++) {
    uint8_t v = 0;
    DECODE_U8(buf + decoded_size, v, decoded_size);
    extra_octets_.push_back(v);
  }

  oai::logger::logger_common::nas().debug(
      "Decoded %s, Octet3 value (0x%x)", GetIeName().c_str(), octet3_);
  if (octet7_.has_value()) {
    oai::logger::logger_common::nas().debug(
        "Decoded %s, Octet7 value (0x%x) NSSRG=%d NSAG=%d UAS=%d MPSIU=%d",
        GetIeName().c_str(), octet7_.value(), SupportsNssrg() ? 1 : 0,
        SupportsNsag() ? 1 : 0, SupportsUas() ? 1 : 0,
        SupportsMpsIndicatorUpdate() ? 1 : 0);
  }
  oai::logger::logger_common::nas().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}

//------------------------------------------------------------------------------
// TS 24.501 table 9.11.3.1.1: octet 7 bit 1 = NSSRG
bool _5gmmCapability::SupportsNssrg() const {
  if (!octet7_.has_value()) return false;
  return (octet7_.value() & 0x01) != 0;
}

//------------------------------------------------------------------------------
// TS 24.501 table 9.11.3.1.1: octet 7 bit 6 = NSAG
bool _5gmmCapability::SupportsNsag() const {
  if (!octet7_.has_value()) return false;
  return (octet7_.value() & 0x20) != 0;
}

//------------------------------------------------------------------------------
// TS 24.501 table 9.11.3.1.1: octet 7 bit 7 = UAS
bool _5gmmCapability::SupportsUas() const {
  if (!octet7_.has_value()) return false;
  return (octet7_.value() & 0x40) != 0;
}

//------------------------------------------------------------------------------
// TS 24.501 table 9.11.3.1.1: octet 7 bit 8 = MPSIU
bool _5gmmCapability::SupportsMpsIndicatorUpdate() const {
  if (!octet7_.has_value()) return false;
  return (octet7_.value() & 0x80) != 0;
}
