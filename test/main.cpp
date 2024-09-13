/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
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

#include "logger_base.hpp"
#include <gtest/gtest.h>

std::vector<uint8_t> hexStringToByteArray(const std::string& hexString) {
  std::vector<uint8_t> byteArray;
  if (hexString.length() % 2 == 1) {
    throw std::invalid_argument("Hex string to convert is not byte aligned");
  }
  // Loop through the hex string, two characters at a time
  for (size_t i = 0; i < hexString.length(); i += 2) {
    // Extract two characters representing a byte
    std::string byteString = hexString.substr(i, 2);

    // Convert the byte string to a uint8_t value
    uint8_t byteValue =
        static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));

    // Add the byte to the byte array
    byteArray.push_back(byteValue);
  }
  return byteArray;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  oai::logger::logger_registry::register_logger(
      "TEST", LOGGER_COMMON, true, false);
  return RUN_ALL_TESTS();
}
