/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include <gtest/gtest.h>

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "3gpp_29.244.hpp"
#include "msg_pfcp.hpp"

extern std::vector<uint8_t> hexStringToByteArray(const std::string& hexString);

namespace {

std::string to_octets(const std::string& hex) {
  const std::vector<uint8_t> bytes = hexStringToByteArray(hex);
  return std::string(bytes.begin(), bytes.end());
}

std::unique_ptr<pfcp::pfcp_ie> load(const std::string& hex) {
  std::istringstream is(to_octets(hex));
  return std::unique_ptr<pfcp::pfcp_ie>(
      pfcp::pfcp_ie::new_pfcp_ie_from_stream(is));
}

}  // namespace

// TS 29.244 §8.1.1: the children of a grouped IE end with it. Created PDR
// (type 8) announces 5 octets but holds a 6-octet PDR ID (type 56).
TEST(TestSuitePfcpMsg, GroupedIeWithOverlongChildIsRejected) {
  EXPECT_THROW(
      load("00080005" + std::string("003800020001")),
      pfcp::pfcp_tlv_bad_length_exception);
}

// The same Created PDR with the right length decodes
TEST(TestSuitePfcpMsg, GroupedIeWithMatchingChildIsDecoded) {
  std::unique_ptr<pfcp::pfcp_ie> ie =
      load("00080006" + std::string("003800020001"));
  ASSERT_NE(ie, nullptr);
  auto* created_pdr = dynamic_cast<pfcp::pfcp_created_pdr_ie*>(ie.get());
  ASSERT_NE(created_pdr, nullptr);
  pfcp::created_pdr pdr = {};
  created_pdr->to_core_type(pdr);
  ASSERT_TRUE(pdr.pdr_id.first);
  EXPECT_EQ(pdr.pdr_id.second.rule_id, 1);
}
