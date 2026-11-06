################################################################################
# SPDX-License-Identifier: LicenseRef-CSSL-1.0
################################################################################
cmake_minimum_required (VERSION 3.22.1)
project( PFCP )

# Used by the common-src unit tests only: each NF builds its own PFCP library
# from these sources (src/pfcp/CMakeLists.txt in oai-cn5g-smf and oai-cn5g-upf).

include_directories(${3GPP_DIR})
include_directories(${COMMON_DIR})
include_directories(${LOGGER_DIR})
include_directories(${PFCP_DIR})
include_directories(${UTILS_DIR})
include_directories(${UTILS_DIR}/bstr)

add_library(PFCP STATIC ${PFCP_DIR}/3gpp_29.244.cpp)

target_link_libraries( PFCP -Wl,--start-group COMMON UTILS LOGGER -lfmt -lspdlog -Wl,--end-group )
