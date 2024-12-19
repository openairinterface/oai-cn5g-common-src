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

#ifndef _NAS_5GSM_MESSAGE_H_
#define _NAS_5GSM_MESSAGE_H_

#include "Nas5gsmHeader.hpp"
#include "NasMessage.hpp"

namespace oai::nas {

class Nas5gsmMessage : public NasMessage {
 public:
  Nas5gsmMessage(){};
  virtual ~Nas5gsmMessage(){};

  Nas5gsmMessage(
      uint8_t epd, uint8_t pdu_session_id, uint16_t procedure_transaction_id,
      uint8_t msg_type);
  Nas5gsmMessage(uint8_t epd, uint8_t msg_type);

  Nas5gsmMessage& operator=(const struct Nas5gsmMessage& nas_msg) {
    ie_header_ = nas_msg.ie_header_;
    return *this;
  }

  void SetHeader(
      uint8_t epd, uint8_t pdu_session_id, uint16_t procedure_transaction_id,
      uint8_t msg_type);
  void SetHeader(uint8_t pdu_session_id, uint16_t procedure_transaction_id);

  Nas5gsmHeader GetHeader() const;
  void GetHeader(Nas5gsmHeader& nas_header) const;

  uint32_t GetLength() const override;

  int Encode(uint8_t* buf, int len) override;
  int Decode(uint8_t* buf, int len) override;

 protected:
  Nas5gsmHeader ie_header_;  // Mandatory
};

}  // namespace oai::nas

#endif
