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

#ifndef _QOS_RULE_H_
#define _QOS_RULE_H_

#include "NasIeHeader.hpp"

namespace oai::nas {
constexpr uint8_t kQosRuleMinimumLength = 7;

class QosRule {
 public:
  QosRule();
  ~QosRule();

  int Encode(uint8_t* buf, int len) const;
  int Decode(uint8_t* buf, int len);

  uint16_t GetLength() const;

  void SetQosRuleId(uint8_t rule_id);
  void GetQosRuleId(uint8_t& rule_id) const;
  uint8_t GetQosRuleId() const;

  void SetRuleOperationCode(uint8_t code);
  void GetRuleOperationCode(uint8_t& code) const;
  uint8_t GetRuleOperationCode() const;

  void SetDqrBit(bool dqr);
  void GetDqrBit(bool& dqr) const;
  bool GetDqrBit() const;

  void SetNumberOfPacketFilter(uint8_t no_pf);
  void GetNumberOfPacketFilter(uint8_t& no_pf) const;
  uint8_t GetNumberOfPacketFilter() const;

  void SetPacketFilterModifyAndDeleteList(
      const std::vector<PacketFilterModifyAndDelete>& list);
  void GetPacketFilterModifyAndDeleteList(
      std::optional<std::vector<PacketFilterModifyAndDelete>>& list) const;
  std::optional<std::vector<PacketFilterModifyAndDelete>>
  GetPacketFilterModifyAndDeleteList() const;

  void SetPacketFilterCreateAndModifyAndReplaceList(
      const std::vector<PacketFilterCreateAndModifyAndReplace>& list);
  void GetPacketFilterCreateAndModifyAndReplaceList(
      std::optional<std::vector<PacketFilterCreateAndModifyAndReplace>>& list)
      const;
  std::optional<std::vector<PacketFilterCreateAndModifyAndReplace>>
  GetPacketFilterCreateAndModifyAndReplaceList() const;

  void SetPrecedence(uint8_t precedence);
  void GetPrecedence(uint8_t& precedence) const;
  uint8_t GetPrecedence() const;

  void SetSegregation(bool segregation);
  void GetSegregation(bool& segregation) const;
  bool GetSegregation() const;

  void SetQfi(uint8_t qfi);
  void GetQfi(uint8_t& qfi) const;
  uint8_t GetQfi() const;

 private:
  uint8_t qos_rule_id_;
  uint16_t length_;
  uint8_t rule_operation_code_;
  bool dqr_bit_;
  uint8_t number_of_packet_filters_;
  std::optional<std::vector<PacketFilterModifyAndDelete>>
      pf_modify_and_delete_list_;
  std::optional<std::vector<PacketFilterCreateAndModifyAndReplace>>
      pf_create_and_modify_and_replace_list_;
  std::optional<uint8_t> precedence_;
  std::optional<uint8_t> segregation_;
  std::optional<uint8_t> qfi_;
};

}  // namespace oai::nas

#endif
