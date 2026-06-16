/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "NsagInformation.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.hpp"
#include "logger_base.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
NsagInformation::NsagInformation() : Type6NasIe() {
  SetLengthIndicator(kNsagInformationMinimumContentLength);
}

//------------------------------------------------------------------------------
NsagInformation::NsagInformation(uint8_t iei) : Type6NasIe(iei) {
  SetLengthIndicator(kNsagInformationMinimumContentLength);
}

//------------------------------------------------------------------------------
void NsagInformation::SetValue(const std::vector<uint8_t>& data) {
  value_               = data;
  uint16_t content_len = (static_cast<uint16_t>(data.size()) >
                          kNsagInformationMinimumContentLength) ?
                             static_cast<uint16_t>(data.size()) :
                             kNsagInformationMinimumContentLength;
  SetLengthIndicator(content_len);
}

//------------------------------------------------------------------------------
const std::vector<uint8_t>& NsagInformation::GetValue() const {
  return value_;
}

//------------------------------------------------------------------------------
int NsagInformation::Encode(uint8_t* buf, int len) const {
  oai::logger::logger_common::nas().debug("Encoding %s", GetIeName().c_str());

  int encoded_size = 0;
  int len_pos      = 0;
  int encoded_header_size =
      Type6NasIe::Encode(buf + encoded_size, len, len_pos);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  // Encode content bytes
  for (uint8_t byte : value_) {
    if (encoded_size >= len) return KEncodeDecodeError;
    buf[encoded_size++] = byte;
  }

  // Write actual length
  int dummy = 0;
  ENCODE_U16(buf + len_pos, static_cast<uint16_t>(value_.size()), dummy);

  oai::logger::logger_common::nas().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int NsagInformation::Decode(const uint8_t* const buf, int len, bool is_iei) {
  oai::logger::logger_common::nas().debug("Decoding %s", GetIeName().c_str());
  int decoded_size = 0;

  int decoded_header_size = Type6NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  uint16_t content_len = GetLengthIndicator();

  if (static_cast<int>(decoded_size + content_len) > len) {
    oai::logger::logger_common::nas().error(
        "Buffer too short for %s content (%d needed, %d remaining)",
        GetIeName().c_str(), content_len, len - decoded_size);
    return KEncodeDecodeError;
  }

  value_.assign(buf + decoded_size, buf + decoded_size + content_len);
  decoded_size += content_len;

  oai::logger::logger_common::nas().debug(
      "Decoded %s (len %d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
