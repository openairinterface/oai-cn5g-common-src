################################################################################
# Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The OpenAirInterface Software Alliance licenses this file to You under
# the OAI Public License, Version 1.1  (the "License"); you may not use this file
# except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.openairinterface.org/?page_id=698
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#-------------------------------------------------------------------------------
# For more information about the OpenAirInterface (OAI) Software Alliance:
#      contact@openairinterface.org
################################################################################
cmake_minimum_required (VERSION 3.22.1)
project(UNIT_TESTING_COMMON_SRC)

enable_testing()
include(GoogleTest)

# Override options for COMMON
set (  PACKAGE_NAME                    "COMMON" )

set (NAS_TEST_DIR   ${NAS_DIR}/test)
set (NGAP_TEST_DIR  ${NGAP_DIR}/test)

include_directories(${3GPP_DIR})
include_directories(${COMMON_DIR})
include_directories(${LOGGER_DIR})
include_directories(${MODEL_DIR}/common_model)
include_directories(${NAS_DIR}/5gmm-msgs)
include_directories(${NAS_DIR}/common)
include_directories(${NAS_DIR}/ies)
include_directories(${NAS_DIR}/utils)
include_directories(${NGAP_DIR}/libngap)
include_directories(${NGAP_DIR}/libngap_ies)
include_directories(${NGAP_DIR}/ngap_ies)
include_directories(${NGAP_DIR}/ngap_msgs)
include_directories(${NGAP_DIR}/utils)
include_directories(${UTILS_DIR})
include_directories(${UTILS_DIR}/bstr)

add_executable(unit_tests 
  main.cpp
  ${NAS_TEST_DIR}/test_msg.cpp
  ${NGAP_TEST_DIR}/test_msg.cpp) 

target_link_libraries(unit_tests 
 -Wl,--start-group COMMON  NAS NGAP LOGGER UTILS -lboost_system 
 -lboost_thread -lboost_date_time -lfmt -lspdlog -lssl -Wl,--end-group
   gtest pthread m rt boost_system)

gtest_discover_tests(unit_tests)
