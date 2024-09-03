/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file 3gpp_conversions.cpp
 * \brief
 * \author Lionel Gauthier
 * \company Eurecom
 * \email: lionel.gauthier@eurecom.fr
 */
#include "3gpp_conversions.hpp"
#include "3gpp_29.510.h"
#include "3gpp_24.501.hpp"

#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <inttypes.h>

using namespace oai::utils;

//------------------------------------------------------------------------------
void xgpp_conv::pdu_session_type_to_pdn_type(
    const pdu_session_type_t& pdu_session_type, pdn_type_t& pdn_type) {
  switch (pdu_session_type.pdu_session_type) {
    case PDU_SESSION_TYPE_E_IPV4:
      pdn_type.pdn_type = PDN_TYPE_E_IPV4;
      break;
    case PDU_SESSION_TYPE_E_IPV6:
      pdn_type.pdn_type = PDN_TYPE_E_IPV6;
      break;
    case PDU_SESSION_TYPE_E_IPV4V6:
      pdn_type.pdn_type = PDN_TYPE_E_IPV4V6;
      break;
    case PDU_SESSION_TYPE_E_UNSTRUCTURED:
      pdn_type.pdn_type = PDN_TYPE_E_NON_IP;
      break;
    case PDU_SESSION_TYPE_E_ETHERNET:
      pdn_type.pdn_type = PDN_TYPE_E_ETHERNET;
      break;
    default:
      pdn_type.pdn_type = PDN_TYPE_E_UNKNOWN;
      break;
  }
}

//------------------------------------------------------------------------------
void xgpp_conv::pdn_type_to_pdu_session_type(
    const pdn_type_t& pdn_type, pdu_session_type_t& pdu_session_type) {
  switch (pdn_type.pdn_type) {
    case PDN_TYPE_E_IPV4:
      pdu_session_type.pdu_session_type = PDU_SESSION_TYPE_E_IPV4;
      break;
    case PDN_TYPE_E_IPV6:
      pdu_session_type.pdu_session_type = PDU_SESSION_TYPE_E_IPV6;
      break;
    case PDN_TYPE_E_IPV4V6:
      pdu_session_type.pdu_session_type = PDU_SESSION_TYPE_E_IPV4V6;
      break;
    case PDN_TYPE_E_NON_IP:
      pdu_session_type.pdu_session_type = PDU_SESSION_TYPE_E_UNSTRUCTURED;
      break;
    case PDN_TYPE_E_ETHERNET:
      pdu_session_type.pdu_session_type = PDU_SESSION_TYPE_E_ETHERNET;
      break;
    default:
      pdu_session_type.pdu_session_type = PDU_SESSION_TYPE_E_UNKNOWN;
      break;
  }
}

//------------------------------------------------------------------------------
void xgpp_conv::paa_to_pfcp_ue_ip_address(
    const paa_t& paa, pfcp::ue_ip_address_t& ue_ip_address) {
  switch (paa.pdu_session_type.pdu_session_type) {
    case PDU_SESSION_TYPE_E_IPV4:
      ue_ip_address.v4           = 1;
      ue_ip_address.ipv4_address = paa.ipv4_address;
      break;
    case PDU_SESSION_TYPE_E_IPV6:
      ue_ip_address.v6           = 1;
      ue_ip_address.ipv6_address = paa.ipv6_address;
      break;
    case PDU_SESSION_TYPE_E_IPV4V6:
      ue_ip_address.v4           = 1;
      ue_ip_address.v6           = 1;
      ue_ip_address.ipv4_address = paa.ipv4_address;
      ue_ip_address.ipv6_address = paa.ipv6_address;
      break;
    case PDU_SESSION_TYPE_E_UNSTRUCTURED:
    case PDU_SESSION_TYPE_E_ETHERNET:
    default:;
  }
}

//------------------------------------------------------------------------------
void xgpp_conv::pdu_session_ip_to_pfcp_ue_ip_address(
    const pdu_session_type_t& pdu_session_type,
    const struct in_addr& ipv4_address, const struct in6_addr ipv6_address,
    pfcp::ue_ip_address_t& ue_ip_address) {
  switch (pdu_session_type.pdu_session_type) {
    case PDU_SESSION_TYPE_E_IPV4:
      ue_ip_address.v4           = 1;
      ue_ip_address.ipv4_address = ipv4_address;
      break;
    case PDU_SESSION_TYPE_E_IPV6:
      ue_ip_address.v6           = 1;
      ue_ip_address.ipv6_address = ipv6_address;
      break;
    case PDU_SESSION_TYPE_E_IPV4V6:
      ue_ip_address.v4           = 1;
      ue_ip_address.v6           = 1;
      ue_ip_address.ipv4_address = ipv4_address;
      ue_ip_address.ipv6_address = ipv6_address;
      break;
    case PDU_SESSION_TYPE_E_UNSTRUCTURED:
    case PDU_SESSION_TYPE_E_ETHERNET:
    default:;
  }
}

//------------------------------------------------------------------------------
void xgpp_conv::pdn_ip_to_pfcp_ue_ip_address(
    const pdn_type_t& pdn_type, const struct in_addr& ipv4_address,
    const struct in6_addr ipv6_address, pfcp::ue_ip_address_t& ue_ip_address) {
  switch (pdn_type.pdn_type) {
    case PDN_TYPE_E_IPV4:
      ue_ip_address.v4           = 1;
      ue_ip_address.ipv4_address = ipv4_address;
      break;
    case PDN_TYPE_E_IPV6:
      ue_ip_address.v6           = 1;
      ue_ip_address.ipv6_address = ipv6_address;
      break;
    case PDN_TYPE_E_IPV4V6:
      ue_ip_address.v4           = 1;
      ue_ip_address.v6           = 1;
      ue_ip_address.ipv4_address = ipv4_address;
      ue_ip_address.ipv6_address = ipv6_address;
      break;
    case PDN_TYPE_E_NON_IP:
    default:;
  }
}

//------------------------------------------------------------------------------
void xgpp_conv::pfcp_to_core_fteid(
    const pfcp::fteid_t& pfteid, fteid_t& fteid) {
  fteid.v4                  = pfteid.v4;
  fteid.v6                  = pfteid.v6;
  fteid.ipv4_address.s_addr = pfteid.ipv4_address.s_addr;
  fteid.ipv6_address        = pfteid.ipv6_address;
  fteid.teid_gre_key        = pfteid.teid;
}
//------------------------------------------------------------------------------
void xgpp_conv::pfcp_from_core_fteid(
    pfcp::fteid_t& pfteid, const fteid_t& fteid) {
  pfteid.chid                = 0;
  pfteid.ch                  = 0;
  pfteid.choose_id           = 0;
  pfteid.v4                  = fteid.v4;
  pfteid.v6                  = fteid.v6;
  pfteid.ipv4_address.s_addr = fteid.ipv4_address.s_addr;
  pfteid.ipv6_address        = fteid.ipv6_address;
  pfteid.teid                = fteid.teid_gre_key;
}
//------------------------------------------------------------------------------
void xgpp_conv::pfcp_cause_to_core_cause(const pfcp::cause_t& pc, cause_t& c) {
  switch (pc.cause_value) {
    case pfcp::CAUSE_VALUE_REQUEST_ACCEPTED:
      c.cause_value = REQUEST_ACCEPTED;
      break;
    case pfcp::CAUSE_VALUE_REQUEST_REJECTED:
      c.cause_value = REQUEST_REJECTED;
      break;
    case pfcp::CAUSE_VALUE_SESSION_CONTEXT_NOT_FOUND:
    case pfcp::CAUSE_VALUE_MANDATORY_IE_MISSING:
    case pfcp::CAUSE_VALUE_CONDITIONAL_IE_MISSING:
    case pfcp::CAUSE_VALUE_INVALID_LENGTH:
    case pfcp::CAUSE_VALUE_MANDATORY_IE_INCORRECT:
    case pfcp::CAUSE_VALUE_INVALID_FORWARDING_POLICY:
    case pfcp::CAUSE_VALUE_INVALID_FTEID_ALLOCATION_OPTION:
    case pfcp::CAUSE_VALUE_NO_ESTABLISHED_PFCP_ASSOCIATION:
    case pfcp::CAUSE_VALUE_RULE_CREATION_MODIFICATION_FAILURE:
      c.cause_value = SYSTEM_FAILURE;  // ?
      break;
    case pfcp::CAUSE_VALUE_PFCP_ENTITY_IN_CONGESTION:
      c.cause_value = APN_CONGESTION;  // ? ...
      break;
    case pfcp::CAUSE_VALUE_NO_RESOURCES_AVAILABLE:
    case pfcp::CAUSE_VALUE_SERVICE_NOT_SUPPORTED:
    case pfcp::CAUSE_VALUE_SYSTEM_FAILURE:
    default:
      c.cause_value = SYSTEM_FAILURE;  // ? ...
  }
}

//------------------------------------------------------------------------------
bool xgpp_conv::endpoint_to_gtp_u_peer_address(
    const endpoint& ep, gtp_u_peer_address_t& peer_address) {
  switch (ep.family()) {
    case AF_INET: {
      const struct sockaddr_in* const sin =
          reinterpret_cast<const sockaddr_in* const>(&ep.addr_storage);
      peer_address.ipv4_address.s_addr = sin->sin_addr.s_addr;
      peer_address.is_v4               = true;
      return true;
    } break;
    case AF_INET6: {
      const struct sockaddr_in6* const sin6 =
          reinterpret_cast<const sockaddr_in6* const>(&ep.addr_storage);
      peer_address.ipv6_address = sin6->sin6_addr;
      peer_address.is_v4        = false;
      return true;
    } break;
    default:
      return false;
  }
}
