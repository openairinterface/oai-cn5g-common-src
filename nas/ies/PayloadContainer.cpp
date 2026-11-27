/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "PayloadContainer.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.hpp"
#include "logger_base.hpp"

using namespace oai::nas;

namespace {
// Copy of a bstring's octets (empty for a null or empty bstring)
std::vector<uint8_t> copy_octets(const bstring& b) {
  if (b == nullptr || bdata(b) == nullptr || blength(b) <= 0) return {};
  const uint8_t* data = (const uint8_t*) bdata(b);
  return std::vector<uint8_t>(data, data + blength(b));
}
}  // namespace

//------------------------------------------------------------------------------
PayloadContainer::PayloadContainer() : Type6NasIe() {
  content_  = std::nullopt;
  contents_ = std::nullopt;
  SetLengthIndicator(kPayloadContainerContentMinimumLength);
}

//------------------------------------------------------------------------------
PayloadContainer::PayloadContainer(uint8_t iei) : Type6NasIe(iei) {
  content_  = std::nullopt;
  contents_ = std::nullopt;
  SetLengthIndicator(kPayloadContainerContentMinimumLength);
}

//------------------------------------------------------------------------------
PayloadContainer::PayloadContainer(const bstring& b) : Type6NasIe() {
  content_  = copy_octets(b);
  contents_ = std::nullopt;
  SetLengthIndicator(
      (content_.value().size() > kPayloadContainerContentMinimumLength) ?
          content_.value().size() :
          kPayloadContainerContentMinimumLength);
}

//------------------------------------------------------------------------------
PayloadContainer::PayloadContainer(uint8_t iei, const bstring& b)
    : Type6NasIe(iei) {
  content_  = copy_octets(b);
  contents_ = std::nullopt;
  SetLengthIndicator(
      (content_.value().size() > kPayloadContainerContentMinimumLength) ?
          content_.value().size() :
          kPayloadContainerContentMinimumLength);
}

//------------------------------------------------------------------------------
PayloadContainer::PayloadContainer(
    const std::vector<PayloadContainerEntry>& contents)
    : Type6NasIe() {
  content_ = std::nullopt;
  if (contents.size() == 0) return;
  int length = 1;  // for number of entries
  contents_  = std::optional<std::vector<PayloadContainerEntry>>(contents);
  for (int i = 0; i < contents.size(); i++) {
    length = length + 2 +
             contents.at(i).length;  // 2 for Length of Payload container entry
  }
  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
PayloadContainer::PayloadContainer(
    uint8_t iei, const std::vector<PayloadContainerEntry>& contents)
    : Type6NasIe(iei) {
  content_ = std::nullopt;
  if (contents.size() == 0) return;

  int length = 1;  // for number of entries
  contents_  = std::optional<std::vector<PayloadContainerEntry>>(contents);
  for (int i = 0; i < contents.size(); i++) {
    length = length + 2 +
             contents.at(i).length;  // 2 for Length of Payload container entry
  }
  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
PayloadContainer::~PayloadContainer() {}

//------------------------------------------------------------------------------
void PayloadContainer::SetValue(const bstring& cnt) {
  content_ = copy_octets(cnt);
  SetLengthIndicator(
      (content_.value().size() > kPayloadContainerContentMinimumLength) ?
          content_.value().size() :
          kPayloadContainerContentMinimumLength);
}

//------------------------------------------------------------------------------
void PayloadContainer::SetValue(
    const std::vector<PayloadContainerEntry>& content) {
  if (content.size() == 0) return;
  int length = 1;  // for number of entries
  // contents_.assign(content.begin(), content.end());
  contents_ = std::optional<std::vector<PayloadContainerEntry>>(content);
  for (int i = 0; i < content.size(); i++) {
    length = length + 2 +
             content.at(i).length;  // 2 for Length of Payload container entry
  }
  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
bool PayloadContainer::GetValue(
    std::vector<PayloadContainerEntry>& content) const {
  if (contents_.has_value()) {
    content.assign(contents_.value().begin(), contents_.value().end());
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool PayloadContainer::GetValue(bstring& cnt) const {
  if (content_.has_value()) {
    // The caller owns the returned copy
    cnt = content_.value().empty() ?
              bfromcstr("") :
              blk2bstr(content_.value().data(), content_.value().size());
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
int PayloadContainer::Encode(uint8_t* buf, int len, uint8_t type) const {
  oai::logger::logger_common::nas().debug("Encoding %s", GetIeName().c_str());

  int encoded_size = 0;
  // Validate the buffer's length and Encode IEI/Length
  int len_pos = 0;
  int encoded_header_size =
      Type6NasIe::Encode(buf + encoded_size, len, len_pos);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  if (content_.has_value()) {
    if (!content_.value().empty())
      memcpy(
          buf + encoded_size, content_.value().data(), content_.value().size());
    encoded_size += content_.value().size();
  } else if (contents_.has_value()) {
    // Number of entries
    ENCODE_U8(buf + encoded_size, contents_.value().size(), encoded_size);

    for (int i = 0; i < contents_.value().size(); i++) {
      // Length of Payload container entry
      ENCODE_U8(
          buf + encoded_size, contents_.value().at(i).length, encoded_size);
      // Number of optional IEs and Payload container type
      uint8_t octet = 0;
      octet = ((contents_.value().at(i).optionalIE.size() & 0x0f) << 4) |
              contents_.value().at(i).payloadContainerType;
      ENCODE_U8(buf + encoded_size, octet, encoded_size);
      // Optional IEs
      for (int j = 0; j < contents_.value().at(i).optionalIE.size(); j++) {
        // Type
        ENCODE_U8(
            buf + encoded_size,
            contents_.value().at(i).optionalIE.at(j).ie_type, encoded_size);
        // Length
        ENCODE_U8(
            buf + encoded_size, contents_.value().at(i).optionalIE.at(j).ie_len,
            encoded_size);
        // Value
        int size = encode_bstring(
            contents_.value().at(i).optionalIE.at(j).ie_value,
            (buf + encoded_size), len - encoded_size);
        encoded_size += size;
      }
    }
  }

  // Encode length
  int encoded_len_ie = 0;
  ENCODE_U16(buf + len_pos, encoded_size - GetHeaderLength(), encoded_len_ie);
  oai::logger::logger_common::nas().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int PayloadContainer::Decode(
    const uint8_t* const buf, int len, bool is_iei, uint8_t type) {
  oai::logger::logger_common::nas().debug("Decoding %s", GetIeName().c_str());
  int decoded_size = 0;

  // IEI and Length
  uint16_t ie_len         = 0;
  int decoded_header_size = Type6NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;
  ie_len = GetLengthIndicator();

  if (type != kMultiplePayloads) {  // not multiple payloads
    content_ =
        std::vector<uint8_t>(buf + decoded_size, buf + decoded_size + ie_len);
    decoded_size += ie_len;
    return decoded_size;
  }

  // Multiple payloads
  uint8_t num_entries  = {};
  uint8_t num_optional = {};
  std::vector<PayloadContainerEntry> CONTENT_tmp;

  PayloadContainerEntry payloadcontainerentry = {};
  DECODE_U8(buf + decoded_size, num_entries, decoded_size);

  while (num_entries) {
    uint8_t length_entry = 0;
    DECODE_U8(buf + decoded_size, length_entry, decoded_size);
    uint8_t octet = 0;
    // Number of optional IEs and Payload container type
    DECODE_U8(buf + decoded_size, octet, decoded_size);
    payloadcontainerentry.payloadContainerType = octet & 0x0f;
    num_optional                               = (octet & 0xf0) >> 4;

    IE_t value;
    while (num_optional) {
      DECODE_U8(buf + decoded_size, value.ie_type, decoded_size);
      DECODE_U8(buf + decoded_size, value.ie_len, decoded_size);

      decode_bstring(
          &value.ie_value, value.ie_len, (buf + decoded_size),
          len - decoded_size);
      decoded_size += value.ie_len;
      payloadcontainerentry.optionalIE.insert(
          payloadcontainerentry.optionalIE.end(), value);
      num_optional--;
    }
    CONTENT_tmp.insert(CONTENT_tmp.end(), payloadcontainerentry);
    num_entries--;
  }
  contents_ = std::optional<std::vector<PayloadContainerEntry>>(contents_);
  oai::logger::logger_common::nas().debug(
      "Decoded %s (len %d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
