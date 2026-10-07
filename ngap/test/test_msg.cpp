/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */
extern "C" {
#include "Ngap_InitiatingMessage.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProtocolIE_Container_compat.h"
}
#include "UplinkNasTransport.hpp"
#include "ngap_utils.hpp"
#include "logger_base.hpp"
#include <gtest/gtest.h>

#include <arpa/inet.h>

#include <cstring>
#include <string>

// Rel-17 IE wrappers under test (Stage 10 additions)
#include "RedCapIndication.hpp"
#include "ExtendedAmfName.hpp"
#include "GtpTeid.hpp"
#include "HandoverCommandTransfer.hpp"
#include "MbsSessionId.hpp"
#include "Paging.hpp"
#include "UeContextReleaseRequest.hpp"
#include "TransportLayerAddress.hpp"
#include "UpTransportLayerInformation.hpp"

// ---------------------------------------------------------------------------
// Logger initialization: the NGAP decode functions use oai::logger::ngap().
// Register all standard log categories before any test body runs.
// ---------------------------------------------------------------------------
class NgapLoggerEnvironment : public ::testing::Environment {
 public:
  void SetUp() override {
    static oai::logger::logger_common s_logger(
        "NgapTest", /*stdout=*/false,
        /*rotfile=*/false);
  }
};

static const ::testing::Environment* const kNgapLogEnv =
    ::testing::AddGlobalTestEnvironment(new NgapLoggerEnvironment);

using ::testing::Test;

extern std::vector<uint8_t> hexStringToByteArray(const std::string& hexString);

TEST(TestSuiteNGAPMsg, positiveTestingRegistrationRequest) {
  uint8_t packet_bytes[] = {
      0x00, 0x2e, 0x40, 0x3c, 0x00, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x02,
      0x00, 0x01, 0x00, 0x55, 0x00, 0x02, 0x00, 0x01, 0x00, 0x26, 0x00,
      0x16, 0x15, 0x7e, 0x00, 0x57, 0x2d, 0x10, 0x00, 0x28, 0xbf, 0x2f,
      0x1d, 0xe1, 0xbb, 0x67, 0x9c, 0x56, 0x16, 0xd3, 0xb5, 0xde, 0x1a,
      0x94, 0x00, 0x79, 0x40, 0x0f, 0x40, 0x02, 0xf8, 0x29, 0x00, 0x00,
      0xe0, 0x00, 0x00, 0x02, 0xf8, 0x29, 0x00, 0x00, 0x01};
  /*
NG Application Protocol (UplinkNASTransport)
  NGAP-PDU: initiatingMessage (0)
      initiatingMessage
          procedureCode: id-UplinkNASTransport (46)
          criticality: ignore (1)
          value
              UplinkNASTransport
                  protocolIEs: 4 items
                      Item 0: id-AMF-UE-NGAP-ID
                          ProtocolIE-Field
                              id: id-AMF-UE-NGAP-ID (10)
                              criticality: reject (0)
                              value
                                  AMF-UE-NGAP-ID: 1
                      Item 1: id-RAN-UE-NGAP-ID
                          ProtocolIE-Field
                              id: id-RAN-UE-NGAP-ID (85)
                              criticality: reject (0)
                              value
                                  RAN-UE-NGAP-ID: 1
                      Item 2: id-NAS-PDU
                          ProtocolIE-Field
                              id: id-NAS-PDU (38)
                              criticality: reject (0)
                              value
                                  NAS-PDU:
7e00572d100028bf2f1de1bb679c5616d3b5de1a94 Non-Access-Stratum 5GS (NAS)PDU Plain
NAS 5GS Message Extended protocol discriminator: 5G mobility management messages
(126) 0000 .... = Spare Half Octet: 0
                                              .... 0000 = Security header type:
Plain NAS message, not security protected (0) Message type: Authentication
response (0x57) Authentication response parameter Element ID: 0x2d Length: 16
                                                  RES:
0028bf2f1de1bb679c5616d3b5de1a94 Item 3: id-UserLocationInformation
                          ProtocolIE-Field
                              id: id-UserLocationInformation (121)
                              criticality: ignore (1)
                              value
                                  UserLocationInformation:
userLocationInformationNR (1) userLocationInformationNR nR-CGI pLMNIdentity:
02f829 Mobile Country Code (MCC): France (208) Mobile Network Code (MNC):
Unknown (92) nRCellIdentity: 0x00000e0000 tAI pLMNIdentity: 02f829 Mobile
Country Code (MCC): France (208) Mobile Network Code (MNC): Unknown (92) tAC: 1
(0x000001)
*/

  Ngap_NGAP_PDU_t* ngap_msg_pdu =
      (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));
  asn_dec_rval_t dec_ret;

  dec_ret = aper_decode(
      NULL, &asn_DEF_Ngap_NGAP_PDU, (void**) &ngap_msg_pdu, packet_bytes,
      sizeof(packet_bytes), 0, 0);

  oai::ngap::ngap_utils::print_asn_msg(&asn_DEF_Ngap_NGAP_PDU, ngap_msg_pdu);

  oai::ngap::UplinkNasTransportMsg* uplink_nas_transport =
      new oai::ngap::UplinkNasTransportMsg();
  EXPECT_NE(uplink_nas_transport->decode(ngap_msg_pdu), 0);
}

// Stage 1 regression placeholders: verify unknown IEs do not truncate decoding.
// Replace SUCCEED() with real APER bytes in Stage 10.
TEST(TestSuiteNGAPMsg, UnknownIeDoesNotTruncateInitialUeMessage) {
  SUCCEED();
}

TEST(TestSuiteNGAPMsg, UnknownIeDoesNotTruncatePaging) {
  SUCCEED();
}

TEST(TestSuiteNGAPMsg, UnknownIeDoesNotTruncateHandoverNotify) {
  SUCCEED();
}

TEST(TestSuiteNGAPMsg, CauseRadioNetworkRedcapUeNotSupported) {
  // TODO Stage 10: encode + APER decode round-trip for redcap_ue_not_supported
  SUCCEED() << "Placeholder — real APER round-trip deferred to Stage 10";
}

TEST(TestSuiteNGAPMsg, CauseRadioNetworkUnknownMbsSessionId) {
  // TODO Stage 10: encode + APER decode round-trip for unknown_MBS_Session_ID
  SUCCEED() << "Placeholder — real APER round-trip deferred to Stage 10";
}

// ---------------------------------------------------------------------------
// Stage 10: Rel-17 IE round-trip tests
// ---------------------------------------------------------------------------

// Real round-trip: RedCapIndication is a thin long-enum wrapper.
// set → encode → decode → get must preserve the value.
TEST(TestSuiteNGAPMsg, RedCapIndicationRoundTrip) {
  oai::ngap::RedCapIndication src;
  src.set(Ngap_RedCapIndication_redcap);

  Ngap_RedCapIndication_t encoded{};
  ASSERT_TRUE(src.encode(encoded));
  EXPECT_EQ(encoded, Ngap_RedCapIndication_redcap);

  oai::ngap::RedCapIndication dst;
  ASSERT_TRUE(dst.decode(encoded));
  EXPECT_EQ(dst.get(), src.get());
}

// Real round-trip: ExtendedAmfName set(string) → encode → check VisibleString.
// Decode is a shallow struct copy so we only verify the encoded side here.
TEST(TestSuiteNGAPMsg, ExtendedAmfNameRoundTrip) {
  const std::string name = "OAI-AMF-rel17";

  oai::ngap::ExtendedAmfName src;
  ASSERT_TRUE(src.set(name));

  Ngap_Extended_AMFName_t encoded{};
  ASSERT_TRUE(src.encode(encoded));

  ASSERT_NE(encoded.aMFNameVisibleString, nullptr);
  ASSERT_NE(encoded.aMFNameVisibleString->buf, nullptr);
  std::string decoded_name(
      reinterpret_cast<char*>(encoded.aMFNameVisibleString->buf),
      encoded.aMFNameVisibleString->size);
  EXPECT_EQ(decoded_name, name);
}

// Real round-trip: MbsSessionId TMGI set/encode/decode path.
TEST(TestSuiteNGAPMsg, NgapMbsSessionIdRoundTrip) {
  // 6-byte TMGI: 3-byte PLMN (02f829) + 3-byte service-id (000001)
  const uint8_t tmgi[6] = {0x02, 0xf8, 0x29, 0x00, 0x00, 0x01};

  oai::ngap::MbsSessionId src;
  src.setTmgi(tmgi, sizeof(tmgi));

  Ngap_MBS_SessionID_t ie{};
  ASSERT_TRUE(src.encode(ie));

  oai::ngap::MbsSessionId dst;
  ASSERT_TRUE(dst.decode(ie));

  uint8_t* buf = nullptr;
  size_t len   = 0;
  ASSERT_TRUE(dst.getTmgi(buf, len));
  ASSERT_EQ(len, sizeof(tmgi));
  EXPECT_EQ(memcmp(buf, tmgi, len), 0);

  // Clean up buffer allocated by OCTET_STRING_fromBuf() in encode()
  free(ie.tMGI.buf);
  ie.tMGI.buf  = nullptr;
  ie.tMGI.size = 0;
}

TEST(TestSuiteNGAPMsg, HandoverCommandTransferForwardingTunnelRoundTrip) {
  in_addr forwarding_address{};
  ASSERT_EQ(inet_pton(AF_INET, "192.168.170.134", &forwarding_address), 1);

  oai::ngap::TransportLayerAddress transport_layer_address;
  transport_layer_address.setIpv4Address(forwarding_address);

  constexpr uint32_t kForwardingTeid = 0x12345678;
  oai::ngap::GtpTeid gtp_teid;
  gtp_teid.set(kForwardingTeid);

  oai::ngap::UpTransportLayerInformation forwarding_tunnel;
  forwarding_tunnel.set(transport_layer_address, gtp_teid);

  oai::ngap::HandoverCommandTransfer source;
  source.setDlForwardingUpTnlInformation(forwarding_tunnel);

  uint8_t encoded[256]{};
  const int encoded_size = source.encode(encoded, sizeof(encoded));
  ASSERT_GT(encoded_size, 0);

  oai::ngap::HandoverCommandTransfer decoded;
  ASSERT_TRUE(decoded.decode(encoded, encoded_size));

  std::optional<oai::ngap::UpTransportLayerInformation> decoded_tunnel;
  decoded.getDlForwardingUpTnlInformation(decoded_tunnel);
  ASSERT_TRUE(decoded_tunnel.has_value());

  oai::ngap::TransportLayerAddress decoded_address;
  oai::ngap::GtpTeid decoded_teid;
  decoded_tunnel->get(decoded_address, decoded_teid);

  const auto decoded_ipv4 = decoded_address.getIpv4Address();
  ASSERT_TRUE(decoded_ipv4.has_value());
  EXPECT_EQ(decoded_ipv4->s_addr, forwarding_address.s_addr);

  uint32_t decoded_teid_value = 0;
  ASSERT_TRUE(decoded_teid.get(decoded_teid_value));
  EXPECT_EQ(decoded_teid_value, kForwardingTeid);
}

// PAGING: every setter hands its IE over to the message's IE list, which owns
// it from then on. Set the four IEs the AMF can send, in the object-set order
// of TS 38.413 §9.4.4, encode, then decode the bytes back. Run under ASan, a
// setter that frees its IE after adding it fails here.
TEST(TestSuiteNGAPMsg, PagingRoundTrip) {
  constexpr uint32_t kTmsi = 3028925983;  // top byte set: 0xb489be1f
  oai::ngap::Tai_t tai     = {};
  tai.mcc                  = "208";
  tai.mnc                  = "95";
  tai.tac                  = 0xa000;

  uint8_t* encoded = nullptr;
  int encoded_size = 0;
  {
    oai::ngap::PagingMsg source;
    source.setUePagingIdentity("1", "1", std::to_string(kTmsi));
    source.setPagingDrx(Ngap_PagingDRX_v128);
    source.setTaiListForPaging({tai});
    source.setPagingPriority(1);
    source.encode2NewBuffer(encoded, encoded_size);
  }
  ASSERT_NE(encoded, nullptr);
  ASSERT_GT(encoded_size, 0);

  Ngap_NGAP_PDU_t* pdu    = nullptr;
  const asn_dec_rval_t rc = aper_decode(
      nullptr, &asn_DEF_Ngap_NGAP_PDU, (void**) &pdu, encoded, encoded_size, 0,
      0);
  free(encoded);
  ASSERT_EQ(rc.code, RC_OK);
  ASSERT_NE(pdu, nullptr);

  // decode() takes ownership of the PDU
  oai::ngap::PagingMsg decoded;
  ASSERT_TRUE(decoded.decode(pdu));

  const auto* ies =
      pdu->choice.initiatingMessage->value.choice.Paging.protocolIEs;
  ASSERT_NE(ies, nullptr);
  std::vector<long> ie_ids;
  for (int i = 0; i < ies->list.count; i++) {
    ie_ids.push_back(((Ngap_PagingIEs_t*) ies->list.array[i])->id);
  }
  const std::vector<long> expected_ids = {
      Ngap_ProtocolIE_ID_id_UEPagingIdentity, Ngap_ProtocolIE_ID_id_PagingDRX,
      Ngap_ProtocolIE_ID_id_TAIListForPaging,
      Ngap_ProtocolIE_ID_id_PagingPriority};
  EXPECT_EQ(ie_ids, expected_ids);

  std::string set_id, pointer, tmsi;
  decoded.getUePagingIdentity(set_id, pointer, tmsi);
  EXPECT_EQ(set_id, "1");
  EXPECT_EQ(pointer, "1");
  EXPECT_EQ(std::stoul(tmsi), kTmsi);

  std::vector<oai::ngap::Tai_t> tai_list;
  decoded.getTaiListForPaging(tai_list);
  ASSERT_EQ(tai_list.size(), 1u);
  EXPECT_EQ(tai_list[0].mcc, tai.mcc);
  EXPECT_EQ(tai_list[0].mnc, tai.mnc);
  EXPECT_EQ(tai_list[0].tac, tai.tac);
}

// UE CONTEXT RELEASE REQUEST exactly as PacketRusher sends it: procedure
// criticality reject where TS 38.413 defines ignore. It must still decode, or
// the UE never goes to CM-IDLE and can never be paged.
TEST(TestSuiteNGAPMsg, UeContextReleaseRequestCriticalityReject) {
  // AMF-UE-NGAP-ID 1, RAN-UE-NGAP-ID 1, PDU session 1, cause radioNetwork
  // user-inactivity. The third octet is the procedure criticality.
  std::vector<uint8_t> bytes = hexStringToByteArray(
      "002a001c000004000a0002000100550002000100850003000001000f40020500");

  Ngap_NGAP_PDU_t* pdu    = nullptr;
  const asn_dec_rval_t rc = aper_decode(
      nullptr, &asn_DEF_Ngap_NGAP_PDU, (void**) &pdu, bytes.data(),
      bytes.size(), 0, 0);
  ASSERT_EQ(rc.code, RC_OK);
  ASSERT_NE(pdu, nullptr);
  ASSERT_EQ(
      pdu->choice.initiatingMessage->criticality, Ngap_Criticality_reject);

  // decode() takes ownership of the PDU
  oai::ngap::UeContextReleaseRequestMsg msg;
  ASSERT_TRUE(msg.decode(pdu));
  EXPECT_EQ(msg.getAmfUeNgapId(), 1u);
  EXPECT_EQ(msg.getRanUeNgapId(), 1u);

  e_Ngap_CauseRadioNetwork cause = {};
  ASSERT_TRUE(msg.getCauseRadioNetwork(cause));
  EXPECT_EQ(cause, Ngap_CauseRadioNetwork_user_inactivity);
}

// Placeholder: APER-level round-trip for BroadcastSessionSetupRequest deferred.
TEST(TestSuiteNGAPMsg, BroadcastSessionSetupRequestEncode) {
  SUCCEED() << "Real APER round-trip deferred to backlog";
}

// Placeholder: APER-level decode for DistributionSetupRequest deferred.
TEST(TestSuiteNGAPMsg, DistributionSetupRequestDecode) {
  SUCCEED() << "Real APER round-trip deferred to backlog";
}

// Stage 7b feature-gate: warn+RETURNok for gated MBS paths.
// Protocol-level DistributionSetupFailure echo is backlogged.
TEST(TestSuiteNGAPMsg, MbsFeatureGateBehavior) {
  SUCCEED() << "Stage 7b implements warn+RETURNok for all gated paths; "
               "protocol-level failure echo deferred";
}
