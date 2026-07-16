/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef FILE_3GPP_129_244_R17_H_SEEN
#define FILE_3GPP_129_244_R17_H_SEEN
#include "3gpp_29.274.h"
#include "common_root_types.h"
#include "conversions.hpp"
#include <fmt/format.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <string>
#include <vector>

namespace pfcp {
//-------------------------------------
// 8.1.1 IE with Enterprise Info
typedef struct enterprise_specific_s {
  uint16_t enterprise_id;
  std::string proprietary_data;
} enterprise_specific_t;
//-------------------------------------

struct pfcp_exception : public std::exception {
  pfcp_exception() throw() {
    cause = 0;
    phrase.assign("PFCP Exception unknown cause");
  }

  pfcp_exception(int acause) throw() {
    cause  = acause;
    phrase = fmt::format("PFCP Exception cause {}", cause);
  }
  const char* what() const throw() { return phrase.c_str(); }

 public:
  int cause;
  std::string phrase;
};

struct pfcp_msg_bad_length_exception : public pfcp_exception {
 public:
  pfcp_msg_bad_length_exception(
      const uint8_t msg_type, const uint16_t hdr_size, const uint16_t ie_size,
      const uint16_t check_ie_size, const char* file, const int line) throw() {
    phrase = fmt::format(
        "PFCP msg {} Bad Length hdr.length {}/ sum ie {} / check sum ie {} "
        "Exception {}:{}",
        msg_type, hdr_size, ie_size, check_ie_size, file, line);
  }
  pfcp_msg_bad_length_exception(std::string& aphrase) throw() {
    phrase = aphrase;
  }
  virtual ~pfcp_msg_bad_length_exception() throw() {}
};

struct pfcp_msg_unimplemented_ie_exception : public pfcp_exception {
 public:
  pfcp_msg_unimplemented_ie_exception(
      const uint8_t msg_type, const uint16_t ie_type,
      const uint8_t instance = 0) throw() {
    phrase = fmt::format(
        "PFCP msg {} Unimplemented {} IE Instance {} Exception", msg_type,
        ie_type, instance);
  }
  pfcp_msg_unimplemented_ie_exception(std::string& aphrase) throw() {
    phrase = aphrase;
  }
  virtual ~pfcp_msg_unimplemented_ie_exception() throw() {}
};

struct pfcp_msg_illegal_ie_exception : public pfcp_exception {
 public:
  pfcp_msg_illegal_ie_exception(
      const uint8_t msg_type, const uint16_t ie_type, const char* file,
      const int line) throw() {
    phrase = fmt::format(
        "PFCP msg {} Illegal IE {} Exception {}:{}", msg_type, ie_type, file,
        line);
  }
  pfcp_msg_illegal_ie_exception(std::string& aphrase) throw() {
    phrase = aphrase;
  }
  virtual ~pfcp_msg_illegal_ie_exception() throw() {}
};

struct pfcp_ie_exception : public pfcp_exception {
 public:
  pfcp_ie_exception(uint16_t ie_type) throw() {
    phrase = fmt::format("PFCP IE {} Exception", ie_type);
  }
  pfcp_ie_exception(std::string& aphrase) throw() { phrase = aphrase; }
  virtual ~pfcp_ie_exception() throw() {}
};

struct pfcp_ie_unimplemented_exception : public pfcp_ie_exception {
 public:
  pfcp_ie_unimplemented_exception(uint16_t ie_type) throw()
      : pfcp_ie_exception(ie_type) {
    phrase = fmt::format("PFCP IE {} Unimplemented Exception", ie_type);
  }
  virtual ~pfcp_ie_unimplemented_exception() throw() {}
};

struct pfcp_tlv_exception : public pfcp_ie_exception {
 public:
  pfcp_tlv_exception(uint16_t ie_type) throw() : pfcp_ie_exception(ie_type) {
    phrase = fmt::format("PFCP IE TLV {} Exception", ie_type);
  }
  virtual ~pfcp_tlv_exception() throw() {}
};

struct pfcp_tlv_bad_length_exception : public pfcp_tlv_exception {
 public:
  pfcp_tlv_bad_length_exception(
      uint16_t ie_type, uint16_t ie_length, const char* file,
      const int line) throw()
      : pfcp_tlv_exception(ie_type) {
    phrase = fmt::format(
        "PFCP IE TLV {} Bad Length {} Exception {}:{}", ie_type, ie_length,
        file, line);
  }
  virtual ~pfcp_tlv_bad_length_exception() throw() {}
};

struct pfcp_ie_value_exception : public pfcp_ie_exception {
 public:
  pfcp_ie_value_exception(uint16_t ie_type, const char* field) throw()
      : pfcp_ie_exception(ie_type) {
    phrase =
        fmt::format("PFCP IE {} Bad Value of {} Exception", ie_type, field);
  }
  virtual ~pfcp_ie_value_exception() throw() {}
};

#define PFCP_IE_CREATE_PDR (1)
#define PFCP_IE_PDI (2)
#define PFCP_IE_CREATE_FAR (3)
#define PFCP_IE_FORWARDING_PARAMETERS (4)
#define PFCP_IE_DUPLICATING_PARAMETERS (5)
#define PFCP_IE_CREATE_URR (6)
#define PFCP_IE_CREATE_QER (7)
#define PFCP_IE_CREATED_PDR (8)
#define PFCP_IE_UPDATE_PDR (9)
#define PFCP_IE_UPDATE_FAR (10)
#define PFCP_IE_UPDATE_FORWARDING_PARAMETERS (11)
#define PFCP_IE_UPDATE_BAR_WITHIN_PFCP_SESSION_REPORT_RESPONSE (12)
#define PFCP_IE_UPDATE_URR (13)
#define PFCP_IE_UPDATE_QER (14)
#define PFCP_IE_REMOVE_PDR (15)
#define PFCP_IE_REMOVE_FAR (16)
#define PFCP_IE_REMOVE_URR (17)
#define PFCP_IE_REMOVE_QER (18)
#define PFCP_IE_CAUSE (19)
#define PFCP_IE_SOURCE_INTERFACE (20)
#define PFCP_IE_F_TEID (21)
#define PFCP_IE_NETWORK_INSTANCE (22)
#define PFCP_IE_SDF_FILTER (23)
#define PFCP_IE_APPLICATION_ID (24)
#define PFCP_IE_GATE_STATUS (25)
#define PFCP_IE_MBR (26)
#define PFCP_IE_GBR (27)
#define PFCP_IE_QER_CORRELATION_ID (28)
#define PFCP_IE_PRECEDENCE (29)
#define PFCP_IE_TRANSPORT_LEVEL_MARKING (30)
#define PFCP_IE_VOLUME_THRESHOLD (31)
#define PFCP_IE_TIME_THRESHOLD (32)
#define PFCP_IE_MONITORING_TIME (33)
#define PFCP_IE_SUBSEQUENT_VOLUME_THRESHOLD (34)
#define PFCP_IE_SUBSEQUENT_TIME_THRESHOLD (35)
#define PFCP_IE_INACTIVITY_DETECTION_TIME (36)
#define PFCP_IE_REPORTING_TRIGGERS (37)
#define PFCP_IE_REDIRECT_INFORMATION (38)
#define PFCP_IE_REPORT_TYPE (39)
#define PFCP_IE_OFFENDING_IE (40)
#define PFCP_IE_FORWARDING_POLICY (41)
#define PFCP_IE_DESTINATION_INTERFACE (42)
#define PFCP_IE_UP_FUNCTION_FEATURES (43)
#define PFCP_IE_APPLY_ACTION (44)
#define PFCP_IE_DOWNLINK_DATA_SERVICE_INFORMATION (45)
#define PFCP_IE_DOWNLINK_DATA_NOTIFICATION_DELAY (46)
#define PFCP_IE_DL_BUFFERING_DURATION (47)
#define PFCP_IE_DL_BUFFERING_SUGGESTED_PACKET_COUNT (48)
#define PFCP_IE_PFCPSMREQ_FLAGS (49)
#define PFCP_IE_PFCPSRRSP_FLAGS (50)
#define PFCP_IE_LOAD_CONTROL_INFORMATION (51)
#define PFCP_IE_SEQUENCE_NUMBER (52)
#define PFCP_IE_METRIC (53)
#define PFCP_IE_OVERLOAD_CONTROL_INFORMATION (54)
#define PFCP_IE_TIMER (55)
#define PFCP_IE_PACKET_DETECTION_RULE_ID (56)
#define PFCP_IE_F_SEID (57)
#define PFCP_IE_APPLICATION_IDS_PFDS (58)
#define PFCP_IE_PFD (59)
#define PFCP_IE_NODE_ID (60)
#define PFCP_IE_PFD_CONTENTS (61)
#define PFCP_IE_MEASUREMENT_METHOD (62)
#define PFCP_IE_USAGE_REPORT_TRIGGER (63)
#define PFCP_IE_MEASUREMENT_PERIOD (64)
#define PFCP_IE_FQ_CSID (65)
#define PFCP_IE_VOLUME_MEASUREMENT (66)
#define PFCP_IE_DURATION_MEASUREMENT (67)
#define PFCP_IE_APPLICATION_DETECTION_INFORMATION (68)
#define PFCP_IE_TIME_OF_FIRST_PACKET (69)
#define PFCP_IE_TIME_OF_LAST_PACKET (70)
#define PFCP_IE_QUOTA_HOLDING_TIME (71)
#define PFCP_IE_DROPPED_DL_TRAFFIC_THRESHOLD (72)
#define PFCP_IE_VOLUME_QUOTA (73)
#define PFCP_IE_TIME_QUOTA (74)
#define PFCP_IE_START_TIME (75)
#define PFCP_IE_END_TIME (76)
#define PFCP_IE_QUERY_URR (77)
#define PFCP_IE_USAGE_REPORT_WITHIN_SESSION_MODIFICATION_RESPONSE (78)
#define PFCP_IE_USAGE_REPORT_WITHIN_SESSION_DELETION_RESPONSE (79)
#define PFCP_IE_USAGE_REPORT_WITHIN_SESSION_REPORT_REQUEST (80)
#define PFCP_IE_URR_ID (81)
#define PFCP_IE_LINKED_URR_ID (82)
#define PFCP_IE_DOWNLINK_DATA_REPORT (83)
#define PFCP_IE_OUTER_HEADER_CREATION (84)
#define PFCP_IE_CREATE_BAR (85)
#define PFCP_IE_UPDATE_BAR_WITHIN_PFCP_SESSION_MODIFICATION_REQUEST (86)
#define PFCP_IE_REMOVE_BAR (87)
#define PFCP_IE_BAR_ID (88)
#define PFCP_IE_CP_FUNCTION_FEATURES (89)
#define PFCP_IE_USAGE_INFORMATION (90)
#define PFCP_IE_APPLICATION_INSTANCE_ID (91)
#define PFCP_IE_FLOW_INFORMATION (92)
#define PFCP_IE_UE_IP_ADDRESS (93)
#define PFCP_IE_PACKET_RATE (94)
#define PFCP_IE_OUTER_HEADER_REMOVAL (95)
#define PFCP_IE_RECOVERY_TIME_STAMP (96)
#define PFCP_IE_DL_FLOW_LEVEL_MARKING (97)
#define PFCP_IE_HEADER_ENRICHMENT (98)
#define PFCP_IE_ERROR_INDICATION_REPORT (99)
#define PFCP_IE_MEASUREMENT_INFORMATION (100)
#define PFCP_IE_NODE_REPORT_TYPE (101)
#define PFCP_IE_USER_PLANE_PATH_FAILURE_REPORT (102)
#define PFCP_IE_REMOTE_GTP_U_PEER (103)
#define PFCP_IE_UR_SEQN (104)
#define PFCP_IE_UPDATE_DUPLICATING_PARAMETERS (105)
#define PFCP_IE_ACTIVATE_PREDEFINED_RULES (106)
#define PFCP_IE_DEACTIVATE_PREDEFINED_RULES (107)
#define PFCP_IE_FAR_ID (108)
#define PFCP_IE_QER_ID (109)
#define PFCP_IE_OCI_FLAGS (110)
#define PFCP_IE_PFCP_ASSOCIATION_RELEASE_REQUEST (111)
#define PFCP_IE_GRACEFUL_RELEASE_PERIOD (112)
#define PFCP_IE_PDN_TYPE (113)
#define PFCP_IE_FAILED_RULE_ID (114)
#define PFCP_IE_TIME_QUOTA_MECHANISM (115)

#define PFCP_IE_USER_PLANE_IP_RESOURCE_INFORMATION (116)

#define PFCP_IE_USER_PLANE_INACTIVITY_TIMER (117)
#define PFCP_IE_AGGREGATED_URRS (118)
#define PFCP_IE_MULTIPLIER (119)
#define PFCP_IE_AGGREGATED_URR_ID (120)
#define PFCP_IE_SUBSEQUENT_VOLUME_QUOTA (121)
#define PFCP_IE_SUBSEQUENT_TIME_QUOTA (122)
#define PFCP_IE_RQI (123)
#define PFCP_IE_QFI (124)
#define PFCP_IE_QUERY_URR_REFERENCE (125)
#define PFCP_IE_ADDITIONAL_USAGE_REPORTS_INFORMATION (126)
#define PFCP_IE_CREATE_TRAFFIC_ENDPOINT (127)
#define PFCP_IE_CREATED_TRAFFIC_ENDPOINT (128)
#define PFCP_IE_UPDATE_TRAFFIC_ENDPOINT (129)
#define PFCP_IE_REMOVE_TRAFFIC_ENDPOINT (130)
#define PFCP_IE_TRAFFIC_ENDPOINT_ID (131)
#define PFCP_IE_ETHERNET_PACKET_FILTER (132)
#define PFCP_IE_MAC_ADDRESS (133)
#define PFCP_IE_C_TAG (134)
#define PFCP_IE_S_TAG (135)
#define PFCP_IE_ETHERTYPE (136)
#define PFCP_IE_PROXYING (137)
#define PFCP_IE_ETHERNET_FILTER_ID (138)
#define PFCP_IE_ETHERNET_FILTER_PROPERTIES (139)
#define PFCP_IE_SUGGESTED_BUFFERING_PACKETS_COUNT (140)
#define PFCP_IE_USER_ID (141)
#define PFCP_IE_ETHERNET_PDU_SESSION_INFORMATION (142)
#define PFCP_IE_ETHERNET_TRAFFIC_INFORMATION (143)
#define PFCP_IE_MAC_ADDRESSES_DETECTED (144)
#define PFCP_IE_MAC_ADDRESSES_REMOVED (145)
#define PFCP_IE_ETHERNET_INACTIVITY_TIMER (146)
#define PFCP_IE_ADDITIONAL_MONITORING_TIME (147)
#define PFCP_IE_EVENT_QUOTA (148)
#define PFCP_IE_EVENT_THRESHOLD (149)
#define PFCP_IE_SUBSEQUENT_EVENT_QUOTA (150)
#define PFCP_IE_SUBSEQUENT_EVENT_THRESHOLD (151)
#define PFCP_IE_TRACE_INFORMATION (152)
#define PFCP_IE_FRAMED_ROUTE (153)
#define PFCP_IE_FRAMED_ROUTING (154)
#define PFCP_IE_FRAMED_IPV6_ROUTE (155)
#define PFCP_IE_TIME_STAMP (156)
#define PFCP_IE_AVERAGING_WINDOW (157)
#define PFCP_IE_PAGING_POLICY_INDICATOR (158)
#define PFCP_IE_APN_DNN (159)
#define PFCP_IE_3GPP_INTERFACE_TYPE (160)

#define PFCP_IE_ENTERPRISE_SPECIFIC_RANGE_START (32768)
#define PFCP_IE_ENTERPRISE_SPECIFIC_RANGE_END (65535)
#define OAI_PRIVATE_ENTERPRISE_NUMBER (56701)

// ---- Rel-17 IE types 161..321 (TS 29.244 Table 8.1.2-1) ----
#define PFCP_IE_PFCPSRREQ_FLAGS (161)
// Backward-compat alias (previous non-spec spelling):
#define PFCP_IE_PFCPSRREQ_FLAGS_3GPP (161)
#define PFCP_IE_PFCPAUREQ_FLAGS (162)
#define PFCP_IE_ACTIVATION_TIME (163)
#define PFCP_IE_DEACTIVATION_TIME (164)
#define PFCP_IE_CREATE_MAR (165)
#define PFCP_IE_3GPP_ACCESS_FORWARDING_ACTION_INFORMATION (166)
#define PFCP_IE_NON_3GPP_ACCESS_FORWARDING_ACTION_INFORMATION (167)
// Backward-compat aliases (previous spelling):
#define PFCP_IE_ACCESS_FORWARDING_ACTION_INFORMATION_1 (166)
#define PFCP_IE_ACCESS_FORWARDING_ACTION_INFORMATION_2 (167)
#define PFCP_IE_REMOVE_MAR (168)
#define PFCP_IE_UPDATE_MAR (169)
#define PFCP_IE_MAR_ID (170)
#define PFCP_IE_STEERING_FUNCTIONALITY (171)
#define PFCP_IE_STEERING_MODE (172)
#define PFCP_IE_WEIGHT (173)
#define PFCP_IE_PRIORITY (174)
#define PFCP_IE_UPDATE_3GPP_ACCESS_FORWARDING_ACTION_INFORMATION (175)
#define PFCP_IE_UPDATE_NON_3GPP_ACCESS_FORWARDING_ACTION_INFORMATION (176)
// Backward-compat aliases (previous spelling):
#define PFCP_IE_UPDATE_ACCESS_FORWARDING_ACTION_INFORMATION_1 (175)
#define PFCP_IE_UPDATE_ACCESS_FORWARDING_ACTION_INFORMATION_2 (176)
#define PFCP_IE_UE_IP_ADDRESS_POOL_IDENTITY (177)
#define PFCP_IE_ALTERNATIVE_SMF_IP_ADDRESS (178)
// 179 is NOT spare in Rel-17: it is "Packet Replication and Detection
// Carry-On Information" (was previously mislabeled PFCP_IE_SPARED).
#define PFCP_IE_PACKET_REPLICATION_AND_DETECTION_CARRY_ON_INFORMATION (179)
#define PFCP_IE_SMF_SET_ID (180)
#define PFCP_IE_QUOTA_VALIDITY_TIME (181)
#define PFCP_IE_NUMBER_OF_REPORTS (182)
#define PFCP_IE_PFCP_SESSION_RETENTION_INFORMATION_WITHIN_ASSOCIATION_SETUP_REQUEST \
  (183)
#define PFCP_IE_PFCPASRSP_FLAGS (184)
#define PFCP_IE_CP_PFCP_ENTITY_IP_ADDRESS (185)
#define PFCP_IE_PFCPSEREQ_FLAGS (186)
#define PFCP_IE_USER_PLANE_PATH_RECOVERY_REPORT (187)
#define PFCP_IE_IP_MULTICAST_ADDRESSING_INFO_WITHIN_SESSION_ESTABLISHMENT_REQUEST \
  (188)
#define PFCP_IE_JOIN_IP_MULTICAST_INFORMATION_WITHIN_USAGE_REPORT (189)
#define PFCP_IE_LEAVE_IP_MULTICAST_INFORMATION_WITHIN_USAGE_REPORT (190)
#define PFCP_IE_IP_MULTICAST_ADDRESS (191)
#define PFCP_IE_SOURCE_IP_ADDRESS (192)
#define PFCP_IE_PACKET_RATE_STATUS (193)
#define PFCP_IE_CREATE_BRIDGE_INFO_FOR_TSC (194)
#define PFCP_IE_CREATED_BRIDGE_INFO_FOR_TSC (195)
#define PFCP_IE_DS_TT_PORT_NUMBER (196)
#define PFCP_IE_NW_TT_PORT_NUMBER (197)
#define PFCP_IE_5GS_USER_PLANE_NODE (198)
#define PFCP_IE_TSC_MANAGEMENT_INFORMATION_WITHIN_SESSION_MODIFICATION_REQUEST \
  (199)
#define PFCP_IE_TSC_MANAGEMENT_INFORMATION_WITHIN_SESSION_MODIFICATION_RESPONSE \
  (200)
#define PFCP_IE_TSC_MANAGEMENT_INFORMATION_WITHIN_SESSION_REPORT_REQUEST (201)
#define PFCP_IE_PORT_MANAGEMENT_INFORMATION_CONTAINER (202)
#define PFCP_IE_CLOCK_DRIFT_CONTROL_INFORMATION (203)
#define PFCP_IE_REQUESTED_CLOCK_DRIFT_INFORMATION (204)
#define PFCP_IE_CLOCK_DRIFT_REPORT (205)
#define PFCP_IE_TIME_DOMAIN_NUMBER (206)
#define PFCP_IE_TIME_OFFSET_THRESHOLD (207)
#define PFCP_IE_CUMULATIVE_RATE_RATIO_THRESHOLD (208)
#define PFCP_IE_TIME_OFFSET_MEASUREMENT (209)
#define PFCP_IE_CUMULATIVE_RATE_RATIO_MEASUREMENT (210)
#define PFCP_IE_REMOVE_SRR (211)
#define PFCP_IE_CREATE_SRR (212)
#define PFCP_IE_UPDATE_SRR (213)
#define PFCP_IE_SESSION_REPORT (214)
#define PFCP_IE_SRR_ID (215)
#define PFCP_IE_ACCESS_AVAILABILITY_CONTROL_INFORMATION (216)
#define PFCP_IE_REQUESTED_ACCESS_AVAILABILITY_INFORMATION (217)
#define PFCP_IE_ACCESS_AVAILABILITY_REPORT (218)
#define PFCP_IE_ACCESS_AVAILABILITY_INFORMATION (219)
#define PFCP_IE_PROVIDE_ATSSS_CONTROL_INFORMATION (220)
#define PFCP_IE_ATSSS_CONTROL_PARAMETERS (221)
#define PFCP_IE_MPTCP_CONTROL_INFORMATION (222)
#define PFCP_IE_ATSSS_LL_CONTROL_INFORMATION (223)
#define PFCP_IE_PMF_CONTROL_INFORMATION (224)
#define PFCP_IE_MPTCP_PARAMETERS (225)
#define PFCP_IE_ATSSS_LL_PARAMETERS (226)
#define PFCP_IE_PMF_PARAMETERS (227)
#define PFCP_IE_MPTCP_ADDRESS_INFORMATION (228)
#define PFCP_IE_UE_LINK_SPECIFIC_IP_ADDRESS (229)
#define PFCP_IE_PMF_ADDRESS_INFORMATION (230)
#define PFCP_IE_ATSSS_LL_INFORMATION (231)
#define PFCP_IE_DATA_NETWORK_ACCESS_IDENTIFIER (232)
#define PFCP_IE_UE_IP_ADDRESS_POOL_INFORMATION (233)
#define PFCP_IE_AVERAGE_PACKET_DELAY (234)
#define PFCP_IE_MINIMUM_PACKET_DELAY (235)
#define PFCP_IE_MAXIMUM_PACKET_DELAY (236)
#define PFCP_IE_QOS_REPORT_TRIGGER (237)
#define PFCP_IE_GTP_U_PATH_QOS_CONTROL_INFORMATION (238)
#define PFCP_IE_GTP_U_PATH_QOS_REPORT_WITHIN_NODE_REPORT_REQUEST (239)
#define PFCP_IE_QOS_INFORMATION_IN_GTP_U_PATH_QOS_REPORT (240)
#define PFCP_IE_GTP_U_PATH_INTERFACE_TYPE (241)
#define PFCP_IE_QOS_MONITORING_PER_QOS_FLOW_CONTROL_INFORMATION (242)
#define PFCP_IE_REQUESTED_QOS_MONITORING (243)
#define PFCP_IE_REPORTING_FREQUENCY (244)
#define PFCP_IE_PACKET_DELAY_THRESHOLDS (245)
#define PFCP_IE_MINIMUM_WAIT_TIME (246)
#define PFCP_IE_QOS_MONITORING_REPORT (247)
#define PFCP_IE_QOS_MONITORING_MEASUREMENT (248)
#define PFCP_IE_MT_EDT_CONTROL_INFORMATION (249)
#define PFCP_IE_DL_DATA_PACKETS_SIZE (250)
#define PFCP_IE_QER_CONTROL_INDICATIONS (251)
#define PFCP_IE_PACKET_RATE_STATUS_REPORT (252)
#define PFCP_IE_NF_INSTANCE_ID (253)
#define PFCP_IE_ETHERNET_CONTEXT_INFORMATION (254)
#define PFCP_IE_REDUNDANT_TRANSMISSION_PARAMETERS (255)
#define PFCP_IE_UPDATED_PDR (256)
#define PFCP_IE_S_NSSAI (257)
#define PFCP_IE_IP_VERSION (258)
#define PFCP_IE_PFCPASREQ_FLAGS (259)
#define PFCP_IE_DATA_STATUS (260)
#define PFCP_IE_PROVIDE_RDS_CONFIGURATION_INFORMATION (261)
#define PFCP_IE_RDS_CONFIGURATION_INFORMATION (262)
#define PFCP_IE_QUERY_PACKET_RATE_STATUS_WITHIN_SESSION_MODIFICATION_REQUEST   \
  (263)
#define PFCP_IE_PACKET_RATE_STATUS_REPORT_WITHIN_SESSION_MODIFICATION_RESPONSE \
  (264)
#define PFCP_IE_MPTCP_APPLICABLE_INDICATION (265)
#define PFCP_IE_BRIDGE_MANAGEMENT_INFORMATION_CONTAINER (266)
#define PFCP_IE_UE_IP_ADDRESS_USAGE_INFORMATION (267)
#define PFCP_IE_NUMBER_OF_UE_IP_ADDRESSES (268)
#define PFCP_IE_VALIDITY_TIMER (269)
#define PFCP_IE_REDUNDANT_TRANSMISSION_FORWARDING_PARAMETERS (270)
#define PFCP_IE_TRANSPORT_DELAY_REPORTING (271)
#define PFCP_IE_PARTIAL_FAILURE_INFORMATION (272)
#define PFCP_IE_SPARE (273)
#define PFCP_IE_OFFENDING_IE_INFORMATION (274)
#define PFCP_IE_RAT_TYPE (275)
#define PFCP_IE_L2TP_TUNNEL_INFORMATION (276)
#define PFCP_IE_L2TP_SESSION_INFORMATION (277)
#define PFCP_IE_L2TP_USER_AUTHENTICATION (278)
#define PFCP_IE_CREATED_L2TP_SESSION (279)
#define PFCP_IE_LNS_ADDRESS (280)
#define PFCP_IE_TUNNEL_PREFERENCE (281)
#define PFCP_IE_CALLING_NUMBER (282)
#define PFCP_IE_CALLED_NUMBER (283)
#define PFCP_IE_L2TP_SESSION_INDICATIONS (284)
#define PFCP_IE_DNS_SERVER_ADDRESS (285)
#define PFCP_IE_NBNS_SERVER_ADDRESS (286)
#define PFCP_IE_MAXIMUM_RECEIVE_UNIT (287)
#define PFCP_IE_THRESHOLDS (288)
#define PFCP_IE_STEERING_MODE_INDICATOR (289)
#define PFCP_IE_PFCP_SESSION_CHANGE_INFO (290)
#define PFCP_IE_GROUP_ID (291)
#define PFCP_IE_CP_IP_ADDRESS (292)
#define PFCP_IE_IP_ADDRESS_AND_PORT_NUMBER_REPLACEMENT (293)
#define PFCP_IE_DNS_QUERY_FILTER (294)
#define PFCP_IE_DIRECT_REPORTING_INFORMATION (295)
#define PFCP_IE_EVENT_NOTIFICATION_URI (296)
#define PFCP_IE_NOTIFICATION_CORRELATION_ID (297)
#define PFCP_IE_REPORTING_FLAGS (298)
#define PFCP_IE_PREDEFINED_RULES_NAME (299)
#define PFCP_IE_MBS_SESSION_N4MB_CONTROL_INFORMATION (300)
#define PFCP_IE_MBS_MULTICAST_PARAMETERS (301)
#define PFCP_IE_ADD_MBS_UNICAST_PARAMETERS (302)
#define PFCP_IE_MBS_SESSION_N4MB_INFORMATION (303)
#define PFCP_IE_REMOVE_MBS_UNICAST_PARAMETERS (304)
#define PFCP_IE_MBS_SESSION_IDENTIFIER (305)
#define PFCP_IE_MULTICAST_TRANSPORT_INFORMATION (306)
#define PFCP_IE_MBSN4MBREQ_FLAGS (307)
#define PFCP_IE_LOCAL_INGRESS_TUNNEL (308)
#define PFCP_IE_MBS_UNICAST_PARAMETERS_ID (309)
#define PFCP_IE_MBS_SESSION_N4_CONTROL_INFORMATION (310)
#define PFCP_IE_MBS_SESSION_N4_INFORMATION (311)
#define PFCP_IE_MBSN4RESP_FLAGS (312)
#define PFCP_IE_TUNNEL_PASSWORD (313)
#define PFCP_IE_AREA_SESSION_ID (314)
#define PFCP_IE_PEER_UP_RESTART_REPORT (315)
#define PFCP_IE_DSCP_TO_PPI_CONTROL_INFORMATION (316)
#define PFCP_IE_DSCP_TO_PPI_MAPPING_INFORMATION (317)
#define PFCP_IE_PFCPSDRSP_FLAGS (318)
#define PFCP_IE_QER_INDICATIONS (319)
#define PFCP_IE_VENDOR_SPECIFIC_NODE_REPORT_TYPE (320)
#define PFCP_IE_CONFIGURED_TIME_DOMAIN (321)

#define PFCP_MESSAGE_RESERVED (0)

// PFCP_NODE_RELATED_MESSAGES
#define PFCP_HEARTBEAT_REQUEST (1)
#define PFCP_HEARTBEAT_RESPONSE (2)
#define PFCP_PFCP_PFD_MANAGEMENT_REQUEST (3)
#define PFCP_PFCP_PFD_MANAGEMENT_RESPONSE (4)
#define PFCP_ASSOCIATION_SETUP_REQUEST (5)
#define PFCP_ASSOCIATION_SETUP_RESPONSE (6)
#define PFCP_ASSOCIATION_UPDATE_REQUEST (7)
#define PFCP_ASSOCIATION_UPDATE_RESPONSE (8)
#define PFCP_ASSOCIATION_RELEASE_REQUEST (9)
#define PFCP_ASSOCIATION_RELEASE_RESPONSE (10)
#define PFCP_VERSION_NOT_SUPPORTED_RESPONSE (11)
#define PFCP_NODE_REPORT_REQUEST (12)
#define PFCP_NODE_REPORT_RESPONSE (13)
#define PFCP_SESSION_SET_DELETION_REQUEST (14)
#define PFCP_SESSION_SET_DELETION_RESPONSE (15)
#define PFCP_SESSION_SET_MODIFICATION_REQUEST (16)
#define PFCP_SESSION_SET_MODIFICATION_RESPONSE (17)
// 18 to 49 for future use

// PFCP_SESSION_RELATED_MESSAGES
#define PFCP_SESSION_ESTABLISHMENT_REQUEST (50)
#define PFCP_SESSION_ESTABLISHMENT_RESPONSE (51)
#define PFCP_SESSION_MODIFICATION_REQUEST (52)
#define PFCP_SESSION_MODIFICATION_RESPONSE (53)
#define PFCP_SESSION_DELETION_REQUEST (54)
#define PFCP_SESSION_DELETION_RESPONSE (55)
#define PFCP_SESSION_REPORT_REQUEST (56)
#define PFCP_SESSION_REPORT_RESPONSE (57)
// 58 to 99 for future use

// Other messages
// 100 to 255 for future use
}  // namespace pfcp

namespace pfcp {
//-------------------------------------
// 8.2.1 Cause
enum cause_value_e {
  /* Acceptance in a response (values 1..63) */
  CAUSE_VALUE_RESERVED                   = 0,
  CAUSE_VALUE_REQUEST_ACCEPTED           = 1,
  CAUSE_VALUE_MORE_USAGE_REPORT_TO_SEND  = 2,
  CAUSE_VALUE_REQUEST_PARTIALLY_ACCEPTED = 3,
  /* Rejection in a response (values 64..255) */
  CAUSE_VALUE_REQUEST_REJECTED                   = 64,
  CAUSE_VALUE_SESSION_CONTEXT_NOT_FOUND          = 65,
  CAUSE_VALUE_MANDATORY_IE_MISSING               = 66,
  CAUSE_VALUE_CONDITIONAL_IE_MISSING             = 67,
  CAUSE_VALUE_INVALID_LENGTH                     = 68,
  CAUSE_VALUE_MANDATORY_IE_INCORRECT             = 69,
  CAUSE_VALUE_INVALID_FORWARDING_POLICY          = 70,
  CAUSE_VALUE_INVALID_FTEID_ALLOCATION_OPTION    = 71,
  CAUSE_VALUE_NO_ESTABLISHED_PFCP_ASSOCIATION    = 72,
  CAUSE_VALUE_RULE_CREATION_MODIFICATION_FAILURE = 73,
  CAUSE_VALUE_PFCP_ENTITY_IN_CONGESTION          = 74,
  CAUSE_VALUE_NO_RESOURCES_AVAILABLE             = 75,
  CAUSE_VALUE_SERVICE_NOT_SUPPORTED              = 76,
  CAUSE_VALUE_SYSTEM_FAILURE                     = 77,
  CAUSE_VALUE_REDIRECTION_REQUESTED              = 78,
  CAUSE_VALUE_ALL_DYNAMIC_ADDRESSES_ARE_OCCUPIED = 79,
  CAUSE_VALUE_UNKNOWN_PRE_DEFINED_RULE           = 80,
  CAUSE_VALUE_UNKNOWN_APPLICATION_ID             = 81,
  CAUSE_VALUE_L2TP_TUNNEL_ESTABLISHMENT_FAILURE  = 82,
  CAUSE_VALUE_L2TP_SESSION_ESTABLISHMENT_FAILURE = 83,
  CAUSE_VALUE_L2TP_TUNNEL_RELEASE                = 84,
  CAUSE_VALUE_L2TP_SESSION_RELEASE               = 85,
  CAUSE_VALUE_PFCP_SESSION_RESTORATION_FAILURE   = 86
};

typedef struct cause_s {
  uint8_t cause_value;

  bool operator==(const struct cause_s& i) const {
    return (i.cause_value == cause_value);
  };
  bool operator==(const uint8_t& cvalue) const {
    return (cvalue == cause_value);
  };
} cause_t;

//-------------------------------------
// 8.2.2 Source Interface
// Table 8.2.2-1: Source Interface value set (DISTINCT from the Destination
// Interface value set in Table 8.2.24-1 -- they diverge at value 4).
enum source_interface_value_e {
  SOURCE_INTERFACE_VALUE_ACCESS         = 0,
  SOURCE_INTERFACE_VALUE_CORE           = 1,
  SOURCE_INTERFACE_VALUE_SGI_LAN_N6_LAN = 2,
  SOURCE_INTERFACE_VALUE_CP_FUNCTION    = 3,
  SOURCE_INTERFACE_VALUE_5G_VN_INTERNAL = 4  // NB: 4 = LI Function for dest.
};

typedef struct source_interface_s {
  // bit 1 first, ascending, spare last (whole-file convention)
  uint8_t interface_value : 4;
  uint8_t spare : 4;
  bool operator==(const struct source_interface_s& i) const {
    return (i.interface_value == interface_value);
  };
} source_interface_t;

//-------------------------------------
// 8.2.3 F-TEID
struct fteid_s {
  uint8_t v4 : 1;
  uint8_t v6 : 1;
  uint8_t ch : 1;
  uint8_t chid : 1;
  uint8_t spare : 4;
  teid_t teid;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
  uint8_t choose_id;

  bool operator==(const struct fteid_s& f) const {
    return (teid == f.teid) and
           (ipv4_address.s_addr == f.ipv4_address.s_addr) and
           (chid == f.chid) and (ch == f.ch) and (choose_id == f.choose_id) and
           (ipv6_address.s6_addr32[0] == f.ipv6_address.s6_addr32[0]) and
           (ipv6_address.s6_addr32[1] == f.ipv6_address.s6_addr32[1]) and
           (ipv6_address.s6_addr32[2] == f.ipv6_address.s6_addr32[2]) and
           (ipv6_address.s6_addr32[3] == f.ipv6_address.s6_addr32[3]) and
           (v4 == f.v4) and (v6 == f.v6);
  }

  fteid_s& operator=(const struct fteid_s& f) {
    v4                  = f.v4;
    v6                  = f.v6;
    chid                = f.chid;
    ch                  = f.ch;
    choose_id           = f.choose_id;
    teid                = f.teid;
    ipv4_address.s_addr = f.ipv4_address.s_addr;
    ipv6_address        = f.ipv6_address;
    // ipv6_address.s6_addr32[0] = f.ipv6_address.s6_addr32[0];
    // ipv6_address.s6_addr32[1] = f.ipv6_address.s6_addr32[1];
    // ipv6_address.s6_addr32[2] = f.ipv6_address.s6_addr32[2];
    // ipv6_address.s6_addr32[3] = f.ipv6_address.s6_addr32[3];
    return *this;
  }

  std::string toString() const {
    std::string s = {};
    if ((v4) || (v6)) {
      s.append("TEID=").append(std::to_string(teid));
      if (v4) {
        s.append(", IPv4=").append(oai::utils::conv::toString(ipv4_address));
      }
      if (v6) {
        s.append(", IPv6=").append(oai::utils::conv::toString(ipv6_address));
      }
    } else {
      s.append("null_fteid");
    }
    return s;
  }

  bool is_zero() const { return ((!v4) and (!v6)); }
};

typedef struct fteid_s fteid_t;
//-------------------------------------
// 8.2.4 Network Instance
typedef struct network_instance_s {
  std::string network_instance;
} network_instance_t;

//-------------------------------------
// 8.2.5 SDF Filter
typedef struct sdf_filter_s {
  uint8_t fd : 1;
  uint8_t ttc : 1;
  uint8_t spi : 1;
  uint8_t fl : 1;
  uint8_t bid : 1;
  uint8_t spare : 3;
  uint8_t spare2;  // octet 6 (dedicated Spare octet per Figure 8.2.5-1)
  uint16_t length_of_flow_description;
  std::string flow_description;
  std::string tos_traffic_class;         // 2 octets
  std::string security_parameter_index;  // 4 octets
  std::string flow_label;                // 3 octets
  uint32_t sdf_filter_id;
} sdf_filter_t;

//-------------------------------------
//  8.2.6 Application ID
typedef struct application_id_s {
  std::string application_id;
} application_id_t;

//-------------------------------------
//  8.2.7 Gate Status
enum gate_status_e {
  /* Request / Initial message */
  OPEN   = 0,
  CLOSED = 1
};
typedef struct gate_status_s {
  uint8_t dl_gate : 2;
  uint8_t ul_gate : 2;
  uint8_t spare : 4;
} gate_status_t;

//-------------------------------------
// 8.2.8 MBR
typedef struct mbr_s {
  uint64_t ul_mbr;
  uint64_t dl_mbr;
} mbr_t;

//-------------------------------------
// 8.2.9 GBR
typedef struct gbr_s {
  uint64_t ul_gbr;
  uint64_t dl_gbr;
} gbr_t;

//-------------------------------------
// 8.2.10 QER Correlation ID
typedef struct qer_correlation_id_s {
  uint32_t qer_correlation_id;
} qer_correlation_id_t;

//-------------------------------------
// 8.2.11 Precedence
typedef struct precedence_s {
  uint32_t precedence;
  bool operator==(const struct precedence_s& i) const {
    return (i.precedence == precedence);
  };
} precedence_t;

//-------------------------------------
// 8.2.12 Transport Level Marking
typedef struct transport_level_marking_s {
  std::string transport_level_marking;  // 2 octets
} transport_level_marking_t;

//-------------------------------------
// 8.2.13 Volume Threshold
typedef struct volume_threshold_s {
  uint8_t tovol : 1;
  uint8_t ulvol : 1;
  uint8_t dlvol : 1;
  uint8_t spare : 5;
  uint64_t total_volume;
  uint64_t uplink_volume;
  uint64_t downlink_volume;
} volume_threshold_t;

//-------------------------------------
// 8.2.14 Time Threshold
typedef struct time_threshold_s {
  uint32_t time_threshold;
  bool operator==(const struct time_threshold_s& i) const {
    return (i.time_threshold == time_threshold);
  };
} time_threshold_t;

//-------------------------------------
// 8.2.15 Monitoring Time
typedef struct monitoring_time_s {
  uint32_t monitoring_time;
} monitoring_time_t;

//-------------------------------------
// 8.2.16 Subsequent Volume Threshold
typedef struct subsequent_volume_threshold_s {
  uint8_t tovol : 1;
  uint8_t ulvol : 1;
  uint8_t dlvol : 1;
  uint8_t spare : 5;
  uint64_t total_volume;
  uint64_t uplink_volume;
  uint64_t downlink_volume;
} subsequent_volume_threshold_t;

//-------------------------------------
// 8.2.17 Subsequent Time Threshold
typedef struct subsequent_time_threshold_s {
  uint32_t subsequent_time_threshold;
  bool operator==(const struct subsequent_time_threshold_s& i) const {
    return (i.subsequent_time_threshold == subsequent_time_threshold);
  };
} subsequent_time_threshold_t;

//-------------------------------------
// 8.2.18 Inactivity Detection Time
typedef struct inactivity_detection_time_s {
  uint32_t inactivity_detection_time;
  bool operator==(const struct inactivity_detection_time_s& i) const {
    return (i.inactivity_detection_time == inactivity_detection_time);
  };
} inactivity_detection_time_t;

//-------------------------------------
// 8.2.19 Reporting Triggers
typedef struct reporting_triggers_s {
  // Octet 5 (bit 1 -> bit 8)
  uint8_t perio : 1;
  uint8_t volth : 1;
  uint8_t timth : 1;
  uint8_t quhti : 1;
  uint8_t start : 1;
  uint8_t stop : 1;  // STOPT
  uint8_t droth : 1;
  uint8_t liusa : 1;
  // Octet 6 (bit 1 -> bit 8)
  uint8_t volqu : 1;
  uint8_t timqu : 1;
  uint8_t envcl : 1;
  uint8_t macar : 1;
  uint8_t eveth : 1;
  uint8_t evequ : 1;
  uint8_t ipmjl : 1;
  uint8_t quvti : 1;
  // Octet 7 (bit 1 -> bit 8)
  uint8_t reemr : 1;
  uint8_t upint : 1;
  uint8_t spare : 6;
} reporting_triggers_t;

//-------------------------------------
// 8.2.20 Redirect Information
typedef struct redirect_information_s {
  uint8_t redirect_address_type : 4;
  uint8_t spare : 4;
  uint16_t redirect_server_address_length;
  std::string redirect_server_address;
  // Present when Redirect Address Type = "IPv4 and IPv6 addresses" (4):
  uint16_t other_redirect_server_address_length;
  std::string other_redirect_server_address;
  uint16_t redirect_port;  // added (Rel-17)
} redirect_information_t;

enum redirect_address_type_e {
  IPV4_ADDRESS                     = 0,
  IPV6_ADDRESS                     = 1,
  URL                              = 2,
  SIP_URI                          = 3,
  IPV4_AND_IPV6_ADDRESSES          = 4,
  PORT                             = 5,
  IPV4_ADDRESS_AND_PORT            = 6,
  IPV6_ADDRESS_AND_PORT            = 7,
  IPV4_AND_IPV6_ADDRESSES_AND_PORT = 8,
  // Spare for future use       9 to 15
};

//-------------------------------------
// 8.2.21 Report Type
typedef struct report_type_s {
  uint8_t dldr : 1;
  uint8_t usar : 1;
  uint8_t erir : 1;
  uint8_t upir : 1;
  uint8_t tmir : 1;  // added (Rel-16)
  uint8_t sesr : 1;  // added (Rel-16)
  uint8_t uisr : 1;  // added (Rel-16)
  uint8_t spare : 1;
} report_type_t;

//-------------------------------------
// 8.2.22 Offending IE
typedef struct offending_ie_s {
  uint16_t offending_ie;
  bool operator==(const struct offending_ie_s& i) const {
    return (i.offending_ie == offending_ie);
  };
} offending_ie_t;

//-------------------------------------
// 8.2.23 Forwarding Policy
typedef struct forwarding_policy_s {
  uint8_t forwarding_policy_identifier_length;
  std::string forwarding_policy_identifier;  // TODO CHECK TYPE
} forwarding_policy_t;

//-------------------------------------
// 8.2.24 Destination Interface
// Table 8.2.24-1: Destination Interface value set (differs from Source at 4/5)
enum destination_interface_value_e {
  INTERFACE_VALUE_ACCESS         = 0,
  INTERFACE_VALUE_CORE           = 1,
  INTERFACE_VALUE_SGI_LAN_N6_LAN = 2,
  INTERFACE_VALUE_CP_FUNCTION    = 3,
  INTERFACE_VALUE_LI_FUNCTION    = 4,
  INTERFACE_VALUE_5G_VN_INTERNAL = 5  // added (was missing)
};

typedef struct destination_interface_s {
  uint8_t interface_value : 4;
  uint8_t spare : 4;
  bool operator==(const struct destination_interface_s& i) const {
    return (i.interface_value == interface_value);
  };
} destination_interface_t;

//-------------------------------------
// 8.2.25 UP Function Features
struct up_function_features_s {
  uint16_t bucp : 1;
  uint16_t ddnd : 1;
  uint16_t dlbd : 1;
  uint16_t trst : 1;
  uint16_t ftup : 1;
  uint16_t pfdm : 1;
  uint16_t heeu : 1;
  uint16_t treu : 1;

  uint16_t empu : 1;
  uint16_t pdiu : 1;
  uint16_t udbc : 1;
  uint16_t quoac : 1;
  uint16_t trace : 1;
  uint16_t frrt : 1;
  //  uint16_t spare : 2;
  uint16_t pfde : 1;
  uint16_t epfar : 1;

  uint16_t dpdra : 1;
  uint16_t adpdp : 1;
  uint16_t ueip : 1;
  uint16_t sset : 1;
  uint8_t mnop : 1;
  uint8_t mte : 1;
  uint8_t bundl : 1;
  uint8_t gcom : 1;

  uint8_t mpas : 1;
  uint8_t rttl : 1;
  uint8_t vtime : 1;
  uint8_t norp : 1;
  uint8_t iptv : 1;
  uint8_t ip6pl : 1;
  uint8_t tscu : 1;  // 8/7: spec 17.10.0 renamed this bit "TSN" (kept as
                     // 'tscu' for encoder compatibility)
  uint8_t mptcp : 1;

  uint8_t atsss_ll : 1;
  uint8_t qfqm : 1;
  uint8_t gpqm : 1;
  uint8_t mt_edt : 1;
  uint8_t ciot : 1;
  uint8_t ethar : 1;
  uint8_t ddds : 1;
  uint8_t rds : 1;

  // Octet 10
  uint8_t rttwp : 1;  // 10/1
  uint8_t quasf : 1;  // 10/2
  uint8_t nspoc : 1;  // 10/3
  uint8_t l2tp : 1;   // 10/4
  uint8_t upber : 1;  // 10/5
  uint8_t resps : 1;  // 10/6
  uint8_t iprep : 1;  // 10/7
  uint8_t dnsts : 1;  // 10/8
  // Octet 11
  uint8_t drqos : 1;     // 11/1
  uint8_t mbsn4 : 1;     // 11/2
  uint8_t psuprm : 1;    // 11/3
  uint8_t epppi : 1;     // 11/4
  uint8_t ratp : 1;      // 11/5
  uint8_t upidp : 1;     // 11/6
  uint8_t spare_11 : 2;  // 11/7, 11/8 reserved
  // Octet 12
  uint8_t spare_12_1 : 1;  // 12/1 reserved
  uint8_t dbdm : 1;        // 12/2
  uint8_t tscts : 1;       // 12/3
  uint8_t spare : 5;       // 12/4..12/8 reserved

  // up_function_features_s& operator=(up_function_features_s i)
  //{
  //  ...
  //  return *this;
  //}
  up_function_features_s()
      : bucp(0),
        ddnd(0),
        dlbd(0),
        trst(0),
        ftup(0),
        pfdm(0),
        heeu(0),
        treu(0),

        empu(0),
        pdiu(0),
        udbc(0),
        quoac(0),
        trace(0),
        frrt(0),
        pfde(0),
        epfar(0),

        dpdra(0),
        adpdp(0),
        ueip(0),
        sset(0),
        mnop(0),
        mte(0),
        bundl(0),
        gcom(0),

        mpas(0),
        rttl(0),
        vtime(0),
        norp(0),
        iptv(0),
        ip6pl(0),
        tscu(0),
        mptcp(0),

        atsss_ll(0),
        qfqm(0),
        gpqm(0),
        mt_edt(0),
        ciot(0),
        ethar(0),
        ddds(0),
        rds(0),

        rttwp(0),
        quasf(0),
        nspoc(0),
        l2tp(0),
        upber(0),
        resps(0),
        iprep(0),
        dnsts(0),

        drqos(0),
        mbsn4(0),
        psuprm(0),
        epppi(0),
        ratp(0),
        upidp(0),
        spare_11(0),

        spare_12_1(0),
        dbdm(0),
        tscts(0),
        spare(0) {}

  up_function_features_s(const up_function_features_s& i) {
    bucp = i.bucp;
    ddnd = i.ddnd;
    dlbd = i.dlbd;
    trst = i.trst;
    ftup = i.ftup;
    pfdm = i.pfdm;
    heeu = i.heeu;
    treu = i.treu;

    empu  = i.empu;
    pdiu  = i.pdiu;
    udbc  = i.udbc;
    quoac = i.quoac;
    trace = i.trace;
    frrt  = i.frrt;
    spare = i.spare;
    epfar = i.epfar;
    pfde  = i.pfde;

    dpdra = i.dpdra;
    adpdp = i.adpdp;
    ueip  = i.ueip;
    sset  = i.sset;
    mnop  = i.mnop;
    mte   = i.mte;
    bundl = i.bundl;
    gcom  = i.gcom;

    mpas  = i.mpas;
    rttl  = i.rttl;
    vtime = i.vtime;
    norp  = i.norp;
    iptv  = i.iptv;
    ip6pl = i.ip6pl;
    tscu  = i.tscu;
    mptcp = i.mptcp;

    atsss_ll = i.atsss_ll;
    qfqm     = i.qfqm;
    gpqm     = i.gpqm;
    mt_edt   = i.mt_edt;
    ciot     = i.ciot;
    ethar    = i.ethar;
    ddds     = i.ddds;
    rds      = i.rds;

    rttwp = i.rttwp;

    quasf = i.quasf;
    nspoc = i.nspoc;
    l2tp  = i.l2tp;
    upber = i.upber;
    resps = i.resps;
    iprep = i.iprep;
    dnsts = i.dnsts;

    drqos    = i.drqos;
    mbsn4    = i.mbsn4;
    psuprm   = i.psuprm;
    epppi    = i.epppi;
    ratp     = i.ratp;
    upidp    = i.upidp;
    spare_11 = i.spare_11;

    spare_12_1 = i.spare_12_1;
    dbdm       = i.dbdm;
    tscts      = i.tscts;
  }
};
// typedef struct up_function_features_s up_function_features_t;

//-------------------------------------
// 8.2.26 Apply Action
typedef struct apply_action_s {
  // Octet 5 (spare 3 bits replaced by IPMA/IPMD/DFRT)
  uint8_t drop : 1;
  uint8_t forw : 1;
  uint8_t buff : 1;
  uint8_t nocp : 1;
  uint8_t dupl : 1;
  uint8_t ipma : 1;
  uint8_t ipmd : 1;
  uint8_t dfrt : 1;
  // Octet 6 (added in Rel-16/17)
  uint8_t edrt : 1;
  uint8_t bdpn : 1;
  uint8_t ddpn : 1;
  uint8_t fssm : 1;
  uint8_t mbsu : 1;
  uint8_t spare : 3;
} apply_action_t;

//-------------------------------------
// 8.2.27 Downlink Data Service Information
typedef struct downlink_data_service_information_s {
  uint8_t ppi : 1;
  uint8_t qfii : 1;
  uint8_t spare : 6;
  uint8_t Paging_Policy_Indication;
  uint8_t qfi;
} downlink_data_service_information_t;

//-------------------------------------
// 8.2.28 Downlink Data Notification Delay
typedef struct downlink_data_notification_delay_s {
  uint8_t delay;  // Delay Value in integer multiples of 50 millisecs, or zero
} downlink_data_notification_delay_t;

//-------------------------------------
// 8.2.29 DL Buffering Duration
typedef struct dl_buffering_duration_s {
  uint8_t timer_value : 5;
  uint8_t timer_unit : 3;
} dl_buffering_duration_t;

//-------------------------------------
// 8.2.30 DL Buffering Suggested Packet Count
typedef struct dl_buffering_suggested_packet_count_s {
  uint16_t packet_count;
} dl_buffering_suggested_packet_count_t;

//-------------------------------------
// 8.2.31 PFCPSMReq-Flags
typedef struct pfcpsmreq_flags_s {
  uint8_t drobu : 1;
  uint8_t sndem : 1;
  uint8_t qaurr : 1;
  uint8_t sumpc : 1;   // added (Rel-16)
  uint8_t rumuc : 1;   // added (Rel-16)
  uint8_t deteid : 1;  // added (Rel-17)
  uint8_t spare : 2;
} pfcpsmreq_flags_t;

//-------------------------------------
// 8.2.32 PFCPSRRsp-Flags
typedef struct pfcpsrrsp_flags_s {
  uint8_t drobu : 1;
  uint8_t spare : 7;
} pfcpsrrsp_flags_t;

//-------------------------------------
// 8.2.33 Sequence Number
typedef struct sequence_number_s {
  uint32_t sequence_number;
  bool operator==(const struct sequence_number_s& i) const {
    return (i.sequence_number == sequence_number);
  };
} sequence_number_t;

//-------------------------------------
// 8.2.34 Metric
typedef struct metric_s {
  uint8_t metric;  // It indicates a percentage and may take binary coded
                   // integer values from
  // and including 0 up to and including 100. Other values shall be considered
  // as 0.
  bool operator==(const struct metric_s& i) const {
    return (i.metric == metric);
  };
} metric_t;

//-------------------------------------
// 8.2.35 Timer
typedef struct timer_s {
  uint8_t timer_value : 5;
  uint8_t timer_unit : 3;
} timer_t;

//-------------------------------------
// 8.2.36 Packet Detection Rule ID (PDR ID)
typedef struct pdr_id_s {
  uint16_t rule_id;
  pdr_id_s() : rule_id(0) {}
  pdr_id_s(const uint8_t& p) : rule_id(p) {}
  pdr_id_s(const struct pdr_id_s& p) : rule_id(p.rule_id) {}
  bool operator==(const struct pdr_id_s& i) const {
    return (i.rule_id == rule_id);
  };
} pdr_id_t;

//-------------------------------------
// 8.2.37 F-SEID
typedef struct fseid_s {
  uint8_t v6 : 1;
  uint8_t v4 : 1;
  uint8_t spare : 6;
  uint64_t seid;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;

  bool operator==(const struct fseid_s& i) const {
    if ((i.seid == this->seid) && (i.v4 == this->v4) &&
        (i.ipv4_address.s_addr == this->ipv4_address.s_addr) &&
        (i.v6 == this->v6) &&
        (i.ipv6_address.s6_addr32[0] == this->ipv6_address.s6_addr32[0]) &&
        (i.ipv6_address.s6_addr32[1] == this->ipv6_address.s6_addr32[1]) &&
        (i.ipv6_address.s6_addr32[2] == this->ipv6_address.s6_addr32[2]) &&
        (i.ipv6_address.s6_addr32[3] == this->ipv6_address.s6_addr32[3])) {
      return true;
    } else {
      return false;
    }
  };
  bool operator<(const struct fseid_s& i) const {
    if (i.seid < this->seid)
      return true;
    else if (i.seid > this->seid)
      return false;
    if (i.v4 == this->v4) {
      if (i.ipv4_address.s_addr > this->ipv4_address.s_addr)
        return true;
      else if (i.ipv4_address.s_addr < this->ipv4_address.s_addr)
        return false;
    } else if (this->v4)
      return true;
    if (i.v6 == this->v6) {
      uint64_t i64 = ((uint64_t) i.ipv6_address.s6_addr32[0] << 32) |
                     ((uint64_t) i.ipv6_address.s6_addr32[1]);
      uint64_t this64 = ((uint64_t) this->ipv6_address.s6_addr32[0] << 32) |
                        ((uint64_t) this->ipv6_address.s6_addr32[1]);

      if (i64 < this64)
        return true;
      else if (i64 > this64)
        return false;
      i64 = ((uint64_t) i.ipv6_address.s6_addr32[2] << 32) |
            ((uint64_t) i.ipv6_address.s6_addr32[3]);
      this64 = ((uint64_t) this->ipv6_address.s6_addr32[2] << 32) |
               ((uint64_t) this->ipv6_address.s6_addr32[3]);
      if (i64 < this64)
        return true;
      else if (i64 > this64)
        return false;
    } else if (this->v6)
      return true;
    return false;
  };
} fseid_t;

//-------------------------------------
// 8.2.38 Node ID
enum node_id_type_value_e {
  /* Request / Initial message */
  NODE_ID_TYPE_IPV4_ADDRESS = 0,
  NODE_ID_TYPE_IPV6_ADDRESS = 1,
  NODE_ID_TYPE_FQDN         = 2,
  NODE_ID_TYPE_UNKNOWN      = 3
};

struct node_id_s {
  uint8_t node_id_type : 4;
  uint8_t spare : 4;
  union {
    struct in_addr ipv4_address;
    struct in6_addr ipv6_address;
  } u1;
  std::string fqdn;  // should be in union but problem with virtual ~
  node_id_s() {
    node_id_type           = node_id_type_value_e::NODE_ID_TYPE_UNKNOWN;
    u1.ipv4_address.s_addr = INADDR_ANY;
    u1.ipv6_address        = in6addr_any;
    fqdn                   = {};
  }

  bool operator==(const struct node_id_s& i) const {
    if (i.node_id_type != this->node_id_type) return false;
    switch (i.node_id_type) {
      case NODE_ID_TYPE_IPV4_ADDRESS: {
        if (i.u1.ipv4_address.s_addr == this->u1.ipv4_address.s_addr)
          return true;
      } break;
      case NODE_ID_TYPE_IPV6_ADDRESS: {
        if ((i.u1.ipv6_address.s6_addr32[0] ==
             this->u1.ipv6_address.s6_addr32[0]) &&
            (i.u1.ipv6_address.s6_addr32[1] ==
             this->u1.ipv6_address.s6_addr32[1]) &&
            (i.u1.ipv6_address.s6_addr32[2] ==
             this->u1.ipv6_address.s6_addr32[2]) &&
            (i.u1.ipv6_address.s6_addr32[3] ==
             this->u1.ipv6_address.s6_addr32[3]))
          return true;
      } break;
      case NODE_ID_TYPE_FQDN: {
        if (i.fqdn == this->fqdn) return true;
      } break;
      default: {
        return false;
      }
    }
    return false;

    /*
        if ((i.node_id_type == this->node_id_type) &&
            (i.u1.ipv4_address.s_addr == this->u1.ipv4_address.s_addr) &&
            (i.fqdn == this->fqdn) &&
            (i.u1.ipv6_address.s6_addr32[0] ==
             this->u1.ipv6_address.s6_addr32[0]) &&
            (i.u1.ipv6_address.s6_addr32[1] ==
             this->u1.ipv6_address.s6_addr32[1]) &&
            (i.u1.ipv6_address.s6_addr32[2] ==
             this->u1.ipv6_address.s6_addr32[2]) &&
            (i.u1.ipv6_address.s6_addr32[3] ==
             this->u1.ipv6_address.s6_addr32[3])) {
          return true;
        } else {
          return false;
        }
        */
  };
  bool operator==(const std::string& f) const {
    if ((NODE_ID_TYPE_FQDN == this->node_id_type) && (fqdn.compare(f) == 0)) {
      return true;
    } else {
      return false;
    }
  };
  bool operator==(const struct in_addr& a) const {
    if ((NODE_ID_TYPE_IPV4_ADDRESS == this->node_id_type) &&
        (a.s_addr == u1.ipv4_address.s_addr)) {
      return true;
    } else {
      return false;
    }
  };
  bool operator==(const struct in6_addr& i) const {
    if ((NODE_ID_TYPE_IPV6_ADDRESS == this->node_id_type) &&
        (i.s6_addr32[0] == this->u1.ipv6_address.s6_addr32[0]) &&
        (i.s6_addr32[1] == this->u1.ipv6_address.s6_addr32[1]) &&
        (i.s6_addr32[2] == this->u1.ipv6_address.s6_addr32[2]) &&
        (i.s6_addr32[3] == this->u1.ipv6_address.s6_addr32[3])) {
      return true;
    } else {
      return false;
    }
  };

  node_id_s& operator=(const struct node_id_s& i) {
    node_id_type                 = i.node_id_type;
    fqdn                         = i.fqdn;
    u1.ipv4_address.s_addr       = i.u1.ipv4_address.s_addr;
    u1.ipv6_address.s6_addr32[0] = i.u1.ipv6_address.s6_addr32[0];
    u1.ipv6_address.s6_addr32[1] = i.u1.ipv6_address.s6_addr32[1];
    u1.ipv6_address.s6_addr32[2] = i.u1.ipv6_address.s6_addr32[2];
    u1.ipv6_address.s6_addr32[3] = i.u1.ipv6_address.s6_addr32[3];

    return *this;
  }

  std::string toString() const {
    if (NODE_ID_TYPE_FQDN == this->node_id_type) {
      return fqdn;
    }
    if (NODE_ID_TYPE_IPV4_ADDRESS == this->node_id_type) {
      return oai::utils::conv::toString(u1.ipv4_address);
    } else if (NODE_ID_TYPE_IPV6_ADDRESS == this->node_id_type) {
      return oai::utils::conv::toString(u1.ipv6_address);
    }
    return std::string("Node id - unknown node id type");
  }
};
typedef struct node_id_s node_id_t;

//-------------------------------------
// 8.2.39 PFD Contents
// One (Domain Name, Domain Name Protocol) instance of the Additional Domain
// Name and Domain Name Protocol field (Figure 8.2.39-4).
typedef struct pfd_domain_name_and_protocol_s {
  std::string domain_name;
  std::string domain_name_protocol;
} pfd_domain_name_and_protocol_t;
typedef struct pfd_contents_s {
  uint8_t fd : 1;
  uint8_t url_ : 1;
  uint8_t dn : 1;
  uint8_t cp : 1;
  uint8_t dnp : 1;   // added (Rel-16): Domain Name Protocol present
  uint8_t afd : 1;   // added (Rel-16): Additional Flow Description present
  uint8_t aurl : 1;  // added (Rel-16): Additional URL present
  uint8_t adnp : 1;  // added (Rel-16): Additional Domain Name/Protocol present
  uint8_t spare;     // octet 6 (dedicated Spare octet per Figure 8.2.39-1)
  uint16_t length_of_flow_description;
  std::string flow_description;
  uint16_t length_of_url;
  std::string url;
  uint16_t length_of_domain_name;
  std::string domain_name;
  uint16_t length_of_custom_pfd_contents;
  std::string custom_pfd_contents;
  // Added (Rel-16):
  uint16_t length_of_domain_name_protocol;  // if DNP
  std::string domain_name_protocol;
  // The "Additional" fields carry MULTIPLE instances (Figures 8.2.39-2/-3/-4),
  // i.e. the 2nd, 3rd ... m-th Flow Description / URL / (Domain Name +
  // Protocol). On the wire each block is prefixed by an overall length (the
  // uint16_t below) and then contains a sequence of length-prefixed instances
  // (held in the vectors).
  // if AFD: Flow Description 2..m (Figure 8.2.39-2)
  uint16_t length_of_additional_flow_description;
  std::vector<std::string> additional_flow_descriptions;
  // if AURL: URL 2..m (Figure 8.2.39-3)
  uint16_t length_of_additional_url;
  std::vector<std::string> additional_urls;
  // if ADNP: (Domain Name, Domain Name Protocol) 2..m (Figure 8.2.39-4)
  uint16_t length_of_additional_domain_name_and_domain_name_protocol;
  std::vector<pfd_domain_name_and_protocol_t>
      additional_domain_names_and_protocols;
} pfd_contents_t;

//-------------------------------------
// 8.2.40 Measurement Method
typedef struct measurement_method_s {
  uint16_t durat : 1;
  uint16_t volum : 1;
  uint16_t event : 1;
  uint16_t spare : 5;
} measurement_method_t;

//-------------------------------------
// 8.2.41 Usage Report Trigger
typedef struct usage_report_trigger_s {
  // Octet 5 (bit 1 -> bit 8)
  uint16_t perio : 1;
  uint16_t volth : 1;
  uint16_t timth : 1;
  uint16_t quhti : 1;
  uint16_t start : 1;
  uint16_t stop : 1;
  uint16_t droth : 1;
  uint16_t immer : 1;
  // Octet 6 (bit 1 -> bit 8)
  uint16_t volqu : 1;
  uint16_t timqu : 1;
  uint16_t liusa : 1;
  uint16_t termr : 1;
  uint16_t monit : 1;
  uint16_t envcl : 1;
  uint16_t macar : 1;
  uint16_t eveth : 1;
  // Octet 7 (bit 1 -> bit 8)
  uint16_t evequ : 1;
  uint16_t tebur : 1;
  uint16_t ipmjl : 1;
  uint16_t quvti : 1;
  uint16_t emrre : 1;
  uint16_t upint : 1;
  uint16_t spare : 2;
} usage_report_trigger_t;

//-------------------------------------
// 8.2.42 Measurement Period
typedef struct measurement_period_s {
  uint32_t measurement_period;
} measurement_period_t;

//-------------------------------------
// 8.2.43 Fully qualified PDN Connection Set Identifier (FQ-CSID)
// FQ-CSID (Figure 8.2.43-1) has the SAME format as TS 29.274 clause 8.62
// (GTPv2-C): octet 5 = Node-ID Type(4) | Number of CSIDs(4), then the
// Node-Address (IPv4/IPv6/MCC-MNC+operator id), then up to 15 CSIDs of 2
// octets each. It is defined once, in the global namespace, as ::fq_csid_t in
// 3gpp_29.274.h; alias it into the pfcp namespace so pfcp::fq_csid_t works.
// typedef ::fq_csid_t fq_csid_t;

// Extended as per Release 16.5.0, to handle stats related to packet count
//-------------------------------------
// 8.2.44 Volume Measurement
typedef struct volume_measurement_s {
  uint8_t tovol : 1;
  uint8_t ulvol : 1;
  uint8_t dlvol : 1;
  uint8_t tonop : 1;
  uint8_t ulnop : 1;
  uint8_t dlnop : 1;
  uint8_t spare : 2;
  uint64_t total_volume;
  uint64_t uplink_volume;
  uint64_t downlink_volume;
  uint64_t total_nop;
  uint64_t uplink_nop;
  uint64_t downlink_nop;
} volume_measurement_t;

//-------------------------------------
// 8.2.45 Duration Measurement
typedef struct duration_measurement_s {
  uint32_t duration;
} duration_measurement_t;

//-------------------------------------
// 8.2.46 Time of First Packet
typedef struct time_of_first_packet_s {
  uint32_t time_of_first_packet;
} time_of_first_packet_t;

//-------------------------------------
// 8.2.47 Time of Last Packet
typedef struct time_of_last_packet_s {
  uint32_t time_of_last_packet;
} time_of_last_packet_t;

//-------------------------------------
// 8.2.48 Quota Holding Time
typedef struct quota_holding_time_s {
  uint32_t quota_holding_time;
} quota_holding_time_t;

//-------------------------------------
// 8.2.49 Dropped DL Traffic Threshold
typedef struct dropped_dl_traffic_threshold_s {
  uint8_t dlpa : 1;
  uint8_t dlby : 1;
  uint8_t spare : 6;
  uint64_t downlink_packets;
  uint64_t number_of_bytes_of_downlink_data;
} dropped_dl_traffic_threshold_t;

//-------------------------------------
// 8.2.50 Volume Quota
typedef struct volume_quota_s {
  uint8_t tovol : 1;
  uint8_t ulvol : 1;
  uint8_t dlvol : 1;
  uint8_t spare : 5;
  uint64_t total_volume;
  uint64_t uplink_volume;
  uint64_t downlink_volume;
} volume_quota_t;

//-------------------------------------
// 8.2.51 Time Quota
typedef struct time_quota_s {
  uint32_t time_quota;
} time_quota_t;

//-------------------------------------
// 8.2.52 Start Time
typedef struct start_time_s {
  uint32_t start_time;
} start_time_t;

//-------------------------------------
// 8.2.53 End Time
typedef struct end_time_s {
  uint32_t end_time;
} end_time_t;

//-------------------------------------
// 8.2.54 URR ID
typedef struct urr_id_s {
  uint32_t urr_id;
} urr_id_t;

//-------------------------------------
// 8.2.55 Linked URR ID IE
typedef struct linked_urr_id_s {
  uint32_t linked_urr_id;
} linked_urr_id_t;

//-------------------------------------
// 8.2.56 Outer Header Creation
// Table 8.2.56-1 (bitmask, octet 5 = high byte, octet 6 = low byte)
enum outer_header_creation_description_value_e {
  OUTER_HEADER_CREATION_GTPU_UDP_IPV4    = 0x0100,  // 5/1
  OUTER_HEADER_CREATION_GTPU_UDP_IPV6    = 0x0200,  // 5/2
  OUTER_HEADER_CREATION_UDP_IPV4         = 0x0400,  // 5/3
  OUTER_HEADER_CREATION_UDP_IPV6         = 0x0800,  // 5/4
  OUTER_HEADER_CREATION_IPV4             = 0x1000,  // 5/5
  OUTER_HEADER_CREATION_IPV6             = 0x2000,  // 5/6
  OUTER_HEADER_CREATION_C_TAG            = 0x4000,  // 5/7
  OUTER_HEADER_CREATION_S_TAG            = 0x8000,  // 5/8
  OUTER_HEADER_CREATION_N19_INDICATION   = 0x0001,  // 6/1
  OUTER_HEADER_CREATION_N6_INDICATION    = 0x0002,  // 6/2
  OUTER_HEADER_CREATION_LL_SSM_AND_CTEID = 0x0004   // 6/3
};

typedef struct outer_header_creation_s {
  uint16_t outer_header_creation_description;
  uint32_t teid;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
  uint16_t port_number;
  uint32_t c_tag;  // present if C-TAG bit set (added)
  uint32_t s_tag;  // present if S-TAG bit set (added)
} outer_header_creation_t;

//-------------------------------------
// 8.2.57 BAR ID
typedef struct bar_id_s {
  uint8_t bar_id;
} bar_id_t;

//-------------------------------------
// 8.2.58 CP Function Features
struct cp_function_features_s {
  // Octet 5
  uint8_t load : 1;   // 5/1
  uint8_t ovrl : 1;   // 5/2
  uint8_t epfar : 1;  // 5/3
  uint8_t sset : 1;   // 5/4
  uint8_t bundl : 1;  // 5/5
  uint8_t mpas : 1;   // 5/6
  uint8_t ardr : 1;   // 5/7
  uint8_t uiaur : 1;  // 5/8
  // Octet 6
  uint8_t psucc : 1;  // 6/1
  uint8_t rpgur : 1;  // 6/2
  uint8_t spare : 6;
  cp_function_features_s()
      : load(0),
        ovrl(0),
        epfar(0),
        sset(0),
        bundl(0),
        mpas(0),
        ardr(0),
        uiaur(0),
        psucc(0),
        rpgur(0),
        spare(0) {}
  cp_function_features_s(const cp_function_features_s& i)
      : load(i.load),
        ovrl(i.ovrl),
        epfar(i.epfar),
        sset(i.sset),
        bundl(i.bundl),
        mpas(i.mpas),
        ardr(i.ardr),
        uiaur(i.uiaur),
        psucc(i.psucc),
        rpgur(i.rpgur),
        spare(i.spare) {}
};
typedef struct cp_function_features_s cp_function_features_t;
//-------------------------------------
// 8.2.59 Usage Information
typedef struct usage_information_s {
  uint8_t bef : 1;
  uint8_t aft : 1;
  uint8_t uae : 1;
  uint8_t ube : 1;
  uint8_t spare : 4;
} usage_information_t;

//-------------------------------------
// 8.2.60 Application Instance ID
typedef struct application_instance_id_s {
  std::string application_instance_identifier;
} application_instance_id_t;

//-------------------------------------
// 8.2.61 Flow Information
enum flow_direction_value_e {
  UNSPECIFIED   = 0,
  DOWNLINK      = 1,
  UPLINK        = 2,
  BIDIRECTIONAL = 3
};

typedef struct flow_information_s {
  uint8_t flow_direction : 3;
  uint8_t spare : 5;
  // Octets 6-7: 2-octet length field (was erroneously a 3-bit bit-field).
  uint16_t length_of_flow_description;
  std::string flow_description;
} flow_information_t;

//-------------------------------------
// 8.2.62 UE IP Address
typedef struct ue_ip_address_s {
  uint8_t v6 : 1;
  uint8_t v4 : 1;
  uint8_t sd : 1;
  uint8_t ipv6d : 1;
  uint8_t chv4 : 1;
  uint8_t chv6 : 1;
  uint8_t ip6pl : 1;
  uint8_t spare : 1;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
  uint8_t ipv6_prefix_delegation_bits;
  uint8_t ipv6_prefix_length;
} ue_ip_address_t;

//-------------------------------------
// 8.2.63 Packet Rate
typedef struct packet_rate_s {
  // Octet 5: Spare(5) | APRC | DLPR | ULPR
  uint8_t ulpr : 1;
  uint8_t dlpr : 1;
  uint8_t aprc : 1;  // Additional Packet Rate Control present (Rel-16)
  uint8_t spare : 5;
  // Octet m: Spare(5) | Uplink Time Unit(3)
  uint8_t uplink_time_unit : 3;
  uint8_t spare_ul : 5;
  uint16_t maximum_uplink_packet_rate;  // octets (m+1)..(m+2)
  // Octet p: Spare(5) | Downlink Time Unit(3)
  uint8_t downlink_time_unit : 3;
  uint8_t spare_dl : 5;
  uint16_t maximum_downlink_packet_rate;  // octets (p+1)..(p+2)
  // Octet q: Spare(5) | Additional Uplink Time Unit(3)   (present if APRC)
  uint8_t additional_uplink_time_unit : 3;
  uint8_t spare_aul : 5;
  uint16_t additional_maximum_uplink_packet_rate;
  // Octet r: Spare(5) | Additional Downlink Time Unit(3) (present if APRC)
  uint8_t additional_downlink_time_unit : 3;
  uint8_t spare_adl : 5;
  uint16_t additional_maximum_downlink_packet_rate;
} packet_rate_t;

//-------------------------------------
// 8.2.64 Outer Header Removal
// Table 8.2.64-1: Outer Header Removal Description (octet 5)
#define OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4 0
#define OUTER_HEADER_REMOVAL_GTPU_UDP_IPV6 1
#define OUTER_HEADER_REMOVAL_UDP_IPV4 2
#define OUTER_HEADER_REMOVAL_UDP_IPV6 3
#define OUTER_HEADER_REMOVAL_IPV4 4
#define OUTER_HEADER_REMOVAL_IPV6 5
#define OUTER_HEADER_REMOVAL_GTPU_UDP_IP 6
#define OUTER_HEADER_REMOVAL_VLAN_TAG_POP 7
#define OUTER_HEADER_REMOVAL_VLAN_TAGS_POP_POP 8
// Table 8.2.64-2: GTP-U Extension Header Deletion bitmask (octet 6)
#define OUTER_HEADER_REMOVAL_EXTHDR_DEL_PDU_SESSION_CONTAINER 0x01  // bit 6/1
typedef struct outer_header_removal_s {
  uint8_t outer_header_removal_description;  // octet 5 (Table 8.2.64-1)
  uint8_t gtp_u_extension_header_deletion;   // octet 6 bitmask (conditional)
} outer_header_removal_t;

//-------------------------------------
// 8.2.65 Recovery Time Stamp
typedef struct recovery_time_stamp_s {
  uint32_t recovery_time_stamp;
  bool operator==(const struct recovery_time_stamp_s& a) const {
    return (recovery_time_stamp == a.recovery_time_stamp);
  }
} recovery_time_stamp_t;

//-------------------------------------
// 8.2.66 DL Flow Level Marking
typedef struct dl_flow_level_marking_s {
  uint8_t ttc : 1;
  uint8_t sci : 1;
  uint8_t spare : 6;
  std::string tos_traffic_class;
  std::string service_class_indicator;
} dl_flow_level_marking_t;

//-------------------------------------
// 8.2.67 Header Enrichment
// Table 8.2.67-1: Header Type (5-bit field; values 1..31 spare)
enum header_type_value_e { HEADER_TYPE_HTTP = 0 };
typedef struct header_enrichment_s {
  uint8_t header_type : 5;
  uint8_t spare : 3;
  uint8_t lenght_of_header_field_name;
  std::string header_field_name;
  uint8_t lenght_of_header_field_value;
  std::string header_field_value;
} header_enrichment_t;

//-------------------------------------
// 8.2.68 Measurement Information
typedef struct measurement_information_s {
  uint8_t mbqe : 1;
  uint8_t inam : 1;
  uint8_t radi : 1;
  uint8_t istm : 1;   // added (Rel-16)
  uint8_t mnop : 1;   // added (Rel-16)
  uint8_t sspoc : 1;  // added (Rel-17)
  uint8_t aspoc : 1;  // added (Rel-17)
  uint8_t ciam : 1;   // added (Rel-17)
} measurement_information_t;

//-------------------------------------
// 8.2.69 Node Report Type
typedef struct node_report_type_s {
  uint8_t upfr : 1;
  uint8_t uprr : 1;  // added (Rel-16)
  uint8_t ckdr : 1;  // added (Rel-16)
  uint8_t gpqr : 1;  // added (Rel-16)
  uint8_t purr : 1;  // added (Rel-17)
  uint8_t vsr : 1;   // added (Rel-17)
  uint8_t spare : 2;
} node_report_type_t;

//-------------------------------------
// 8.2.70 Remote GTP-U Peer
typedef struct remote_gtp_u_peer_s {
  uint8_t v6 : 1;
  uint8_t v4 : 1;
  uint8_t di : 1;  // Destination Interface present (added)
  uint8_t ni : 1;  // Network Instance present (added)
  uint8_t spare : 4;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
  uint16_t length_of_destination_interface_field;  // present if DI
  uint8_t destination_interface;                   // present if DI
  uint16_t length_of_network_instance_field;       // present if NI
  std::string network_instance;                    // present if NI
} remote_gtp_u_peer_t;

//-------------------------------------
// 8.2.71 UR-SEQN
typedef struct ur_seqn_s {
  uint32_t ur_seqn;
} ur_seqn_t;

//-------------------------------------
// 8.2.72 Activate Predefined Rules
typedef struct activate_predefined_rules_s {
  std::string predefined_rules_name;
} activate_predefined_rules_t;

//-------------------------------------
// 8.2.73 Deactivate Predefined Rules
typedef struct deactivate_predefined_rules_s {
  std::string predefined_rules_name;
} deactivate_predefined_rules_t;

//-------------------------------------
// 8.2.74 FAR ID
struct far_id_s {
  uint32_t far_id;
  far_id_s() : far_id(0) {}
  far_id_s(const uint8_t& f) : far_id(f) {}
  far_id_s(const struct far_id_s& f) : far_id(f.far_id) {}
  bool operator==(const struct far_id_s& i) const {
    return (i.far_id == far_id);
  };
};
typedef struct far_id_s far_id_t;
//-------------------------------------
// 8.2.75 QER ID
typedef struct qer_id_s {
  uint32_t qer_id;
} qer_id_t;

//-------------------------------------
// 8.2.76 OCI Flags
typedef struct oci_flags_s {
  uint8_t aoci : 1;
  uint8_t spare : 7;
} oci_flags_t;

//-------------------------------------
// 8.2.77 PFCP Association Release Request
typedef struct pfcp_association_release_request_s {
  uint8_t sarr : 1;
  uint8_t urss : 1;  // added: UP Function Restart / Session Set report
  uint8_t spare : 6;
} pfcp_association_release_request_t;

//-------------------------------------
// 8.2.78 Graceful Release Period
typedef struct graceful_release_period_s {
  uint8_t timer_value : 5;
  uint8_t timer_unit : 3;
} graceful_release_period_t;

//-------------------------------------
// 8.2.79 PDN Type
enum pdn_type_value_e {
  /* Request / Initial message */
  IPV4     = 1,
  IPV6     = 2,
  IPV4V6   = 3,
  NON_IP   = 4,
  ETHERNET = 5
};

typedef struct pdn_type_s {
  uint8_t pdn_type : 3;
  uint8_t spare : 5;
} pdn_type_t;

//-------------------------------------
// 8.2.80 Failed Rule ID
enum rule_id_type_e {
  /* Request / Initial message */
  FAILED_RULE_ID_TYPE_PDR = 0,
  FAILED_RULE_ID_TYPE_FAR,
  FAILED_RULE_ID_TYPE_QER,
  FAILED_RULE_ID_TYPE_URR,
  FAILED_RULE_ID_TYPE_BAR,
  FAILED_RULE_ID_TYPE_MAR,  // 5 (added)
  FAILED_RULE_ID_TYPE_SRR   // 6 (added)
};
typedef struct failed_rule_id_s {
  uint8_t rule_id_type : 5;
  uint8_t spare : 3;
  uint32_t rule_id_value;
} failed_rule_id_t;

//-------------------------------------
// 8.2.81 Time Quota Mechanism
enum base_time_interval_type_value_e {
  /* Request / Initial message */
  CTP = 0,
  DTP = 1
};

typedef struct time_quota_mechanism_s {
  uint8_t btit : 2;  // Octet 5 bits 2-1 (was 3 bits => byte overflow)
  uint8_t spare : 6;
  uint32_t base_time_interval;
} time_quota_mechanism_t;

//-------------------------------------
// 8.2.82 Void (in Rel-16+): clause 8.2.82 is Void and the associated IE type
// 116 is Reserved in TS 29.244 Rel-17.
//
// DEPRECATED / legacy IE: "User Plane IP Resource Information" (IE type 116)
// was clause 8.2.82 in Rel-15 (used by SGW-U/PGW-U to report GTP-U F-TEID
// resources over the 4G Sxa/Sxb interfaces in PFCP Association Setup). It has
// been Void since Rel-16. Kept here because the existing encoder/messages
// still reference it for backward compatibility; do NOT use it for N4/5G.
typedef struct user_plane_ip_resource_information_s {
  uint8_t v4 : 1;
  uint8_t v6 : 1;
  uint8_t teidri : 3;
  uint8_t assoni : 1;
  uint8_t assosi : 1;
  uint8_t spare : 1;
  uint8_t teid_range;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
  std::string network_instance;
  uint8_t source_interface;
} user_plane_ip_resource_information_t;

//-------------------------------------
// 8.2.83 User Plane Inactivity Timer
typedef struct user_plane_inactivity_timer_s {
  uint32_t user_plane_inactivity_timer;
} user_plane_inactivity_timer_t;

//-------------------------------------
// 8.2.84 Multiplier
typedef struct multiplier_s {
  uint64_t value_digits;
  uint32_t exponent;
} multiplier_t;

//-------------------------------------
// 8.2.85 Aggregated URR ID IE
typedef struct aggregated_urr_id_s {
  uint32_t urr_id;
} aggregated_urr_id_t;

//-------------------------------------
// 8.2.86 Subsequent Volume Quota
typedef struct subsequent_volume_quota_s {
  uint8_t tovol : 1;
  uint8_t ulvol : 1;
  uint8_t dlvol : 1;
  uint8_t spare : 5;
  uint64_t total_volume;
  uint64_t uplink_volume;
  uint64_t downlink_volume;
} subsequent_volume_quota_t;

//-------------------------------------
// 8.2.87 Subsequent Time Quota
typedef struct subsequent_time_quota_s {
  uint64_t time_quota;
} subsequent_time_quota_t;

//-------------------------------------
// 8.2.88 RQI
typedef struct rqi_s {
  uint8_t rqi : 1;
  uint8_t spare : 7;
} rqi_t;

//-------------------------------------
// 8.2.89 QFI
typedef struct qfi_s {
  uint8_t qfi : 6;
  uint8_t spare : 2;
  qfi_s() : qfi(0), spare(0) {}
  qfi_s(const uint8_t& q) : qfi(q), spare(0) {}
  qfi_s(const struct qfi_s& q) : qfi(q.qfi), spare(q.spare) {}
  inline bool operator==(const struct qfi_s& rhs) const {
    return ((qfi == rhs.qfi) && (spare == rhs.spare));
  }
  inline bool operator!=(const struct qfi_s& rhs) const {
    return !((qfi == rhs.qfi) && (spare == rhs.spare));
  }
} qfi_t;

//-------------------------------------
// 8.2.90 Query URR Reference
typedef struct query_urr_reference_s {
  uint32_t query_urr_reference;
} query_urr_reference_t;

//-------------------------------------
// 8.2.91 Additional Usage Reports Information
typedef struct additional_usage_reports_information_s {
  uint8_t auri : 1;  // octet 5 bit 8
  // 15-bit count (octet 5 bits 7-1 + octet 6); range 0..32767.
  uint16_t number_of_additional_usage_reports_value;
} additional_usage_reports_information_t;

//-------------------------------------
// 8.2.92 Traffic Endpoint ID
typedef struct traffic_endpoint_id_s {
  uint8_t traffic_endpoint_id;
} traffic_endpoint_id_t;

//-------------------------------------
// 8.2.93 MAC address
typedef struct mac_address_s {
  // Octet 5: Spare(4) | UDES | USOU | DEST | SOUR (all 1-bit flags)
  uint8_t sour : 1;
  uint8_t dest : 1;
  uint8_t usou : 1;
  uint8_t udes : 1;
  uint8_t spare : 4;
  uint8_t source_mac_address[6];
  uint8_t destination_mac_address[6];
  uint8_t upper_source_mac_address[6];
  uint8_t upper_destination_mac_address[6];
} mac_address_t;

//-------------------------------------
// 8.2.94 C-TAG (Customer-VLAN tag)
typedef struct c_tag_s {
  uint8_t pcp : 1;
  uint8_t dei : 1;
  uint8_t vid : 1;
  uint8_t spare : 5;
  uint8_t pcp_value : 3;
  uint8_t dei_flag : 1;
  // 12-bit C-VID value (octet 6 high nibble + octet 7)
  uint16_t cvid_value;
} c_tag_t;

//-------------------------------------
// 8.2.95 S-TAG (Service-VLAN tag)
typedef struct s_tag_s {
  uint8_t pcp : 1;
  uint8_t dei : 1;
  uint8_t vid : 1;
  uint8_t spare : 5;
  uint8_t pcp_value : 3;
  uint8_t dei_flag : 1;
  // 12-bit S-VID value (octet 6 high nibble + octet 7)
  uint16_t svid_value;
} s_tag_t;

//-------------------------------------
// 8.2.96 Ethertype
typedef struct ethertype_s {
  uint16_t ethertype;
} ethertype_t;

//-------------------------------------
// 8.2.97 Proxying
typedef struct proxying_s {
  uint8_t arp : 1;
  uint8_t ins : 1;
  uint8_t spare : 6;
} proxying_t;

//-------------------------------------
// 8.2.98 Ethernet Filter ID
typedef struct ethernet_filter_id_s {
  uint32_t ethernet_filter_id;
} ethernet_filter_id_t;

//-------------------------------------
// 8.2.99 Ethernet Filter Properties
typedef struct ethernet_filter_properties_s {
  uint8_t bide : 1;
  uint8_t spare : 7;
} ethernet_filter_properties_t;

//-------------------------------------
// 8.2.100 Suggested Buffering Packets Count
typedef struct suggested_buffering_packets_count_s {
  uint8_t packets_count_value;
} suggested_buffering_packets_count_t;

//-------------------------------------
// 8.2.101 User ID
typedef struct user_id_s {
  uint8_t imsif : 1;
  uint8_t imeif : 1;
  uint8_t msisdnf : 1;
  uint8_t naif : 1;
  uint8_t supif : 1;  // added (Rel-16)
  uint8_t gpsif : 1;  // added (Rel-16)
  uint8_t peif : 1;   // added (Rel-17)
  uint8_t spare : 1;
  uint8_t length_of_imsi;
  imsi_t imsi;
  uint8_t length_of_imei;
  std::string imei;
  uint8_t length_of_msisdn;
  msisdn_t msisdn;
  uint8_t length_of_nai;
  std::string nai;
  uint8_t length_of_supi;  // added (Rel-16)
  std::string supi;        // added (Rel-16)
  uint8_t length_of_gpsi;  // added (Rel-16)
  std::string gpsi;        // added (Rel-16)
  uint8_t length_of_pei;   // added (Rel-17)
  std::string pei;         // added (Rel-17)
} user_id_t;

//-------------------------------------
// 8.2.102 Ethernet PDU Session Information
typedef struct ethernet_pdu_session_information_s {
  uint8_t ethi : 1;
  uint8_t spare : 7;
} ethernet_pdu_session_information_t;

//-------------------------------------
// A single 48-bit (6-octet) MAC address value, as listed in the MAC Addresses
// Detected / Removed IEs (Figures 8.2.103-1 / 8.2.104-1).
typedef struct mac_address_value_s {
  uint8_t value[6];
} mac_address_value_t;

//-------------------------------------
// 8.2.103 MAC addresses Detected
typedef struct mac_addresses_detected_s {
  uint8_t number_of_mac_addresses;  // octet 5 (k)
  std::vector<mac_address_value_t>
      mac_addresses;        // k * 6 octets (from octet 6)
  uint8_t length_of_c_tag;  // octet s (0 if C-TAG absent)
  std::string c_tag;        // C-TAG value per 8.2.94, excluding octets 1-4
  uint8_t length_of_s_tag;  // octet u (0 if S-TAG absent)
  std::string s_tag;        // S-TAG value per 8.2.95, excluding octets 1-4
} mac_addresses_detected_t;

//-------------------------------------
// 8.2.104 MAC addresses Removed
typedef struct mac_addresses_removed_s {
  uint8_t number_of_mac_addresses;  // octet 5 (k)
  std::vector<mac_address_value_t>
      mac_addresses;        // k * 6 octets (from octet 6)
  uint8_t length_of_c_tag;  // octet s (0 if C-TAG absent)
  std::string c_tag;        // C-TAG value per 8.2.94, excluding octets 1-4
  uint8_t length_of_s_tag;  // octet u (0 if S-TAG absent)
  std::string s_tag;        // S-TAG value per 8.2.95, excluding octets 1-4
} mac_addresses_removed_t;

//-------------------------------------
// 8.2.105 Ethernet Inactivity Timer
typedef struct ethernet_inactivity_timer_s {
  uint32_t ethernet_inactivity_timer;
} ethernet_inactivity_timer_t;

//-------------------------------------
// 8.2.106 Subsequent Event Quota
typedef struct subsequent_event_quota_s {
  uint32_t subsequent_event_quota;
} subsequent_event_quota_t;

//-------------------------------------
// 8.2.107 Subsequent Event Threshold
typedef struct subsequent_event_threshold_s {
  uint32_t subsequent_event_threshold;
} subsequent_event_threshold_t;

//-------------------------------------
// 8.2.108 Trace Information
typedef struct trace_information_s {
  // Bit 1 first (low nibble first), per whole-file convention:
  // Octet 5: MCC digit 1 (bits 1-4) | MCC digit 2 (bits 5-8)
  uint8_t mcc_digit1 : 4;
  uint8_t mcc_digit2 : 4;
  // Octet 6: MCC digit 3 (bits 1-4) | MNC digit 3 (bits 5-8)
  uint8_t mcc_digit3 : 4;
  uint8_t mnc_digit3 : 4;
  // Octet 7: MNC digit 1 (bits 1-4) | MNC digit 2 (bits 5-8)
  uint8_t mnc_digit1 : 4;
  uint8_t mnc_digit2 : 4;
  uint32_t trace_id;  // octets 8-10 (3 octets)
  uint8_t length_of_triggered_events;
  std::string triggering_events;
  uint8_t session_trace_depth;
  uint8_t length_of_list_of_interfaces;
  std::string list_of_interfaces;
  uint8_t length_of_ip_address_of_trace_collection_entity;
  std::string ip_address_of_trace_collection_entity;
} trace_information_t;

//-------------------------------------
// TO BE REMOVED: Event ID
typedef struct event_id_s {
  uint32_t event_id;
} event_id_t;

//-------------------------------------
// 8.2.109 Framed-Route
typedef struct framed_route_s {
  std::string framed_route;
} framed_route_t;

//-------------------------------------
// 8.2.110 Framed-Routing
typedef struct framed_routing_s {
  uint32_t framed_routing;
} framed_routing_t;

//-------------------------------------
// 8.2.111 Framed-IPv6-Route
typedef struct framed_ipv6_route_s {
  std::string framed_ipv6_route;
} framed_ipv6_route_t;

//-------------------------------------
// 8.2.112 Event Quota
typedef struct event_quota_s {
  uint32_t subsequent_event_quota;
} event_quota_t;

//-------------------------------------
// 8.2.113 Event Threshold
typedef struct event_threshold_s {
  uint32_t event_threshold;
} event_threshold_t;

//-------------------------------------
// 8.2.114 Time Stamp
typedef struct time_stamp_s {
  uint32_t time_stamp;
} time_stamp_t;

//-------------------------------------
// 8.2.115 Averaging Window
typedef struct averaging_window_s {
  uint32_t averaging_window;
} averaging_window_t;

//-------------------------------------
// 8.2.116 Paging Policy Indicator (PPI)
typedef struct paging_policy_indicator_s {
  // Octet 5: Spare(5) | PPI value(3)  (PPI value range 0..7)
  uint8_t ppi_value : 3;
  uint8_t spare : 5;
} paging_policy_indicator_t;

//-------------------------------------
// 8.2.117 APN/DNN
typedef struct apn_dnn_s {
  std::string apn_dnn;
} apn_dnn_t;

typedef struct nssai_s {
  uint32_t nssai;
} nssai_t;

//-------------------------------------
// 8.2.118 3GPP Interface Type
// Table 8.2.118-1: Interface Type value (6-bit field; 32..63 spare)
enum _3gpp_interface_type_e {
  _3GPP_INTERFACE_TYPE_S1_U                         = 0,
  _3GPP_INTERFACE_TYPE_S5_S8_U                      = 1,
  _3GPP_INTERFACE_TYPE_S4_U                         = 2,
  _3GPP_INTERFACE_TYPE_S11_U                        = 3,
  _3GPP_INTERFACE_TYPE_S12                          = 4,  // "S12" (not S12-U)
  _3GPP_INTERFACE_TYPE_GN_GP_U                      = 5,
  _3GPP_INTERFACE_TYPE_S2A_U                        = 6,
  _3GPP_INTERFACE_TYPE_S2B_U                        = 7,
  _3GPP_INTERFACE_TYPE_ENODEB_GTP_U_DL              = 8,   // DL data forwarding
  _3GPP_INTERFACE_TYPE_ENODEB_GTP_U_UL              = 9,   // UL data forwarding
  _3GPP_INTERFACE_TYPE_SGW_UPF_GTP_U_DL             = 10,  // DL data forwarding
  _3GPP_INTERFACE_TYPE_N3_3GPP_ACCESS               = 11,
  _3GPP_INTERFACE_TYPE_N3_TRUSTED_NON_3GPP_ACCESS   = 12,
  _3GPP_INTERFACE_TYPE_N3_UNTRUSTED_NON_3GPP_ACCESS = 13,
  _3GPP_INTERFACE_TYPE_N3_FOR_DATA_FORWARDING = 14,  // was mislabeled "N3"
  _3GPP_INTERFACE_TYPE_N9                     = 15,  // N9 (non-roaming)
  _3GPP_INTERFACE_TYPE_SGI                    = 16,
  _3GPP_INTERFACE_TYPE_N6                     = 17,
  _3GPP_INTERFACE_TYPE_N19                    = 18,
  _3GPP_INTERFACE_TYPE_S8_U                   = 19,
  _3GPP_INTERFACE_TYPE_GP_U                   = 20,
  _3GPP_INTERFACE_TYPE_N9_FOR_ROAMING         = 21,
  _3GPP_INTERFACE_TYPE_IU_U                   = 22,
  _3GPP_INTERFACE_TYPE_N9_FOR_DATA_FORWARDING = 23,
  _3GPP_INTERFACE_TYPE_SXA_U                  = 24,
  _3GPP_INTERFACE_TYPE_SXB_U                  = 25,
  _3GPP_INTERFACE_TYPE_SXC_U                  = 26,
  _3GPP_INTERFACE_TYPE_N4_U                   = 27,
  _3GPP_INTERFACE_TYPE_SGW_UPF_GTP_U_UL       = 28,  // UL data forwarding
  _3GPP_INTERFACE_TYPE_N6MB_NMB9              = 29,
  _3GPP_INTERFACE_TYPE_N3MB                   = 30,
  _3GPP_INTERFACE_TYPE_N19MB                  = 31
};

typedef struct _3gpp_interface_type_s {
  uint8_t interface_type_value : 6;
  uint8_t spare : 2;
} _3gpp_interface_type_t;

//-------------------------------------
// 8.2.119 PFCPSRReq-Flags
typedef struct pfcpsrreq_flags_s {
  uint8_t psdbu : 1;
  uint8_t spare : 7;
} pfcpsrreq_flags_t;

//-------------------------------------
// 8.2.120 PFCPAUReq-Flags
typedef struct pfcpaureq_flags_s {
  uint8_t parps : 1;
  uint8_t spare : 7;
} pfcpaureq_flags_t;

//-------------------------------------
// 8.2.121 Activation Time
typedef struct activation_time_s {
  uint32_t activation_time;
} activation_time_t;

//-------------------------------------
// 8.2.122 Deactivation Time
typedef struct deactivation_time_s {
  uint32_t deactivation_time;
} deactivation_time_t;

//-------------------------------------
// 8.2.123 MAR ID
typedef struct mar_id_s {
  uint8_t mar_id;
} mar_id_t;

//-------------------------------------
// 8.2.124 Steering Functionality
typedef struct steering_functionality_s {
  uint8_t steering_functionality_value : 4;
  uint8_t spare : 4;
} steering_functionality_t;

enum steering_functionality_value_e {
  STEERING_FUNCTIONALITY_ATSSS_LL = 0,
  STEERING_FUNCTIONALITY_MPTCP    = 1,
  STEERING_FUNCTIONALITY_SPARE    = 2
};

//-------------------------------------
// 8.2.125 Steering Mode
typedef struct steering_mode_s {
  uint8_t steering_mode_value : 4;
  uint8_t spare : 4;
} steering_mode_t;

enum steering_mode_value_e {
  STEERING_MODE_ACTIVE_STANDBY = 0,
  STEERING_MODE_SMALLEST_DELAY = 1,
  STEERING_MODE_LOAD_BALANCING = 2,
  STEERING_MODE_PRIORITY_BASED = 3,
  STEERING_MODE_SPARED         = 4
};

//-------------------------------------
// 8.2.126 Weight
typedef struct weight_s {
  uint8_t weight_value;
} weight_t;

//-------------------------------------
// 8.2.127 Priority
typedef struct priority_s {
  uint8_t priority_value : 4;
  uint8_t spare : 4;
} priority_t;
// Table 8.2.127-1: Priority value (4-bit field; 5..15 spare)
enum priority_value_e {
  PRIORITY_VALUE_ACTIVE     = 0,
  PRIORITY_VALUE_STANDBY    = 1,
  PRIORITY_VALUE_NO_STANDBY = 2,
  PRIORITY_VALUE_HIGH       = 3,
  PRIORITY_VALUE_LOW        = 4
};

//-------------------------------------
// 8.2.128 UE IP address Pool Identity
typedef struct ue_ip_address_pool_identity_s {
  uint16_t ue_ip_address_pool_id_length;    // octets 5-6 (was missing)
  std::string ue_ip_address_pool_identity;  // octets 7..k (OctetString)
} ue_ip_address_pool_identity_t;

//-------------------------------------
// 8.2.129 Alternative SMF IP Address
typedef struct alternative_smf_ip_address_s {
  // Figure 8.2.129-1: octet 5 = Spare(5) | PPE | V4 | V6
  uint8_t v6 : 1;
  uint8_t v4 : 1;
  uint8_t ppe : 1;  // added (was missing): PFCP Port present
  uint8_t spare : 5;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
} alternative_smf_ip_address_t;

//===========================================================================
// Release-17 leaf IEs (types 179..321 / clauses 8.2.130..8.2.218).
// Added to complete TS 29.244 Rel-17. These are the on-wire (leaf) IEs;
// grouped IEs (Create SRR, Provide ATSSS Control Information, MBS Session
// N4mb Control Information, L2TP Tunnel/Session Information, etc.) are
// assembled as message classes in pfcp/3gpp_29.244.hpp, following the same
// architecture as the pre-existing grouped IEs (kept commented out below).
//===========================================================================

//-------------------------------------
// 8.2.130 Packet Replication and Detection Carry-On Information (type 179)
typedef struct packet_replication_and_detection_carry_on_information_s {
  uint8_t priueai : 1;
  uint8_t prin19i : 1;
  uint8_t prin6i : 1;
  uint8_t dcaroni : 1;
  uint8_t spare : 4;
} packet_replication_and_detection_carry_on_information_t;

//-------------------------------------
// 8.2.131 SMF Set ID (type 180)
typedef struct smf_set_id_s {
  uint8_t spare;     // octet 5
  std::string fqdn;  // octets 6..m
} smf_set_id_t;

//-------------------------------------
// 8.2.132 Quota Validity Time (type 181)
typedef struct quota_validity_time_s {
  uint32_t validity_time_value;
} quota_validity_time_t;

//-------------------------------------
// 8.2.133 Number of Reports (type 182)
typedef struct number_of_reports_s {
  uint16_t number_of_reports;
} number_of_reports_t;

//-------------------------------------
// 8.2.134 PFCPASRsp-Flags (type 184)
typedef struct pfcpasrsp_flags_s {
  uint8_t psrei : 1;
  uint8_t uupsi : 1;
  uint8_t spare : 6;
} pfcpasrsp_flags_t;

//-------------------------------------
// 8.2.135 CP PFCP Entity IP Address (type 185)
typedef struct cp_pfcp_entity_ip_address_s {
  uint8_t v6 : 1;
  uint8_t v4 : 1;
  uint8_t spare : 6;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
} cp_pfcp_entity_ip_address_t;

//-------------------------------------
// 8.2.136 PFCPSEReq-Flags (type 186)
typedef struct pfcpsereq_flags_s {
  uint8_t resti : 1;
  uint8_t sumpc : 1;
  uint8_t spare : 6;
} pfcpsereq_flags_t;

//-------------------------------------
// 8.2.137 IP Multicast Address (type 191)
typedef struct ip_multicast_address_s {
  uint8_t v6 : 1;
  uint8_t v4 : 1;
  uint8_t r : 1;  // Range: any source (start=end means single address)
  uint8_t a : 1;  // Any IP multicast address
  uint8_t spare : 4;
  struct in_addr start_ipv4_address;
  struct in6_addr start_ipv6_address;
  struct in_addr end_ipv4_address;
  struct in6_addr end_ipv6_address;
} ip_multicast_address_t;

//-------------------------------------
// 8.2.138 Source IP Address (type 192)
typedef struct source_ip_address_s {
  uint8_t v6 : 1;
  uint8_t v4 : 1;
  uint8_t mpl : 1;  // Mask/Prefix Length present
  uint8_t spare : 5;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
  uint8_t mask_prefix_length;  // present if MPL
} source_ip_address_t;

//-------------------------------------
// 8.2.139 Packet Rate Status (type 193)
typedef struct packet_rate_status_s {
  uint8_t ul : 1;
  uint8_t dl : 1;
  uint8_t apr : 1;  // Additional Packet Rate present
  uint8_t spare : 5;
  uint16_t number_of_remaining_uplink_packets_allowed;
  uint16_t number_of_remaining_additional_uplink_packets_allowed;
  uint16_t number_of_remaining_downlink_packets_allowed;
  uint16_t number_of_remaining_additional_downlink_packets_allowed;
  uint64_t rate_control_status_validity_time;
} packet_rate_status_t;

//-------------------------------------
// 8.2.140 Create Bridge Info for TSC (type 194)
typedef struct create_bridge_info_for_tsc_s {
  uint8_t bii : 1;
  uint8_t spare : 7;
} create_bridge_info_for_tsc_t;

//-------------------------------------
// 8.2.141 DS-TT Port Number (type 196)
typedef struct ds_tt_port_number_s {
  uint32_t value;
} ds_tt_port_number_t;

//-------------------------------------
// 8.2.142 NW-TT Port Number (type 197)
typedef struct nw_tt_port_number_s {
  uint32_t value;
} nw_tt_port_number_t;

//-------------------------------------
// 8.2.143 5GS User Plane Node (type 198)
typedef struct _5gs_user_plane_node_s {
  uint8_t bid : 1;  // Bridge ID present
  uint8_t spare : 7;
  uint8_t user_plane_node_value[6];  // Bridge ID (if BID)
} _5gs_user_plane_node_t;

//-------------------------------------
// 8.2.144 Port Management Information Container (type 202)
typedef struct port_management_information_container_s {
  std::string port_management_information;
} port_management_information_container_t;

//-------------------------------------
// 8.2.145 Requested Clock Drift Information (type 204)
typedef struct requested_clock_drift_information_s {
  uint8_t rrto : 1;  // Request to Report Time Offset
  uint8_t rrcr : 1;  // Request to Report Cumulative rateRatio
  uint8_t spare : 6;
} requested_clock_drift_information_t;

//-------------------------------------
// 8.2.146 (TSN) Time Domain Number (type 206)
typedef struct time_domain_number_s {
  uint8_t time_domain_number;
} time_domain_number_t;

//-------------------------------------
// 8.2.147 Time Offset Threshold (type 207)
typedef struct time_offset_threshold_s {
  int64_t time_offset_threshold;  // 8 octets, nanoseconds
} time_offset_threshold_t;

//-------------------------------------
// 8.2.148 Cumulative rateRatio Threshold (type 208)
typedef struct cumulative_rate_ratio_threshold_s {
  uint32_t cumulative_rate_ratio_threshold;
} cumulative_rate_ratio_threshold_t;

//-------------------------------------
// 8.2.149 Time Offset Measurement (type 209)
typedef struct time_offset_measurement_s {
  int64_t time_offset_measurement;  // 8 octets, nanoseconds
} time_offset_measurement_t;

//-------------------------------------
// 8.2.150 Cumulative rateRatio Measurement (type 210)
typedef struct cumulative_rate_ratio_measurement_s {
  uint32_t cumulative_rate_ratio_measurement;
} cumulative_rate_ratio_measurement_t;

//-------------------------------------
// 8.2.151 SRR ID (type 215)
typedef struct srr_id_s {
  uint8_t srr_id;
} srr_id_t;

//-------------------------------------
// 8.2.152 Requested Access Availability Information (type 217)
typedef struct requested_access_availability_information_s {
  uint8_t rrca : 1;  // Request to Report Change in Access availability
  uint8_t spare : 7;
} requested_access_availability_information_t;

//-------------------------------------
// 8.2.153 Access Availability Information (type 219)
enum access_availability_status_e { UNAVAILABLE = 0, AVAILABLE = 1 };
enum access_availability_type_e {
  ACCESS_TYPE_3GPP     = 0,
  ACCESS_TYPE_NON_3GPP = 1
};
typedef struct access_availability_information_s {
  uint8_t access_type : 2;
  uint8_t availability_status : 2;
  uint8_t spare : 4;
} access_availability_information_t;

//-------------------------------------
// 8.2.154 MPTCP Control Information (type 222)
typedef struct mptcp_control_information_s {
  uint8_t tci : 1;
  uint8_t spare : 7;
} mptcp_control_information_t;

//-------------------------------------
// 8.2.155 ATSSS-LL Control Information (type 223)
typedef struct atsss_ll_control_information_s {
  uint8_t lli : 1;
  uint8_t spare : 7;
} atsss_ll_control_information_t;

//-------------------------------------
// 8.2.156 PMF Control Information (type 224)
typedef struct pmf_control_information_s {
  uint8_t pmfi : 1;
  uint8_t drtti : 1;
  uint8_t pqpm : 1;
  uint8_t spare : 5;
  uint8_t number_of_qfi;
  std::vector<uint8_t> qfi;  // 'number_of_qfi' QFI values (if PQPM)
} pmf_control_information_t;

//-------------------------------------
// 8.2.157 MPTCP Address Information (type 228)
typedef struct mptcp_address_information_s {
  uint8_t v4 : 1;
  uint8_t v6 : 1;
  uint8_t spare : 6;
  uint8_t mptcp_proxy_type;
  uint16_t mptcp_proxy_port;
  struct in_addr mptcp_proxy_ipv4_address;
  struct in6_addr mptcp_proxy_ipv6_address;
} mptcp_address_information_t;

//-------------------------------------
// 8.2.158 UE Link-Specific IP Address (type 229)
typedef struct ue_link_specific_ip_address_s {
  uint8_t v4 : 1;   // UE Link-Specific IPv4 for 3GPP access present
  uint8_t v6 : 1;   // UE Link-Specific IPv6 for 3GPP access present
  uint8_t nv4 : 1;  // UE Link-Specific IPv4 for non-3GPP access present
  uint8_t nv6 : 1;  // UE Link-Specific IPv6 for non-3GPP access present
  uint8_t spare : 4;
  struct in_addr ue_ipv4_address_3gpp;
  struct in6_addr ue_ipv6_address_3gpp;
  struct in_addr ue_ipv4_address_non_3gpp;
  struct in6_addr ue_ipv6_address_non_3gpp;
} ue_link_specific_ip_address_t;

//-------------------------------------
// 8.2.159 PMF Address Information (type 230)
typedef struct pmf_address_information_s {
  uint8_t v4 : 1;
  uint8_t v6 : 1;
  uint8_t mac : 1;  // MAC addresses present
  uint8_t spare : 5;
  struct in_addr pmf_ipv4_address;
  struct in6_addr pmf_ipv6_address;
  uint16_t pmf_port_3gpp;
  uint16_t pmf_port_non_3gpp;
  uint8_t pmf_mac_address_3gpp[6];
  uint8_t pmf_mac_address_non_3gpp[6];
} pmf_address_information_t;

//-------------------------------------
// 8.2.160 ATSSS-LL Information (type 231)
typedef struct atsss_ll_information_s {
  uint8_t lli : 1;
  uint8_t spare : 7;
} atsss_ll_information_t;

//-------------------------------------
// 8.2.161 Data Network Access Identifier (type 232)
typedef struct data_network_access_identifier_s {
  std::string data_network_access_identifier;
} data_network_access_identifier_t;

//-------------------------------------
// 8.2.162 Average Packet Delay (type 234)
typedef struct average_packet_delay_s {
  uint32_t delay_value_in_milliseconds;
} average_packet_delay_t;

//-------------------------------------
// 8.2.163 Minimum Packet Delay (type 235)
typedef struct minimum_packet_delay_s {
  uint32_t delay_value_in_milliseconds;
} minimum_packet_delay_t;

//-------------------------------------
// 8.2.164 Maximum Packet Delay (type 236)
typedef struct maximum_packet_delay_s {
  uint32_t delay_value_in_milliseconds;
} maximum_packet_delay_t;

//-------------------------------------
// 8.2.165 QoS Report Trigger (type 237)
typedef struct qos_report_trigger_s {
  uint8_t per : 1;
  uint8_t thr : 1;
  uint8_t ire : 1;
  uint8_t spare : 5;
} qos_report_trigger_t;

//-------------------------------------
// 8.2.166 GTP-U Path Interface Type (type 241)
typedef struct gtp_u_path_interface_type_s {
  uint8_t n9 : 1;
  uint8_t n3 : 1;
  uint8_t spare : 6;
} gtp_u_path_interface_type_t;

//-------------------------------------
// 8.2.167 Requested QoS Monitoring (type 243)
typedef struct requested_qos_monitoring_s {
  uint8_t dl : 1;
  uint8_t ul : 1;
  uint8_t rp : 1;      // Round trip
  uint8_t gtpupm : 1;  // GTP-U Path Monitoring
  uint8_t spare : 4;
} requested_qos_monitoring_t;

//-------------------------------------
// 8.2.168 Reporting Frequency (type 244)
typedef struct reporting_frequency_s {
  uint8_t evett : 1;     // bit 1: Event Triggered QoS monitoring reporting
  uint8_t perio : 1;     // bit 2: Periodic QoS monitoring reporting
  uint8_t reserved : 1;  // bit 3: Reserved (was SESRL in earlier releases;
                         //        deprecated in Rel-17, set to 0 / ignored)
  uint8_t spare : 5;     // bits 4-8
} reporting_frequency_t;

//-------------------------------------
// 8.2.169 Packet Delay Thresholds (type 245)
typedef struct packet_delay_thresholds_s {
  uint8_t dl : 1;
  uint8_t ul : 1;
  uint8_t rp : 1;
  uint8_t spare : 5;
  uint32_t downlink_packet_delay_threshold;
  uint32_t uplink_packet_delay_threshold;
  uint32_t round_trip_packet_delay_threshold;
} packet_delay_thresholds_t;

//-------------------------------------
// 8.2.170 Minimum Wait Time (type 246)
typedef struct minimum_wait_time_s {
  uint32_t minimum_wait_time;
} minimum_wait_time_t;

//-------------------------------------
// 8.2.171 QoS Monitoring Measurement (type 248)
typedef struct qos_monitoring_measurement_s {
  uint8_t dl : 1;
  uint8_t ul : 1;
  uint8_t rp : 1;
  uint8_t plmf : 1;  // Packet Delay Measurement Failure
  uint8_t spare : 4;
  uint32_t downlink_packet_delay;
  uint32_t uplink_packet_delay;
  uint32_t round_trip_packet_delay;
} qos_monitoring_measurement_t;

//-------------------------------------
// 8.2.172 MT-EDT Control Information (type 249)
typedef struct mt_edt_control_information_s {
  uint8_t rdsi : 1;
  uint8_t spare : 7;
} mt_edt_control_information_t;

//-------------------------------------
// 8.2.173 DL Data Packets Size (type 250)
typedef struct dl_data_packets_size_s {
  uint16_t dl_data_packets_size;
} dl_data_packets_size_t;

//-------------------------------------
// 8.2.174 QER Control Indications (type 251)
typedef struct qer_control_indications_s {
  uint8_t rcsr : 1;
  uint8_t spare : 7;
} qer_control_indications_t;

//-------------------------------------
// 8.2.175 NF Instance ID (type 253)
typedef struct nf_instance_id_s {
  uint8_t nf_instance_id[16];  // 16-octet UUID
} nf_instance_id_t;

//-------------------------------------
// 8.2.176 S-NSSAI (type 257)
typedef struct s_nssai_s {
  uint8_t sst;  // Slice/Service Type
  uint32_t sd;  // Slice Differentiator (3 octets)
} s_nssai_t;

//-------------------------------------
// 8.2.177 IP version (type 258)
typedef struct ip_version_s {
  uint8_t v4 : 1;
  uint8_t v6 : 1;
  uint8_t spare : 6;
} ip_version_t;

//-------------------------------------
// 8.2.178 PFCPASReq-Flags (type 259)
typedef struct pfcpasreq_flags_s {
  uint8_t uupsi : 1;
  uint8_t spare : 7;
} pfcpasreq_flags_t;

//-------------------------------------
// 8.2.179 Data Status (type 260)
typedef struct data_status_s {
  uint8_t drop : 1;
  uint8_t buff : 1;
  uint8_t spare : 6;
} data_status_t;

//-------------------------------------
// 8.2.180 RDS Configuration Information (type 262)
typedef struct rds_configuration_information_s {
  uint8_t rds : 1;
  uint8_t spare : 7;
} rds_configuration_information_t;

//-------------------------------------
// 8.2.181 MPTCP Applicable Indication (type 265)
typedef struct mptcp_applicable_indication_s {
  uint8_t mai : 1;
  uint8_t spare : 7;
} mptcp_applicable_indication_t;

//-------------------------------------
// 8.2.182 User Plane Node Management Information Container (type 266)
typedef struct user_plane_node_management_information_container_s {
  std::string user_plane_node_management_information;
} user_plane_node_management_information_container_t;

//-------------------------------------
// 8.2.183 Number of UE IP Addresses (type 268)
typedef struct number_of_ue_ip_addresses_s {
  uint8_t ipv4 : 1;
  uint8_t ipv6 : 1;
  uint8_t spare : 6;
  uint32_t number_of_ue_ipv4_addresses;  // if IPv4
  uint32_t number_of_ue_ipv6_addresses;  // if IPv6
} number_of_ue_ip_addresses_t;

//-------------------------------------
// 8.2.184 Validity Timer (type 269)
typedef struct validity_timer_s {
  uint16_t validity_timer;
} validity_timer_t;

//-------------------------------------
// 8.2.185 Offending IE Information (type 274)
typedef struct offending_ie_information_s {
  uint16_t type_of_the_offending_ie;
  std::string value_of_the_offending_ie;
} offending_ie_information_t;

//-------------------------------------
// 8.2.186 RAT Type (type 275)
enum rat_type_value_e {
  RAT_TYPE_RESERVED      = 0,
  RAT_TYPE_UTRAN         = 1,
  RAT_TYPE_GERAN         = 2,
  RAT_TYPE_WLAN          = 3,
  RAT_TYPE_GAN           = 4,
  RAT_TYPE_HSPA_EVOL     = 5,
  RAT_TYPE_EUTRAN        = 6,  // EUTRAN (WB-E-UTRAN)
  RAT_TYPE_VIRTUAL       = 7,
  RAT_TYPE_EUTRAN_NB_IOT = 8,
  RAT_TYPE_LTE_M         = 9,
  RAT_TYPE_NR            = 10,
  // Satellite access (added Rel-17, CR0830 / 17.10.0)
  RAT_TYPE_WB_EUTRAN_LEO          = 11,
  RAT_TYPE_WB_EUTRAN_MEO          = 12,
  RAT_TYPE_WB_EUTRAN_GEO          = 13,
  RAT_TYPE_WB_EUTRAN_OTHERSAT     = 14,
  RAT_TYPE_EUTRAN_NB_IOT_LEO      = 15,
  RAT_TYPE_EUTRAN_NB_IOT_MEO      = 16,
  RAT_TYPE_EUTRAN_NB_IOT_GEO      = 17,
  RAT_TYPE_EUTRAN_NB_IOT_OTHERSAT = 18,
  RAT_TYPE_LTE_M_LEO              = 19,
  RAT_TYPE_LTE_M_MEO              = 20,
  RAT_TYPE_LTE_M_GEO              = 21,
  RAT_TYPE_LTE_M_OTHERSAT         = 22,
  RAT_TYPE_NR_LEO                 = 23,
  RAT_TYPE_NR_MEO                 = 24,
  RAT_TYPE_NR_GEO                 = 25,
  RAT_TYPE_NR_OTHERSAT            = 26
};
typedef struct rat_type_s {
  uint8_t rat_type;
} rat_type_t;

//-------------------------------------
// 8.2.187 L2TP User Authentication (type 278)
typedef struct l2tp_user_authentication_s {
  uint16_t proxy_authen_type_value;  // octets 5-6 (comes BEFORE the flags)
  // Octet 7 flags (bit 1 -> bit 8): PAN | PAC | PAR | PAI | Spare(4)
  uint8_t pan : 1;  // bit 1: Proxy Authen Name present
  uint8_t pac : 1;  // bit 2: Proxy Authen Challenge present
  uint8_t par : 1;  // bit 3: Proxy Authen Response present
  uint8_t pai : 1;  // bit 4: Proxy Authen ID present
  uint8_t spare : 4;
  uint8_t length_of_proxy_authen_name;       // octet 8    (if PAN)
  std::string proxy_authen_name;             // octets 9..a
  uint8_t length_of_proxy_authen_challenge;  // octet a+1  (if PAC)
  std::string proxy_authen_challenge;        // octets (a+2)..b
  uint8_t length_of_proxy_authen_response;   // octet b+1  (if PAR)
  std::string proxy_authen_response;         // octets (b+2)..c
  uint16_t proxy_authen_id;                  // octet c+1  (if PAI)
} l2tp_user_authentication_t;

//-------------------------------------
// 8.2.188 LNS Address (type 280)
typedef struct lns_address_s {
  // Figure 8.2.188-1: a single "LNS Address value" (OctetString), octets 5..o.
  // No flags, no type field: length 4 => IPv4 address, length 16 => IPv6
  // address (distinguished by the IE Length). Held as raw bytes.
  std::string lns_address_value;
} lns_address_t;

//-------------------------------------
// 8.2.189 Tunnel Preference (type 281)
typedef struct tunnel_preference_s {
  uint32_t tunnel_preference;  // 3 octets
} tunnel_preference_t;

//-------------------------------------
// 8.2.190 Calling Number (type 282)
typedef struct calling_number_s {
  std::string calling_number;
} calling_number_t;

//-------------------------------------
// 8.2.191 Called Number (type 283)
typedef struct called_number_s {
  std::string called_number;
} called_number_t;

//-------------------------------------
// 8.2.192 L2TP Session Indications (type 284)
typedef struct l2tp_session_indications_s {
  uint8_t reuia : 1;  // Request UE IP Address
  uint8_t redsa : 1;  // Request DNS Server Address
  uint8_t rensa : 1;  // Request NBNS Server Address
  uint8_t spare : 5;
} l2tp_session_indications_t;

//-------------------------------------
// 8.2.193 DNS Server Address (type 285)
typedef struct dns_server_address_s {
  // Figure 8.2.193-1: octets 5-8 = IPv4 address value (no flags).
  struct in_addr ipv4_address;
} dns_server_address_t;

//-------------------------------------
// 8.2.194 NBNS Server Address (type 286)
typedef struct nbns_server_address_s {
  // Figure 8.2.194-1: octets 5-8 = IPv4 address value (no flags).
  struct in_addr ipv4_address;
} nbns_server_address_t;

//-------------------------------------
// 8.2.195 Maximum Receive Unit (type 287)
typedef struct maximum_receive_unit_s {
  uint16_t maximum_receive_unit;
} maximum_receive_unit_t;

//-------------------------------------
// 8.2.196 Thresholds (type 288)
typedef struct thresholds_s {
  uint8_t rtt : 1;  // RTT value present
  uint8_t plr : 1;  // Packet Loss Rate value present
  uint8_t spare : 6;
  uint16_t rtt_value;
  uint8_t plr_value;
} thresholds_t;

//-------------------------------------
// 8.2.197 Steering Mode Indicator (type 289)
typedef struct steering_mode_indicator_s {
  uint8_t albi : 1;  // Autonomous Load-Balance Indicator
  uint8_t ueai : 1;  // UE Assistance Indicator
  uint8_t spare : 6;
} steering_mode_indicator_t;

//-------------------------------------
// 8.2.198 Group Id (type 291)
typedef struct group_id_s {
  std::string group_id;
} group_id_t;

//-------------------------------------
// 8.2.199 CP IP Address (type 292)
typedef struct cp_ip_address_s {
  uint8_t v6 : 1;
  uint8_t v4 : 1;
  uint8_t spare : 6;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
} cp_ip_address_t;

//-------------------------------------
// 8.2.200 IP Address and Port Number Replacement (type 293)
typedef struct ip_address_and_port_number_replacement_s {
  uint8_t dipv4 : 1;  // Destination IPv4 present
  uint8_t dipv6 : 1;  // Destination IPv6 present
  uint8_t dpn : 1;    // Destination Port Number present
  uint8_t sipv4 : 1;  // Source IPv4 present
  uint8_t sipv6 : 1;  // Source IPv6 present
  uint8_t spn : 1;    // Source Port Number present
  uint8_t spare : 2;
  struct in_addr destination_ipv4_address;
  struct in6_addr destination_ipv6_address;
  uint16_t destination_port_number;
  struct in_addr source_ipv4_address;
  struct in6_addr source_ipv6_address;
  uint16_t source_port_number;
} ip_address_and_port_number_replacement_t;

//-------------------------------------
// 8.2.201 DNS Query Filter (type 294)
typedef struct dns_query_filter_s {
  uint16_t length_of_domain_name_pattern;
  std::string domain_name_pattern;
} dns_query_filter_t;

//-------------------------------------
// 8.2.202 Event Notification URI (type 296)
typedef struct event_notification_uri_s {
  std::string event_notification_uri;
} event_notification_uri_t;

//-------------------------------------
// 8.2.203 Notification Correlation ID (type 297)
typedef struct notification_correlation_id_s {
  uint32_t notification_correlation_id;
} notification_correlation_id_t;

//-------------------------------------
// 8.2.204 Reporting Flags (type 298)
typedef struct reporting_flags_s {
  uint8_t dupl : 1;
  uint8_t spare : 7;
} reporting_flags_t;

//-------------------------------------
// 8.2.205 Predefined Rules Name (type 299)
typedef struct predefined_rules_name_s {
  std::string predefined_rules_name;
} predefined_rules_name_t;

//-------------------------------------
// 8.2.206 MBS Session Identifier (type 305)
typedef struct mbs_session_identifier_s {
  uint8_t tmgi : 1;  // TMGI present
  uint8_t ssmi : 1;  // Source Specific Multicast present
  uint8_t nidi : 1;  // NID present
  uint8_t spare : 5;
  uint8_t tmgi_value[6];
  std::string source_specific_ip_multicast_address_info;
  uint8_t nid[6];
} mbs_session_identifier_t;

//-------------------------------------
// 8.2.207 Multicast Transport Information (type 306)
typedef struct multicast_transport_information_s {
  uint8_t spare;
  uint32_t common_tunnel_endpoint_identifier;
  uint8_t distribution_address_length : 6;
  uint8_t distribution_address_type : 2;
  std::string ip_multicast_distribution_address;
  uint8_t source_address_length : 6;
  uint8_t source_address_type : 2;
  std::string ip_source_address;
} multicast_transport_information_t;

//-------------------------------------
// 8.2.208 MBSN4mbReq-Flags (type 307)
typedef struct mbsn4mbreq_flags_s {
  uint8_t pllssm : 1;
  uint8_t jmbssm : 1;
  uint8_t mbs_resti : 1;
  uint8_t spare : 5;
} mbsn4mbreq_flags_t;

//-------------------------------------
// 8.2.209 Local Ingress Tunnel (type 308)
typedef struct local_ingress_tunnel_s {
  uint8_t v4 : 1;
  uint8_t v6 : 1;
  uint8_t ch : 1;  // CHOOSE
  uint8_t spare : 5;
  uint16_t udp_port_number;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
} local_ingress_tunnel_t;

//-------------------------------------
// 8.2.210 MBS Unicast Parameters ID (type 309)
typedef struct mbs_unicast_parameters_id_s {
  uint16_t mbs_unicast_parameters_id;
} mbs_unicast_parameters_id_t;

//-------------------------------------
// 8.2.211 MBSN4Resp-Flags (type 312)
typedef struct mbsn4resp_flags_s {
  uint8_t nn19dt : 1;  // No N19mb Data Transfer
  uint8_t jmti : 1;    // Joined N19mb Multicast Transport
  uint8_t n19dtr : 1;  // N19mb Data Transfer Request
  uint8_t spare : 5;
} mbsn4resp_flags_t;

//-------------------------------------
// 8.2.212 Tunnel Password (type 313)
typedef struct tunnel_password_s {
  std::string tunnel_password;
} tunnel_password_t;

//-------------------------------------
// 8.2.213 Area Session ID (type 314)
typedef struct area_session_id_s {
  uint16_t area_session_id;
} area_session_id_t;

//-------------------------------------
// 8.2.214 DSCP to PPI Mapping Information (type 317)
// One octet of the DSCP list: Spare(2) | DSCP Value(6), per Figure 8.2.214-1.
typedef struct dscp_value_octet_s {
  uint8_t dscp_value : 6;  // 0..63 (DSCP in IPv4 TOS / IPv6 TC, RFC 2474)
  uint8_t spare : 2;
} dscp_value_octet_t;

typedef struct dscp_to_ppi_mapping_information_s {
  // Octet 5: Spare(5) | PPI Value(3), value 0..7
  uint8_t ppi_value : 3;
  uint8_t spare : 5;
  // Octets 6..(5+x): the mapped DSCP values (x = 1..64), each a full octet
  // encoded as Spare(2) | DSCP Value(6).
  std::vector<dscp_value_octet_t> dscp_values;
} dscp_to_ppi_mapping_information_t;

//-------------------------------------
// 8.2.215 PFCPSDRsp-Flags (type 318)
typedef struct pfcpsdrsp_flags_s {
  uint8_t puru : 1;
  uint8_t spare : 7;
} pfcpsdrsp_flags_t;

//-------------------------------------
// 8.2.216 QER Indications (type 319)
typedef struct qer_indications_s {
  uint8_t iqfisn : 1;
  uint8_t spare : 7;
} qer_indications_t;

//-------------------------------------
// 8.2.217 Vendor-Specific Node Report Type (type 320)
typedef struct vendor_specific_node_report_type_s {
  uint16_t enterprise_id;
  uint8_t spare;  // octet 7 (all spare in this release)
} vendor_specific_node_report_type_t;

//-------------------------------------
// 8.2.218 Configured Time Domain (type 321)
typedef struct configured_time_domain_s {
  uint8_t ctdi : 1;
  uint8_t spare : 7;
} configured_time_domain_t;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Specific IEs
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//  // Table 7.4.3.1-3: PFD
//  typedef struct pfd_s {
//    pfd_contents_t pfd_contents;
//  } pfd_t;
//
//  // Table 7.4.3.1-2: Application ID's PFDs
//  typedef struct application_ids_pfds_s {
//    application_id_t application_id;
//    std::vector<pfd_t> pfds;
//  } application_ids_pfds_t;
//
//  // Table 7.4.5.1.2-1: User Plane Path Failure IE within PFCP Node Report
//  Request typedef struct user_plane_path_failure_report_s {
//    remote_gtp_u_peer_t remote_gtp_u_peer;
//  } user_plane_path_failure_report_t;
//
//  // Table 7.5.2.2-3: Ethernet Packet Filter IE within PFCP Session
//  Establishment Request typedef struct ethernet_packet_filter_s {
//    ethernet_filter_id_t        ethernet_filter_id;
//    ethernet_filter_properties_t ethernet_filter_properties;
//    mac_address_t               mac_address;
//    ethertype_t                 ethertype;
//    c_tag_t                     c_tag;
//    s_tag_t                     s_tag;
//    sdf_filter_t                sdf_filter;
//  } ethernet_packet_filter_t;
//
//  // Table 7.5.2.2-2: PDI IE within PFCP Session Establishment Request
//  typedef struct pdi_s {
//    source_interface_t          source_interface;
//    fteid_t                     local_fteid;
//    network_instance_t          network_instance;
//    ue_ip_address_t             ue_ip_address;
//    traffic_endpoint_id_t       traffic_endpoint_id;
//    sdf_filter_t                sdf_filter;
//    application_id_t            application_id;
//    ethernet_packet_filter_t    ethernet_packet_filter;
//    qfi_t                       qfi;
//    framed_route_t              framed_route;
//    framed_routing_t            framed_routing;
//    framed_ipv6_route_t         framed_ipv6_route;
//  } pdi_t;
//
//  // Table 7.5.2.2-1: Create PDR IE within PFCP Session Establishment Request
//  typedef struct create_pdr_s {
//    pdr_id_t                    pdr_id;
//    precedence_t                precedence;
//    pdi_t                       pdi;
//    far_id_t                    far_id;
//    urr_id_t                    urr_id;
//    qer_id_t                    qer_id;
//    activate_predefined_rules_t activate_predefined_rules;
//  } create_pdr_t;
//
//  // Table 7.5.2.3-2: Forwarding Parameters IE in FAR
//  typedef struct forwarding_parameters_s {
//    destination_interface_t     destination_interface;
//    network_instance_t          network_instance;
//    redirect_information_t      redirect_information;
//    outer_header_creation_t     outer_header_creation;
//    transport_level_marking_t   transport_level_marking;
//    forwarding_policy_t         forwarding_policy;
//    header_enrichment_t         header_enrichment;
//    traffic_endpoint_id_t       linked_traffic_endpoint_id_t;
//    proxying_t                  proxying;
//  } forwarding_parameters_t;
//
//  // Table 7.5.2.3-3: Duplicating Parameters IE in FAR
//  typedef struct duplicating_parameters_s {
//    destination_interface_t     destination_interface;
//    outer_header_creation_t     outer_header_creation;
//    transport_level_marking_t   transport_level_marking;
//    forwarding_policy_t         forwarding_policy;
//  } duplicating_parameters_t;
//
//  // Table 7.5.2.3-1: Create FAR IE within PFCP Session Establishment Request
//  typedef struct create_far_s {
//    far_id_t                    far_id;
//    apply_action_t              apply_action;
//    forwarding_parameters_t     forwarding_parameters;
//    duplicating_parameters_t    duplicating_parameters;
//    bar_id_t                    bar_id;
//  } create_far_t;
//
//  // Table 7.5.2.4-2: Aggregated URRs
//  typedef struct aggregated_urrs_s {
//    aggregated_urr_id_t         aggregated_urr_id;
//    multiplier_t                multiplier;
//  } aggregated_urrs_t;
//
//  // Table 7.5.2.4-3: Additional Monitoring Time
//  typedef struct additional_monitoring_time_s {
//    monitoring_time_t           monitoring_time;
//    subsequent_volume_threshold_t subsequent_volume_threshold;
//    subsequent_time_threshold_t subsequent_time_threshold;
//    subsequent_volume_quota_t   subsequent_volume_quota;
//    subsequent_time_quota_t     subsequent_time_quota;
//  } additional_monitoring_time_t;
//
//  // Table 7.5.2.4-4: Event Information
//  typedef struct event_information_s {
//    event_id_t           event_id;
//    event_threshold_t    event_threshold;
//  } event_information_t;
//
//
//  // Table 7.5.2.4-1: Create URR IE within PFCP Session Establishment Request
//  typedef struct create_urr_s {
//    urr_id_t                    urr_id;
//    measurement_method_t        measurement_method;
//    reporting_triggers_t        reporting_triggers;
//    measurement_period_t        measurement_period;
//    volume_threshold_t          volume_threshold;
//    volume_quota_t              volume_quota;
//    time_threshold_t            time_threshold;
//    time_quota_t                time_quota;
//    quota_holding_time_t        quota_holding_time;
//    dropped_dl_traffic_threshold_t  dropped_dl_traffic_threshold;
//    monitoring_time_t           monitoring_time;
//    event_information_t         event_information;
//    subsequent_volume_threshold_t subsequent_volume_threshold;
//    subsequent_time_threshold_t subsequent_time_threshold;
//    subsequent_volume_quota_t   subsequent_volume_quota;
//    subsequent_time_quota_t     subsequent_time_quota;
//    inactivity_detection_time_t inactivity_detection_time;
//    linked_urr_id_t             linked_urr_id;
//    measurement_information_t   measurement_information;
//    time_quota_mechanism_t      time_quota_mechanism;
//    aggregated_urrs_t           aggregated_urrs;
//    far_id_t                    far_id_for_quota_action;
//    ethernet_inactivity_timer_t ethernet_inactivity_timer;
//    additional_monitoring_time_t additional_monitoring_time;
//  } create_urr_t;
//
//  // Table 7.5.2.5-1: Create QER IE within PFCP Session Establishment Request
//  typedef struct create_qer_s {
//    qer_id_t                    qer_id;
//    qer_correlation_id_t        qer_correlation_id;
//    gate_status_t               gate_status;
//    mbr_t                       maximum_bitrate;
//    gbr_t                       guaranteed_bitrate;
//    packet_rate_t               packet_rate;
//    dl_flow_level_marking_t     dl_flow_level_marking;
//    qfi_t                       qos_flow_identifier;
//    rqi_t                       reflective_qos;
//  } create_qer_t;
//
//  // Table 7.5.2.6-1: Create BAR IE within PFCP Session Establishment Request
//  typedef struct create_bar_s {
//    bar_id_t                    bar_id;
//    downlink_data_notification_delay_t  downlink_data_notification_delay;
//    suggested_buffering_packets_count_t suggested_buffering_packets_count;
//  } create_bar_t;
//
//  // Table 7.5.2.7-1: Create Traffic Endpoint IE within PFCP Session
//  Establishment Request typedef struct create_traffic_endpoint_s {
//    traffic_endpoint_id_t       traffic_endpoint_id;
//    fteid_t                     local_fteid;
//    network_instance_t          network_instance;
//    ue_ip_address_t             ue_ip_address;
//    ethernet_pdu_session_information_t ethernet_pdu_session_information;
//    framed_route_t              framed_route;
//    framed_routing_t            framed_routing;
//    framed_ipv6_route_t         framed_ipv6_route;
//  } create_traffic_endpoint_t;
//
//  // Table 7.5.3.2-1: Created PDR IE within PFCP Session Establishment
//  Response typedef struct created_pdr_s {
//    pdr_id_t                    pdr_id;
//    fteid_t                     local_fteid;
//  } created_pdr_t;
//
//  // Table 7.5.3.3-1: Load Control Information IE within PFCP Session
//  Establishment Response typedef struct load_control_information_s {
//    sequence_number_t           load_control_sequence_number;
//    metric_t                    load_metric;
//  } load_control_information_t;
//
//  // Table 7.5.3.4-1: Overload Control Information IE within PFCP Session
//  Establishment Response typedef struct overload_control_information_s {
//    sequence_number_t           overload_control_sequence_number;
//    metric_t                    overload_reduction_metric;
//    timer_t                     period_of_validity;
//    oci_flags_t                 overload_control_information_flags;
//  } overload_control_information_t;
//
//  // Table 7.5.4.2-1: Update PDR IE within PFCP Session Modification Request
//  typedef struct update_pdr_s {
//    pdr_id_t                    pdr_id;
//    outer_header_removal_t      outer_header_removal;
//    precedence_t                precedence;
//    pdi_t                       pdi;
//    far_id_t                    far_id;
//    urr_id_t                    urr_id;
//    qer_id_t                    qer_id;
//    activate_predefined_rules_t activate_predefined_rules;
//    deactivate_predefined_rules_t deactivate_predefined_rules;
//  } update_pdr_t;
//
//  // Table 7.5.4.3-2: Update Forwarding Parameters IE in FAR
//  typedef struct update_forwarding_parameters_s {
//    destination_interface_t     destination_interface;
//    network_instance_t          network_instance;
//    outer_header_creation_t     outer_header_creation;
//    transport_level_marking_t   transport_level_marking;
//    forwarding_policy_t         forwarding_policy;
//    header_enrichment_t         header_enrichment;
//    pfcpsmreq_flags_t           pfcpsmreq_flags;
//    traffic_endpoint_id_t       linked_traffic_endpoint_id;
//  } update_forwarding_parameters_t;
//
//  // Table 7.5.4.3-3: Update Duplicating Parameters IE in FAR
//  typedef struct update_duplicating_parameters_s {
//    destination_interface_t     destination_interface;
//    outer_header_creation_t     outer_header_creation;
//    transport_level_marking_t   transport_level_marking;
//    forwarding_policy_t         forwarding_policy;
//  } update_duplicating_parameters_t;
//
//  // Table 7.5.4.3-1: Update FAR IE within PFCP Session Modification Request
//  typedef struct update_far_s {
//    far_id_t                    far_id;
//    apply_action_t              apply_action;
//    update_forwarding_parameters_t update_forwarding_parameters;
//    update_duplicating_parameters_t update_duplicating_parameters;
//    bar_id_t                    bar_id;
//  } update_far_t;
//
//  // Table 7.5.4.4-1: Update URR IE within PFCP Session Modification Request
//  typedef struct update_urr_s {
//    urr_id_t                    urr_id;
//    measurement_method_t        measurement_method;
//    reporting_triggers_t        reporting_triggers;
//    measurement_period_t        measurement_period;
//    volume_threshold_t          volume_threshold;
//    volume_quota_t              volume_quota;
//    time_threshold_t            time_threshold;
//    time_quota_t                time_quota;
//    quota_holding_time_t        quota_holding_time;
//    dropped_dl_traffic_threshold_t  dropped_dl_traffic_threshold;
//    monitoring_time_t           monitoring_time;
//    event_information_t         event_information;
//    subsequent_volume_threshold_t subsequent_volume_threshold;
//    subsequent_time_threshold_t subsequent_time_threshold;
//    subsequent_volume_quota_t   subsequent_volume_quota;
//    subsequent_time_quota_t     subsequent_time_quota;
//    inactivity_detection_time_t inactivity_detection_time;
//    linked_urr_id_t             linked_urr_id;
//    measurement_information_t   measurement_information;
//    time_quota_mechanism_t      time_quota_mechanism;
//    aggregated_urrs_t           aggregated_urrs;
//    far_id_t                    far_id_for_quota_action;
//    ethernet_inactivity_timer_t ethernet_inactivity_timer;
//    additional_monitoring_time_t additional_monitoring_time;
//  } update_urr_t;
//
//  // Table 7.5.4.5-1: Update QER IE within PFCP Session Modification Request
//  typedef struct update_qer_s {
//    qer_id_t                    qer_id;
//    qer_correlation_id_t        qer_correlation_id;
//    gate_status_t               gate_status;
//    mbr_t                       maximum_bitrate;
//    gbr_t                       guaranteed_bitrate;
//    packet_rate_t               packet_rate;
//    dl_flow_level_marking_t     dl_flow_level_marking;
//    qfi_t                       qos_flow_identifier;
//    rqi_t                       reflective_qos;
//  } update_qer_t;
//
//  // Table 7.5.4.6-1: Remove PDR IE within PFCP Session Modification Request
//  typedef struct remove_pdr_s {
//    pdr_id_t                    pdr_id;
//  } remove_pdr_t;
//
//  // Table 7.5.4.7-1: Remove FAR IE within PFCP Session Modification Request
//  typedef struct remove_far_s {
//    far_id_t                    far_id;
//  } remove_far_t;
//
//  // Table 7.5.4.8-1: Remove URR IE within PFCP Session Modification Request
//  typedef struct remove_urr_s {
//    urr_id_t                    urr_id;
//  } remove_urr_t;
//
//  // Table 7.5.4.9-1: Remove QER IE PFCP Session Modification Request
//  typedef struct remove_qer_s {
//    qer_id_t                    qer_id;
//  } remove_qer_t;
//
//  // Table 7.5.4.10-1: Query URR IE within PFCP Session Modification Request
//  typedef struct query_urr_s {
//    urr_id_t                    urr_id;
//  } query_urr_t;
//
//  // Table 7.5.4.11-1: Update BAR IE within PFCP Session Modification Request
//  typedef struct update_bar_within_pfcp_session_modification_request_s {
//    bar_id_t                            bar_id;
//    downlink_data_notification_delay_t  downlink_data_notification_delay;
//    suggested_buffering_packets_count_t suggested_buffering_packets_count;
//  } update_bar_within_pfcp_session_modification_request_t;
//
//  // Table 7.5.4.12-1: Remove BAR IE within PFCP Session Modification Request
//  typedef struct remove_bar_s {
//    bar_id_t                            bar_id;
//  } remove_bar_t;
//
//
//  // Table 7.5.3.5-1: Created Traffic Endpoint IE within Sx Session
//  Establishment Response typedef struct created_traffic_endpoint_s {
//    traffic_endpoint_id_t       traffic_endpoint_id;
//    fteid_t                     local_fteid;
//  } created_traffic_endpoint_t;
//
//  // Table 7.5.4.13-1: Update Traffic Endpoint IE within Sx Session
//  Modification Request typedef struct update_traffic_endpoint_s {
//    traffic_endpoint_id_t       traffic_endpoint_id;
//    fteid_t                     local_fteid;
//    network_instance_t          network_instance;
//    ue_ip_address_t             ue_ip_address;
//    framed_route_t              framed_route;
//    framed_routing_t            framed_routing;
//    framed_ipv6_route_t         framed_ipv6_route;
//  } update_traffic_endpoint_t;
//
//  // Table 7.5.4.14-1: Remove Traffic Endpoint IE within Sx Session
//  Modification Request typedef struct remove_traffic_endpoint_s {
//    traffic_endpoint_id_t       traffic_endpoint_id;
//  } remove_traffic_endpoint_t;
//
//  // Table 7.5.8.3-3: Ethernet Traffic Information IE within Usage Report IE
//  typedef struct ethernet_traffic_information_s {
//    mac_addresses_detected_t             mac_addresses_detected;
//    mac_addresses_removed_t              mac_addresses_removed;
//  } ethernet_traffic_information_t;
//
//  // Table 7.5.5.2-1: Usage Report IE within PFCP Session Modification
//  Response typedef struct
//  usage_report_within_pfcp_session_modification_response_s {
//    urr_id_t                    urr_id;
//    ur_seqn_t                   ur_seqn;
//    usage_report_trigger_t      usage_report_trigger;
//    start_time_t                start_time;
//    end_time_t                  end_time;
//    volume_measurement_t        volume_measurement;
//    duration_measurement_t      duration_measurement;
//    time_of_first_packet_t      time_of_first_packet;
//    time_of_last_packet_t       time_of_last_packet;
//    usage_information_t         usage_information;
//    query_urr_reference_t       query_urr_reference;
//    ethernet_traffic_information_t ethernet_traffic_information;
//  } usage_report_within_pfcp_session_modification_response_t;
//
//  // Table 7.5.7.2-1: Usage Report IE within PFCP Session Deletion Response
//  typedef struct usage_report_within_pfcp_session_deletion_response_s {
//    urr_id_t                    urr_id;
//    ur_seqn_t                   ur_seqn;
//    usage_report_trigger_t      usage_report_trigger;
//    start_time_t                start_time;
//    end_time_t                  end_time;
//    volume_measurement_t        volume_measurement;
//    duration_measurement_t      duration_measurement;
//    time_of_first_packet_t      time_of_first_packet;
//    time_of_last_packet_t       time_of_last_packet;
//    usage_information_t         usage_information;
//    ethernet_traffic_information_t ethernet_traffic_information;
//  } usage_report_within_pfcp_session_deletion_response_t;
//
//  // Table 7.5.8.2-1: Downlink Data Report IE within PFCP Session Report
//  Request typedef struct downlink_data_report_s {
//    pdr_id_t                    pdr_id;
//    downlink_data_service_information_t  downlink_data_service_information;
//  } downlink_data_report_t;
//
//  // Table 7.5.8.3-2: Application Detection Information IE within Usage Report
//  IE typedef struct application_detection_information_s {
//    application_id_t                    application_id;
//    application_instance_id_t           application_instance_id;
//    flow_information_t                  flow_information;
//  } application_detection_information_t;
//
//  // Table 7.5.8.3-2A: Event Reporting IE within Usage Report IE
//  typedef struct event_reporting_s {
//    event_id_t                    event_id;
//  } event_reporting_t;
//
//  //
//  // Table 7.5.8.3-1: Usage Report IE within PFCP Session Report Request
//  typedef struct usage_report_within_pfcp_session_report_request_s {
//    urr_id_t                    urr_id;
//    ur_seqn_t                   ur_seqn;
//    usage_report_trigger_t      usage_report_trigger;
//    start_time_t                start_time;
//    end_time_t                  end_time;
//    volume_measurement_t        volume_measurement;
//    duration_measurement_t      duration_measurement;
//    application_detection_information_t application_detection_information;
//    ue_ip_address_t             ue_ip_address;
//    network_instance_t          network_instance;
//    time_of_first_packet_t      time_of_first_packet;
//    time_of_last_packet_t       time_of_last_packet;
//    usage_information_t         usage_information;
//    query_urr_reference_t       query_urr_reference;
//    event_reporting_t           event_reporting;
//    ethernet_traffic_information_t ethernet_traffic_information;
//  } usage_report_within_pfcp_session_report_request_t;
//
//  // Table 7.5.8.4-1: Error Indication Report IE within PFCP Session Report
//  Request typedef struct error_indication_report_s {
//    fteid_t                     remote_fteid;
//  } error_indication_report_t;
//
//  // Table 7.5.9.2-1: Update BAR IE in PFCP Session Report Response
//  typedef struct update_bar_within_pfcp_session_report_response_s {
//    bar_id_t                              bar_id;
//    downlink_data_notification_delay_t    downlink_data_notification_delay;
//    dl_buffering_duration_t               dl_buffering_duration;
//    dl_buffering_suggested_packet_count_t
//    dl_buffering_buffering_suggested_packet_count;
//    suggested_buffering_packets_count_t   suggested_buffering_packets_count;
//  } update_bar_within_pfcp_session_report_response_t;
}  // namespace pfcp

namespace std {

template<>
struct hash<pfcp::fseid_t> {
  std::size_t operator()(const pfcp::fseid_t& k) const {
    using std::hash;
    using std::size_t;
    std::size_t h;
    if (k.v4) {
      h = k.ipv4_address.s_addr ^ (k.seid);
      return h;
    }
    if (k.v6) {
      h = k.seid ^ (k.ipv6_address.s6_addr32[0] ^ k.ipv6_address.s6_addr32[1] ^
                    k.ipv6_address.s6_addr32[2] ^ k.ipv6_address.s6_addr32[3]);
      return h;
    }
    return k.seid;
  }
};

template<>
class hash<pfcp::node_id_t> {
 public:
  size_t operator()(const pfcp::node_id_t& k) const {
    using std::hash;
    using std::size_t;
    std::size_t h = 0;
    switch (k.node_id_type) {
      case pfcp::NODE_ID_TYPE_IPV4_ADDRESS:
        h = std::hash<uint32_t>()(k.u1.ipv4_address.s_addr);
        return h;
        break;
      case pfcp::NODE_ID_TYPE_IPV6_ADDRESS:
        h = k.u1.ipv6_address.s6_addr32[0] ^ k.u1.ipv6_address.s6_addr32[1] ^
            k.u1.ipv6_address.s6_addr32[2] ^ k.u1.ipv6_address.s6_addr32[3];
        return h;
        break;
      case pfcp::NODE_ID_TYPE_FQDN:
        h = std::hash<std::string>()(k.fqdn);
        return h;
      default:
        return h;
    }
  }
};
}  // namespace std
#endif /* FILE_3GPP_129_244_R17_H_SEEN */
