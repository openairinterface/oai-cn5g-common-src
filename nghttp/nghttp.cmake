# SPDX-License-Identifier: LicenseRef-CSSL-1.0

SET(NGHTTP_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nghttp)

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger/logger.cmake)
include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/common/common.cmake)

include_directories(${NGHTTP_DIR})

SET(NGHTTP_SRC_FILES
        ${NGHTTP_DIR}/http_client.cpp
)

# The HTTP/2 server (http2_server.cpp) pulls in libevent on top of nghttp2, so
# it is opt-in: an NF that serves SBI over it sets HTTP2_SERVER_ENABLED before
# including this file and adds `event event_pthreads` to its link line. NFs
# that only act as HTTP clients are unaffected.
if (HTTP2_SERVER_ENABLED)
    list(APPEND NGHTTP_SRC_FILES ${NGHTTP_DIR}/http2_server.cpp)
endif ()

## NGHTTP used in NF_TARGET (main)
if (TARGET ${NF_TARGET})
    target_include_directories(${NF_TARGET} PUBLIC ${NGHTTP_DIR})
    target_sources(${NF_TARGET} PRIVATE
            ${NGHTTP_SRC_FILES}
    )
endif ()
