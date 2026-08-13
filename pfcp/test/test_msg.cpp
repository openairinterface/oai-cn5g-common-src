/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */
#include "3gpp_29.244.hpp"

#include <gtest/gtest.h>

#include <cstdio>
#include <sstream>
#include <string>

using ::testing::Test;

namespace {

// Session Modification Request fixture used by the tests below: the SMF arms
// downlink buffering + data notification at AN Release by sending, in a single
// PFCP Session Modification Request, an Update FAR whose Apply Action carries
// BUFF|NOCP and which references a BAR, plus the top-level Create BAR that
// actually creates that BAR on the UP function.
// @see 3GPP TS 29.244 V17.10.0 Table 7.5.4.1-1 (Create BAR row), Table
//      7.5.4.3-1 (Update FAR BAR ID row), Table 7.5.2.6-1 (Create BAR IE).
constexpr uint64_t kUpSeid       = 0x0102030405060708;
constexpr uint32_t kSequenceNb   = 1;
constexpr uint64_t kCpSeid       = 0x00000000abcdef01;
constexpr uint32_t kCpIpv4Addr   = 0xc0a80101;  // 192.168.1.1, host order
constexpr uint32_t kFarId        = 2;
constexpr int kBarId             = 7;
constexpr size_t kEncodedMsgSize = 64;

pfcp::pfcp_session_modification_request make_arm_dl_buffering_request() {
  pfcp::pfcp_session_modification_request req = {};

  pfcp::fseid_t cp_fseid       = {};
  cp_fseid.v4                  = 1;
  cp_fseid.seid                = kCpSeid;
  cp_fseid.ipv4_address.s_addr = htobe32(kCpIpv4Addr);
  req.cp_fseid.first           = true;
  req.cp_fseid.second          = cp_fseid;

  pfcp::apply_action_t apply_action = {};
  apply_action.buff                 = 1;
  apply_action.nocp                 = 1;

  pfcp::bar_id_t bar_id = {};
  bar_id.bar_id         = static_cast<uint8_t>(kBarId);

  pfcp::update_far update_far = {};
  update_far.far_id.far_id    = kFarId;
  update_far.set(apply_action);
  update_far.set(bar_id);
  req.set(update_far);

  pfcp::create_bar create_bar = {};
  create_bar.set(bar_id);
  req.set(create_bar);

  return req;
}

std::string encode(const pfcp::pfcp_session_modification_request& req) {
  pfcp::pfcp_msg msg(req);
  msg.set_seid(kUpSeid);
  msg.set_sequence_number(kSequenceNb);
  std::ostringstream oss;
  msg.dump_to(oss);
  return oss.str();
}

std::string to_hex(const std::string& bytes) {
  std::string hex;
  char buf[3];
  for (const char& c : bytes) {
    std::snprintf(buf, sizeof(buf), "%02x", static_cast<uint8_t>(c));
    hex.append(buf, 2);
  }
  return hex;
}

}  // namespace

// Encode a Session Modification Request carrying an Update FAR that references
// a BAR ID plus a top-level Create BAR, decode it back and recover both.
// Regression test for the top-level Create BAR add that used to be commented
// out of pfcp_msg::pfcp_msg(const pfcp_session_modification_request&).
TEST(TestSuitePfcpMsg, positiveTestingSessionModificationRequestCreateBar) {
  const std::string bytes = encode(make_arm_dl_buffering_request());

  pfcp::pfcp_msg in_msg = {};
  std::istringstream iss(bytes);
  in_msg.load_from(iss);

  EXPECT_EQ(
      static_cast<int>(in_msg.get_message_type()),
      PFCP_SESSION_MODIFICATION_REQUEST);
  EXPECT_EQ(in_msg.get_seid(), kUpSeid);

  pfcp::pfcp_session_modification_request out = {};
  in_msg.to_core_type(out);

  // Update FAR (FAR ID + Apply Action BUFF|NOCP + BAR ID reference).
  ASSERT_EQ(out.update_fars.size(), 1u);
  const pfcp::update_far& far = out.update_fars[0];
  EXPECT_EQ(far.far_id.far_id, kFarId);
  ASSERT_TRUE(far.apply_action.first);
  EXPECT_EQ(static_cast<int>(far.apply_action.second.buff), 1);
  EXPECT_EQ(static_cast<int>(far.apply_action.second.nocp), 1);
  ASSERT_TRUE(far.bar_id.first);
  EXPECT_EQ(static_cast<int>(far.bar_id.second.bar_id), kBarId);

  // Top-level Create BAR (BAR ID only — Downlink Data Notification Delay and
  // Suggested Buffering Packets Count are not encodable by the library yet).
  ASSERT_TRUE(out.create_bar.first);
  ASSERT_TRUE(out.create_bar.second.bar_id.first);
  EXPECT_EQ(
      static_cast<int>(out.create_bar.second.bar_id.second.bar_id), kBarId);

  // The BAR referenced by the Update FAR is the one the Create BAR creates.
  EXPECT_EQ(
      static_cast<int>(out.create_bar.second.bar_id.second.bar_id),
      static_cast<int>(far.bar_id.second.bar_id));
}

// Byte-stability of the wire image, so the SMF encoder and the UPF decoder
// cannot drift apart: the encoded message must contain the Create BAR IE
// (type 85) holding a single BAR ID IE (type 88).
TEST(TestSuitePfcpMsg, positiveTestingSessionModificationRequestCreateBarWire) {
  const std::string bytes = encode(make_arm_dl_buffering_request());

  ASSERT_EQ(bytes.size(), kEncodedMsgSize);
  EXPECT_EQ(
      to_hex(bytes),
      "2134003c010203040506070800000100"    // header: S=1, type 52, len 60
      "0039000d0200000000abcdef01c0a80101"  // F-SEID (57) len 13
      "00550005"                            // Create BAR (85) len 5
      "0058000107"                          // BAR ID (88) len 1, value 7
      "000a0012"                            // Update FAR (10) len 18
      "006c000400000002"                    // FAR ID (108) len 4, value 2
      "002c00010c"                          // Apply Action (44) BUFF|NOCP
      "0058000107");                        // BAR ID (88) len 1, value 7

  // Locate the Create BAR IE by walking the top-level TLVs.
  size_t offset         = 16;  // PFCP message header with SEID
  bool create_bar_found = false;
  while (offset + 4 <= bytes.size()) {
    const uint16_t type = (static_cast<uint8_t>(bytes[offset]) << 8) |
                          static_cast<uint8_t>(bytes[offset + 1]);
    const uint16_t length = (static_cast<uint8_t>(bytes[offset + 2]) << 8) |
                            static_cast<uint8_t>(bytes[offset + 3]);
    if (type == PFCP_IE_CREATE_BAR) {
      create_bar_found = true;
      ASSERT_EQ(static_cast<int>(length), 5);
      EXPECT_EQ(
          (static_cast<uint8_t>(bytes[offset + 4]) << 8) |
              static_cast<uint8_t>(bytes[offset + 5]),
          static_cast<int>(PFCP_IE_BAR_ID));
      EXPECT_EQ(
          static_cast<int>(static_cast<uint8_t>(bytes[offset + 8])), kBarId);
      break;
    }
    offset += 4 + length;
  }
  EXPECT_TRUE(create_bar_found);
}
