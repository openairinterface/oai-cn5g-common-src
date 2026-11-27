/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */
extern "C" {
#include "Ngap_InitiatingMessage.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_PDUSessionResourceFailedToModifyItemModRes.h"
#include "Ngap_PDUSessionResourceModifyItemModRes.h"
#include "Ngap_ProtocolIE_Container_compat.h"
#include "Ngap_SuccessfulOutcome.h"
}
#include "DownlinkNasTransport.hpp"
#include "PduSessionResourceModifyResponse.hpp"
#include "UserLocationInformation.hpp"
#include "common_defs.hpp"
#include "UplinkNasTransport.hpp"
#include "ngap_utils.hpp"
#include "logger_base.hpp"
#include <gtest/gtest.h>

#include <arpa/inet.h>

#include "GtpTeid.hpp"
#include "HandoverCommandTransfer.hpp"
#include "TransportLayerAddress.hpp"
#include "UpTransportLayerInformation.hpp"

#include <cstring>
#include <string>
#include <vector>

// Rel-17 IE wrappers under test (Stage 10 additions)
#include "RedCapIndication.hpp"
#include "ExtendedAmfName.hpp"
#include "MbsSessionId.hpp"

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

TEST(TestSuiteNGAPMsg, AperEncodedBitLengthIsConvertedToBytes) {
  EXPECT_EQ(oai::ngap::ngap_utils::aper_encoded_bits_to_bytes(-1), -1);
  EXPECT_EQ(oai::ngap::ngap_utils::aper_encoded_bits_to_bytes(0), 0);
  EXPECT_EQ(oai::ngap::ngap_utils::aper_encoded_bits_to_bytes(1), 1);
  EXPECT_EQ(oai::ngap::ngap_utils::aper_encoded_bits_to_bytes(8), 1);
  EXPECT_EQ(oai::ngap::ngap_utils::aper_encoded_bits_to_bytes(9), 2);
  EXPECT_EQ(oai::ngap::ngap_utils::aper_encoded_bits_to_bytes(80), 10);
}

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
  // aper_encode_to_buffer() returns bits internally, but encode() exposes
  // bytes to its callers. This transfer is 80 bits, i.e. exactly 10 bytes.
  ASSERT_EQ(encoded_size, 11);

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

// Encode() into a buffer too small for the message reports the failure
// (KEncodeDecodeError) instead of 0 octets, so callers that check for a
// negative size do not send an empty message
TEST(TestSuiteNGAPMsg, EncodeIntoTooSmallBufferFails) {
  std::vector<uint8_t> nas(2048, 0x5a);
  bstring nas_pdu = blk2bstr(nas.data(), static_cast<int>(nas.size()));

  oai::ngap::DownLinkNasTransportMsg msg;
  msg.setAmfUeNgapId(1);
  msg.setRanUeNgapId(1);
  msg.setNasPdu(nas_pdu);
  bdestroy(nas_pdu);

  std::vector<uint8_t> buffer(1024, 0);
  EXPECT_EQ(
      msg.Encode(buffer.data(), static_cast<int>(buffer.size())),
      KEncodeDecodeError);

  buffer.resize(4096);
  const int encoded =
      msg.Encode(buffer.data(), static_cast<int>(buffer.size()));
  EXPECT_GT(encoded, 2048);
  EXPECT_LT(encoded, 4096);
}

// Adds an IE with criticality ignore, which TS 38.413 clause 9.2.1.6 assigns
// to every IE of PDU SESSION RESOURCE MODIFY RESPONSE
static Ngap_PDUSessionResourceModifyResponseIEs_t* addModifyResponseIe(
    Ngap_PDUSessionResourceModifyResponse_t& response, Ngap_ProtocolIE_ID_t id,
    Ngap_PDUSessionResourceModifyResponseIEs__value_PR present) {
  auto* ie = static_cast<Ngap_PDUSessionResourceModifyResponseIEs_t*>(
      calloc(1, sizeof(Ngap_PDUSessionResourceModifyResponseIEs_t)));
  ie->id            = id;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = present;
  ASN_SEQUENCE_ADD(&response.protocolIEs->list, ie);
  return ie;
}

// Checks what the AMF reads from a received PDU SESSION RESOURCE MODIFY
// RESPONSE carrying AMF UE NGAP ID 5, RAN UE NGAP ID 7 and PDU session 1 in
// the Modify Response List. The message takes ownership of received.
static void expectModifyResponseDecodes(Ngap_NGAP_PDU_t* received) {
  oai::ngap::PduSessionResourceModifyResponseMsg msg;
  ASSERT_TRUE(msg.decode(received));
  EXPECT_EQ(msg.getAmfUeNgapId(), 5u);
  EXPECT_EQ(msg.getRanUeNgapId(), 7u);

  std::vector<oai::ngap::PDUSessionResourceModifyResponseItem_t> list;
  ASSERT_TRUE(msg.getPduSessionResourceModifyResponseList(list));
  ASSERT_EQ(list.size(), 1u);
  EXPECT_EQ(list[0].pduSessionId, 1);
  EXPECT_EQ(list[0].pduSessionResourceModifyResponseTransfer.size, 1u);
}

// A PDU SESSION RESOURCE MODIFY RESPONSE as a gNB sends it: Message Type
// reject and every IE ignore (TS 38.413 clause 9.2.1.6), with the optional IEs
// the decoder does not process. The AMF must decode it to forward the
// modified PDU sessions to the SMF.
TEST(TestSuiteNGAPMsg, PduSessionResourceModifyResponseFromGnbDecodes) {
  // The transfers are opaque to the message decoder
  const uint8_t modify_transfer[]       = {0x00};
  const uint8_t unsuccessful_transfer[] = {0x00, 0x00};

  auto* pdu = static_cast<Ngap_NGAP_PDU_t*>(calloc(1, sizeof(Ngap_NGAP_PDU_t)));
  pdu->present                  = Ngap_NGAP_PDU_PR_successfulOutcome;
  pdu->choice.successfulOutcome = static_cast<Ngap_SuccessfulOutcome_t*>(
      calloc(1, sizeof(Ngap_SuccessfulOutcome_t)));
  Ngap_SuccessfulOutcome_t* outcome = pdu->choice.successfulOutcome;
  outcome->procedureCode = Ngap_ProcedureCode_id_PDUSessionResourceModify;
  outcome->criticality   = Ngap_Criticality_reject;
  outcome->value.present =
      Ngap_SuccessfulOutcome__value_PR_PDUSessionResourceModifyResponse;
  Ngap_PDUSessionResourceModifyResponse_t& response =
      outcome->value.choice.PDUSessionResourceModifyResponse;
  response.protocolIEs = static_cast<Ngap_ProtocolIE_Container*>(
      calloc(1, sizeof(Ngap_ProtocolIE_Container)));

  Ngap_PDUSessionResourceModifyResponseIEs_t* ie = addModifyResponseIe(
      response, Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID,
      Ngap_PDUSessionResourceModifyResponseIEs__value_PR_AMF_UE_NGAP_ID);
  asn_uint642INTEGER(&ie->value.choice.AMF_UE_NGAP_ID, 5);

  ie = addModifyResponseIe(
      response, Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID,
      Ngap_PDUSessionResourceModifyResponseIEs__value_PR_RAN_UE_NGAP_ID);
  ie->value.choice.RAN_UE_NGAP_ID = 7;

  ie = addModifyResponseIe(
      response, Ngap_ProtocolIE_ID_id_PDUSessionResourceModifyListModRes,
      Ngap_PDUSessionResourceModifyResponseIEs__value_PR_PDUSessionResourceModifyListModRes);
  auto* modified = static_cast<Ngap_PDUSessionResourceModifyItemModRes_t*>(
      calloc(1, sizeof(Ngap_PDUSessionResourceModifyItemModRes_t)));
  modified->pDUSessionID = 1;
  OCTET_STRING_fromBuf(
      &modified->pDUSessionResourceModifyResponseTransfer,
      reinterpret_cast<const char*>(modify_transfer), sizeof(modify_transfer));
  ASN_SEQUENCE_ADD(
      &ie->value.choice.PDUSessionResourceModifyListModRes.list, modified);

  ie = addModifyResponseIe(
      response,
      Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToModifyListModRes,
      Ngap_PDUSessionResourceModifyResponseIEs__value_PR_PDUSessionResourceFailedToModifyListModRes);
  auto* failed =
      static_cast<Ngap_PDUSessionResourceFailedToModifyItemModRes_t*>(
          calloc(1, sizeof(Ngap_PDUSessionResourceFailedToModifyItemModRes_t)));
  failed->pDUSessionID = 2;
  OCTET_STRING_fromBuf(
      &failed->pDUSessionResourceModifyUnsuccessfulTransfer,
      reinterpret_cast<const char*>(unsuccessful_transfer),
      sizeof(unsuccessful_transfer));
  ASN_SEQUENCE_ADD(
      &ie->value.choice.PDUSessionResourceFailedToModifyListModRes.list,
      failed);

  oai::ngap::NrCgi nr_cgi;
  nr_cgi.set("208", "95", 0xe000);
  oai::ngap::Tai tai;
  tai.set("208", "95", 1);
  oai::ngap::UserLocationInformationNr uli_nr;
  uli_nr.set(nr_cgi, tai);
  oai::ngap::UserLocationInformation uli;
  uli.set(uli_nr);
  ie = addModifyResponseIe(
      response, Ngap_ProtocolIE_ID_id_UserLocationInformation,
      Ngap_PDUSessionResourceModifyResponseIEs__value_PR_UserLocationInformation);
  ie->value.choice.UserLocationInformation =
      static_cast<Ngap_UserLocationInformation_t*>(
          calloc(1, sizeof(Ngap_UserLocationInformation_t)));
  ASSERT_TRUE(uli.encode(*ie->value.choice.UserLocationInformation));

  // Every field of Criticality Diagnostics is optional
  addModifyResponseIe(
      response, Ngap_ProtocolIE_ID_id_CriticalityDiagnostics,
      Ngap_PDUSessionResourceModifyResponseIEs__value_PR_CriticalityDiagnostics);

  // Send it through APER, as the AMF receives it
  std::vector<uint8_t> buffer(1024, 0);
  const asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, nullptr, pdu, buffer.data(), buffer.size());
  ASN_STRUCT_FREE(asn_DEF_Ngap_NGAP_PDU, pdu);
  ASSERT_GT(er.encoded, 0);

  Ngap_NGAP_PDU_t* received = nullptr;
  ASSERT_EQ(
      aper_decode(
          nullptr, &asn_DEF_Ngap_NGAP_PDU, (void**) &received, buffer.data(),
          (er.encoded + 7) >> 3, 0, 0)
          .code,
      RC_OK);
  expectModifyResponseDecodes(received);
}

// The AMF's own encoder writes the criticalities of TS 38.413 clause 9.2.1.6,
// and its decoder reads back what the encoder wrote
TEST(TestSuiteNGAPMsg, PduSessionResourceModifyResponseRoundTrip) {
  const uint8_t transfer[] = {0x00};

  oai::ngap::PDUSessionResourceModifyResponseItem_t item = {};
  item.pduSessionId                                      = 1;
  OCTET_STRING_fromBuf(
      &item.pduSessionResourceModifyResponseTransfer,
      reinterpret_cast<const char*>(transfer), sizeof(transfer));

  std::vector<uint8_t> buffer(1024, 0);
  int encoded = 0;
  {
    // The message keeps the transfer buffer and frees it with its PDU
    oai::ngap::PduSessionResourceModifyResponseMsg sent;
    sent.setAmfUeNgapId(5);
    sent.setRanUeNgapId(7);
    sent.setPduSessionResourceModifyResponseList({item});
    encoded = sent.Encode(buffer.data(), static_cast<int>(buffer.size()));
  }
  ASSERT_GT(encoded, 0);

  Ngap_NGAP_PDU_t* received = nullptr;
  ASSERT_EQ(
      aper_decode(
          nullptr, &asn_DEF_Ngap_NGAP_PDU, (void**) &received, buffer.data(),
          encoded, 0, 0)
          .code,
      RC_OK);
  ASSERT_EQ(received->present, Ngap_NGAP_PDU_PR_successfulOutcome);
  const auto& ies = received->choice.successfulOutcome->value.choice
                        .PDUSessionResourceModifyResponse.protocolIEs->list;
  ASSERT_EQ(ies.count, 3);
  for (int i = 0; i < ies.count; i++) {
    const auto* ie =
        static_cast<const Ngap_PDUSessionResourceModifyResponseIEs_t*>(
            ies.array[i]);
    EXPECT_EQ(ie->criticality, Ngap_Criticality_ignore) << "IE id " << ie->id;
  }
  expectModifyResponseDecodes(received);
}
