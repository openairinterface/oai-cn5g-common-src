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
cmake_minimum_required (VERSION 3.22.1)
project( UTILS )

include_directories(${3GPP_DIR})
include_directories(${COMMON_DIR})
include_directories(${ITTI_DIR})
include_directories(${MODEL_DIR}/common_model)
include_directories(${UTILS_DIR})
include_directories(${UTILS_DIR}/bstr)
include_directories(${LOGGER_DIR})
include_directories(${NAS_DIR}/common)
include_directories(${NAS_DIR}/utils)

file(GLOB UTILS_src_files
    ${UTILS_DIR}/bstr/*.c
    ${UTILS_DIR}/*.cpp
    ${UTILS_DIR}/*.c
)

add_library(UTILS STATIC ${UTILS_src_files})

target_link_libraries( UTILS -Wl,--start-group LOGGER -lfmt -lyaml-cpp -Wl,--end-group )

