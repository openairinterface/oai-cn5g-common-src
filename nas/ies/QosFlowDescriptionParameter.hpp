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

#ifndef _QOS_FLOW_DESCRIPTION_PARAMETER_H_
#define _QOS_FLOW_DESCRIPTION_PARAMETER_H_

#include <cstdint>
#include <optional>
#include <vector>

#include "3gpp_commons.h"
#include "Struct.hpp"

namespace oai::nas {
constexpr uint8_t kQosFlowDescriptionParameterMinimumLength =
    3;  // 1 for Identifier, 1 for length and at least 1 for contents

// Parameter Identifier
constexpr uint8_t kQosFlowDescriptionParameterIdentifier5qi             = 0x01;
constexpr uint8_t kQosFlowDescriptionParameterIdentifierGfbrUplink      = 0x02;
constexpr uint8_t kQosFlowDescriptionParameterIdentifierGfbrDownlink    = 0x03;
constexpr uint8_t kQosFlowDescriptionParameterIdentifierMfbrUplink      = 0x04;
constexpr uint8_t kQosFlowDescriptionParameterIdentifierMfbrDownlink    = 0x05;
constexpr uint8_t kQosFlowDescriptionParameterIdentifierAveragingWindow = 0x06;
constexpr uint8_t kQosFlowDescriptionParameterIdentifierEpsBearerIdentity =
    0x07;

class QosFlowDescriptionParameter {
 public:
  QosFlowDescriptionParameter();
  virtual ~QosFlowDescriptionParameter();

  int Encode(uint8_t* buf, int len) const;
  int Decode(const uint8_t* const buf, int len);

  uint8_t GetLength() const;

  uint8_t GetContentsLength() const;
  void SetContentsLength();

  void SetIdentifier(uint8_t id);
  void GetIdentifier(uint8_t& id) const;
  uint8_t GetIdentifier() const;

  void SetContents(const bstring& contents);
  bstring GetContents() const;
  void GetContents(bstring& contents) const;

  void Set5qi(uint8_t _5qi);
  void Get5qi(std::optional<uint8_t>& _5qi) const;
  std::optional<uint8_t> Get5qi() const;

  void SetGfbrUplink(const BitRate& gfbr_uplink);
  void GetGfbrUplink(std::optional<BitRate>& gfbr_uplink) const;
  std::optional<BitRate> GetGfbrUplink() const;

  void SetGfbrDownlink(const BitRate& gfbr_downlink);
  void GetGfbrDownlink(std::optional<BitRate>& gfbr_downlink) const;
  std::optional<BitRate> GetGfbrDownlink() const;

  void SetMfbrUplink(const BitRate& mfbr_uplink);
  void GetMfbrUplink(std::optional<BitRate>& mfbr_uplink) const;
  std::optional<BitRate> GetMfbrUplink() const;

  void SetMfbrDownlink(const BitRate& mfbr_downlink);
  void GetMfbrDownlink(std::optional<BitRate>& mfbr_downlink) const;
  std::optional<BitRate> GetMfbrDownlink() const;

  // TODO: Averaging window
  // TODO: EPS bearer identity
  std::optional<BitRate> GetBitRate() const;

 private:
  uint8_t identifier_;
  uint8_t length_;
  bstring contents_;

  void SetBitRate(const BitRate& bit_rate);
};

}  // namespace oai::nas

#endif
