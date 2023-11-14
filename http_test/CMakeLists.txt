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

cmake_minimum_required(VERSION 3.16)

project(OAI-CN-COMMON)

set (OPENAIRCN_DIR   $ENV{OPENAIRCN_DIR})
set (BUILD_TOP_DIR   ${OPENAIRCN_DIR}/build)
set (SRC_TOP_DIR     $ENV{OPENAIRCN_DIR}/src)
set (MOUNTED_COMMON oai-cn5g-common-src)

set(NF_TARGET common_http_client_test)

set(CMAKE_CXX_STANDARD 17)

set(COMMON_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

set(TEST_TOP_DIR ${COMMON_SRC_DIR}/http_test)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

find_package(PkgConfig)
pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)

if (NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif ()

include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/config)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)

include(${COMMON_SRC_DIR}/http/http.cmake)
include(${COMMON_SRC_DIR}/logger/logger.cmake)

add_executable(common_http_client_test ${TEST_TOP_DIR}/integration/run_integration_tests.cpp
        ${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger/logger_base.cpp
        ${SRC_TOP_DIR}/${MOUNTED_COMMON}/http/http_client.cpp)

target_link_libraries(common_http_client_test PRIVATE cpr::cpr PkgConfig::Pistache spdlog::spdlog_header_only)

