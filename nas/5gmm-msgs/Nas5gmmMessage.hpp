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

#ifndef _NAS_5GMM_MESSAGE_H_
#define _NAS_5GMM_MESSAGE_H_

#include "3gpp_24.501.hpp"

namespace oai::nas {

class Nas5gmmMessage {
 public:
  Nas5gmmMessage(){};
  virtual ~Nas5gmmMessage() {}

  // May not be the actual length of the message (by rounding 1/2 octet to 1
  // octet in some IEs) but always greater than the actual length of the message
  virtual uint32_t GetLength() const = 0;
  virtual bool Validate(uint32_t len) const;

  virtual int Encode(uint8_t* buf, int len) = 0;
  virtual int Decode(uint8_t* buf, int len) = 0;

  void SetHeader(uint8_t security_header_type);

  void SetMessageName(const std::string& name);
  std::string GetMessageName() const;
  void GetMessageName(std::string& name) const;

 private:
  std::string msg_name_;  // non 3GPP IE
};

}  // namespace oai::nas

#endif
