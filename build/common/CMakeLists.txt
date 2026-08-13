################################################################################
# SPDX-License-Identifier: LicenseRef-CSSL-1.0
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
set (PFCP_DIR       ${SRC_TOP_DIR}/pfcp)
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
