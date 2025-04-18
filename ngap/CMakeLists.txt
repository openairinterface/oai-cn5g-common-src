################################################################################
#Licensed to the OpenAirInterface(OAI) Software Alliance under one or more
#contributor license agreements.See the NOTICE file distributed with
#this work for additional information regarding copyright ownership.
#The OpenAirInterface Software Alliance licenses this file to You under
#the OAI Public License, Version 1.1(the "License"); you may not use this file
#except in compliance with the License.
#You may obtain a copy of the License at
#
#http: // www.openairinterface.org/?page_id=698
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
#-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
#For more information about the OpenAirInterface(OAI) Software Alliance:
#contact @openairinterface.org
################################################################################
set(NGAP_DIR "${SRC_TOP_DIR}/ngap")
enable_testing()
include(GoogleTest)

include_directories(${3GPP_DIR})
include_directories(${COMMON_DIR})
include_directories(${LOGGER_DIR})
include_directories(${MODEL_DIR}/common_model)
include_directories(${NGAP_DIR}/libngap)
include_directories(${NGAP_DIR}/libngap_ies)
include_directories(${NGAP_DIR}/ngap_ies)
include_directories(${NGAP_DIR}/ngap_msgs)
include_directories(${NGAP_DIR}/utils)
include_directories(${UTILS_DIR})
include_directories(${UTILS_DIR}/bstr)

file(GLOB NGAP_src_files
    ${NGAP_DIR}/libngap/*.c
    ${NGAP_DIR}/libngap_ies/*.c
    ${NGAP_DIR}/ngap_ies/*.cpp
    ${NGAP_DIR}/ngap_msgs/*.cpp
    ${NGAP_DIR}/utils/*.cpp
)

add_library(NGAP STATIC ${NGAP_src_files})

target_link_libraries( NGAP -Wl,--start-group LOGGER COMMON -lfmt -lspdlog -Wl,--end-group )

