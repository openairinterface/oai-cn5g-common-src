# SPDX-License-Identifier: LicenseRef-CSSL-1.0

SET(OAM_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/oam)

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger/logger.cmake)
#include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/utils.cmake)
include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/common/common.cmake)

include_directories(${OAM_DIR})

SET(OAM_SRC_FILES
        ${OAM_DIR}/oam_server.cpp
)

## OAM used in NF_TARGET (main)
if (TARGET ${NF_TARGET})
    target_include_directories(${NF_TARGET} PUBLIC ${OAM_DIR})
    target_sources(${NF_TARGET} PRIVATE
            ${OAM_SRC_FILES}
    )
endif ()
