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
project(COMMON_SRC_PROJECT)

enable_testing()

# Override options for COMMON
set (  PACKAGE_NAME                    "COMMON" )
set (  STATIC_LINKING                   False )
#############################################
# Base directories, compatible with legacy OAI building
################################################
set (OPENAIRCN_DIR   $ENV{OPENAIRCN_DIR})
set (BUILD_TOP_DIR   ${OPENAIRCN_DIR}/build)
set (SRC_TOP_DIR     $ENV{OPENAIRCN_DIR})

set (3GPP_DIR       ${SRC_TOP_DIR}/3gpp)
set (COMMON_DIR     ${SRC_TOP_DIR}/common)
set (CONFIG_DIR     ${SRC_TOP_DIR}/config)
set (ITTI_DIR       ${SRC_TOP_DIR}/itti)
set (LOGGER_DIR     ${SRC_TOP_DIR}/logger)
set (MODEL_DIR      ${SRC_TOP_DIR}/model)
set (NAS_DIR        ${SRC_TOP_DIR}/nas)
set (NGAP_DIR       ${SRC_TOP_DIR}/ngap)
set (TEST_DIR       ${SRC_TOP_DIR}/test)
set (UTILS_DIR      ${SRC_TOP_DIR}/utils)

ADD_SUBDIRECTORY(${COMMON_DIR} ${CMAKE_CURRENT_BINARY_DIR}/common)
ADD_SUBDIRECTORY(${LOGGER_DIR} ${CMAKE_CURRENT_BINARY_DIR}/logger)
ADD_SUBDIRECTORY(${NAS_DIR} ${CMAKE_CURRENT_BINARY_DIR}/nas)
ADD_SUBDIRECTORY(${NGAP_DIR} ${CMAKE_CURRENT_BINARY_DIR}/ngap)
ADD_SUBDIRECTORY(${UTILS_DIR} ${CMAKE_CURRENT_BINARY_DIR}/utils)
# UNIT TESTING, should always be compiled along the main code to let
# it play its role.
ADD_SUBDIRECTORY(${TEST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/test)
