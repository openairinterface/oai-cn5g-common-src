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

#ifndef _PDU_SESSION_ESTABLISHMENT_REQUEST_H_
#define _PDU_SESSION_ESTABLISHMENT_REQUEST_H_

#include <bstrlib.h>

#include <vector>

#include "NasIeHeader.hpp"
#include "Nas5gsmHeader.hpp"

namespace oai::nas {

class PduSessionEstablishmentRequest : public Nas5gsmHeader {
 public:
	PduSessionEstablishmentRequest();
  ~PduSessionEstablishmentRequest();

  int Encode(uint8_t* buf, int len) override;
  int Decode(uint8_t* buf, int len) override;

  uint32_t GetLength() const override;

 private:
  //Mandatory
  Nas5gsmHeader ie_header_;
  //Integrity protection maximum data rate

  //Optional
  //PDU session type
  //SSC mode
  //5GSM capability
  //Maximum number of supported packet filters
  //Always-on PDU session requested
  //SM PDU DN request container
  //Extended protocol configuration options
  //IP header compression configuration
  //DS-TT Ethernet port MAC address
  //UE-DS-TT residence time
  //Port management information container
  //Ethernet header compression configuration
  //Suggested interface identifier


  //ACCEPT


  //Selected PDU session type
  //Selected SSC mode
  //Authorized QoS rules
  //Session AMBR

  //Optional
  //5GSM cause
  //PDU address
  //RQ timer value
  //S-NSSAI
  //Always-on PDU session indication
  //Mapped EPS bearer contexts
  //EAP message
  //Authorized QoS flow descriptions
  //Extended protocol configuration
  //Extended protocol configuration options
  //DNN
  //5GSM network feature support
  //Serving PLMN rate control
  //ATSSS container
  //Control plane only indication
  //IP header compression
  //IP header compression configuration configuration
  //Ethernet header compression configuration
};

}  // namespace oai::nas

#endif
