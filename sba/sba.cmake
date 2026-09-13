# SPDX-License-Identifier: LicenseRef-CSSL-1.0

SET(SBA_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/sba)

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger/logger.cmake)
include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/common/common.cmake)

include_directories(${SBA_DIR})

SET(SBA_SRC_FILES
        ${SBA_DIR}/nf_event.cpp
        ${SBA_DIR}/task_manager.cpp
        ${SBA_DIR}/http_client.cpp
)

# The HTTP/2 server (http2_server.cpp) pulls in libevent on top of nghttp2, so
# it is opt-in: an NF that serves SBI over it sets HTTP2_SERVER_ENABLED before
# including this file and adds `event event_pthreads` to its link line. NFs
# that only act as HTTP clients are unaffected.
if (HTTP2_SERVER_ENABLED)
    list(APPEND SBA_SRC_FILES ${SBA_DIR}/http2_server.cpp)
endif ()

## SBA used in NF_TARGET (main)
if (TARGET ${NF_TARGET})
    target_include_directories(${NF_TARGET} PUBLIC ${SBA_DIR})
    target_sources(${NF_TARGET} PRIVATE
            ${SBA_SRC_FILES}
    )
endif ()
