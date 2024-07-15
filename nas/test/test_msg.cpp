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
#include "RegistrationRequest.hpp"
#include "endian.h"

#include <glog/logging.h>
#include <gtest/gtest.h>

#include <array>
#include <string>

using ::testing::Test;

extern std::vector<uint8_t> hexStringToByteArray(const std::string &hexString);

TEST(TestSuiteNasMsg, positiveTestingRegistrationRequest)
{
    /* Non - Access - Stratum 5GS(NAS)PDU
        Plain NAS 5GS Message
            Extended protocol discriminator: 5G mobility management messages (126)
            0000 .... = Spare Half Octet: 0
            .... 0000 = Security header type: Plain NAS message, not security protected (0)
            Message type: Registration request (0x41)
            5GS registration type
                .... 1... = Follow-On Request bit (FOR): Follow-on request pending
                .... .001 = 5GS registration type: initial registration (1)
            NAS key set identifier
                0... .... = Type of security context flag (TSC): Native security context (for KSIAMF)
                .001 .... = NAS key set identifier: 1
            5GS mobile identity
                Length: 13
                0... .... = Spare: 0
                .000 .... = SUPI format: IMSI (0)
                .... 0... = Spare: 0
                .... .001 = Type of identity: SUCI (1)
                Mobile Country Code (MCC): Unknown (1)
                Mobile Network Code (MNC): Unknown (01)
                Routing indicator: 0000
                .... 0000 = Protection scheme Id: NULL scheme (0)
                Home network public key identifier: 0
                MSIN: 0000000100
            UE security capability
                Element ID: 0x2e
                Length: 8
                1... .... = 5G-EA0: Supported
                .0.. .... = 128-5G-EA1: Not supported
                ..0. .... = 128-5G-EA2: Not supported
                ...0 .... = 128-5G-EA3: Not supported
                .... 0... = 5G-EA4: Not supported
                .... .0.. = 5G-EA5: Not supported
                .... ..0. = 5G-EA6: Not supported
                .... ...0 = 5G-EA7: Not supported
                0... .... = 5G-IA0: Not supported
                .0.. .... = 128-5G-IA1: Not supported
                ..1. .... = 128-5G-IA2: Supported
                ...0 .... = 128-5G-IA3: Not supported
                .... 0... = 5G-IA4: Not supported
                .... .0.. = 5G-IA5: Not supported
                .... ..0. = 5G-IA6: Not supported
                .... ...0 = 5G-IA7: Not supported
                0... .... = EEA0: Not supported
                .0.. .... = 128-EEA1: Not supported
                ..0. .... = 128-EEA2: Not supported
                ...0 .... = 128-EEA3: Not supported
                .... 0... = EEA4: Not supported
                .... .0.. = EEA5: Not supported
                .... ..0. = EEA6: Not supported
                .... ...0 = EEA7: Not supported
                0... .... = EIA0: Not supported
                .0.. .... = 128-EIA1: Not supported
                ..0. .... = 128-EIA2: Not supported
                ...0 .... = 128-EIA3: Not supported
                .... 0... = EIA4: Not supported
                .... .0.. = EIA5: Not supported
                .... ..0. = EIA6: Not supported
                .... ...0 = EIA7: Not supported */
    std::string msg_str =
        "7e004119000d0102f8290000000000000000112e08802000000000000000";
    //"1d7e004119000d0100f1100000000000000010002e088020000000000000";
    auto msg_bin = hexStringToByteArray(msg_str);

    oai::nas::RegistrationRequest rr = {};
    int decode_result = rr.Decode(msg_bin.data(), msg_bin.size() - 1);
    EXPECT_EQ(decode_result, msg_bin.size() - 1);

    std::cerr << "Msg size " << msg_bin.size() << std::endl;
    std::cerr << "Msg decoded size " << decode_result << std::endl;
    std::vector<uint8_t> msg_encoded_bin(msg_bin.size());
    int encode_result = rr.Encode(msg_encoded_bin.data(), msg_encoded_bin.size());
    EXPECT_EQ(encode_result, msg_encoded_bin.size());
}
