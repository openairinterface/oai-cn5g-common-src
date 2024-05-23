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

#ifndef _NAS_MM_PLAIN_HEADER_H_
#define _NAS_MM_PLAIN_HEADER_H_

#include "3gpp_24.501.hpp"
#include "ExtendedProtocolDiscriminator.hpp"
#include "Nas5gmmMessage.hpp"
#include "NasMessageType.hpp"
#include "SecurityHeaderType.hpp"

constexpr uint8_t kNasMmPlainHeaderLength = 3;

namespace oai::nas {

class NasMmPlainHeader : public Nas5gmmMessage {
 public:
  NasMmPlainHeader() : Nas5gmmMessage(){};
  NasMmPlainHeader(uint8_t epd);
  NasMmPlainHeader(uint8_t epd, uint8_t msg_type);
  NasMmPlainHeader(uint8_t epd, uint8_t security_header_type, uint8_t msg_type);
  virtual ~NasMmPlainHeader();

  void SetHeader(uint8_t epd, uint8_t security_header_type, uint8_t msg_type);

  uint32_t GetLength() const override;
  bool Validate(uint32_t len) const override;

  int Encode(uint8_t* buf, int len) override;
  int Decode(uint8_t* buf, int len) override;

  void SetEpd(uint8_t epd);
  uint8_t GetEpd() const;

  void SetSecurityHeaderType(uint8_t type);
  uint8_t GetSecurityHeaderType() const;

  void SetMessageType(uint8_t type);
  uint8_t GetMessageType() const;

 private:
  ExtendedProtocolDiscriminator epd_;    // Mandatory
  SecurityHeaderType secu_header_type_;  // Mandatory (1/2 octet)
  // TODO: Spare half octet (1/2 octet)
  NasMessageType msg_type_;  // Mandatory

  std::string msg_name_;  // non 3GPP IE
};

}  // namespace oai::nas

#endif
