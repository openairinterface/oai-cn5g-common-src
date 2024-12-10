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

#include <cstdint>
#include <vector>
#include <optional>

#include "Struct.hpp"

namespace oai::nas {
constexpr uint8_t kQosRuleMinimumLength = 4;

constexpr uint8_t kQosRuleRuleOperationCodeReserved000           = 0;  // 0b000
constexpr uint8_t kQosRuleRuleOperationCodeCreateNewQosRule      = 1;  // 0b001
constexpr uint8_t kQosRuleRuleOperationCodeDeleteExistingQosRule = 2;  // 0b010
constexpr uint8_t
    kQosRuleRuleOperationCodeModifyExistingQosRuleAndAddPacketFilters =
        3;  // 0b011
constexpr uint8_t
    kQosRuleRuleOperationCodeModifyExistingQosRuleAndReplaceAllPacketFilters =
        4;  // 0b100
constexpr uint8_t
    kQosRuleRuleOperationCodeModifyExistingQosRuleAndDeletePacketFilters =
        5;  // 0b101
constexpr uint8_t
    kQosRuleRuleOperationCodeModifyExistingQosRuleWithoutModifyingPacketFilters =
        6;                                                   // 110
constexpr uint8_t kQosRuleRuleOperationCodeReserved111 = 7;  // 111

class QosRule {
 public:
  QosRule();
  virtual ~QosRule();

  int Encode(uint8_t* buf, int len) const;
  int Decode(const uint8_t* const buf, int len);

  uint16_t GetLength() const;
  void SetLength();

  void SetQosRuleId(uint8_t rule_id);
  void GetQosRuleId(uint8_t& rule_id) const;
  uint8_t GetQosRuleId() const;

  void SetRuleOperationCode(uint8_t code);
  void GetRuleOperationCode(uint8_t& code) const;
  uint8_t GetRuleOperationCode() const;

  void SetDqrBit(bool dqr);
  void GetDqrBit(bool& dqr) const;
  bool GetDqrBit() const;

  void SetNumberOfPacketFilters(uint8_t no_pf);
  void GetNumberOfPacketFilters(uint8_t& no_pf) const;
  uint8_t GetNumberOfPacketFilters() const;

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
  void GetPrecedence(std::optional<uint8_t>& precedence) const;
  std::optional<uint8_t> GetPrecedence() const;

  void SetSegregation(bool segregation);
  void GetSegregation(bool& segregation) const;
  bool GetSegregation() const;

  void SetQfi(uint8_t qfi);
  void GetQfi(std::optional<uint8_t>& qfi) const;
  std::optional<uint8_t> GetQfi() const;

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
  bool segregation_;
  std::optional<uint8_t> qfi_;
};

}  // namespace oai::nas

#endif
