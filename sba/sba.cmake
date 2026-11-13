# SPDX-License-Identifier: LicenseRef-CSSL-1.0

SET(SBA_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/sba)

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger/logger.cmake)
include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/common/common.cmake)

include_directories(${SBA_DIR})

SET(SBA_SRC_FILES
        ${SBA_DIR}/nf_event.cpp
        ${SBA_DIR}/nf_profile.cpp
        ${SBA_DIR}/nf_service.cpp
        ${SBA_DIR}/task_manager.cpp
)

## SBA used in NF_TARGET (main)
if (TARGET ${NF_TARGET})
    target_include_directories(${NF_TARGET} PUBLIC ${SBA_DIR})
    target_sources(${NF_TARGET} PRIVATE
            ${SBA_SRC_FILES}
    )
endif ()
