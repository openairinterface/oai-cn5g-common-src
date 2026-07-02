# SPDX-License-Identifier: LicenseRef-CSSL-1.0

SET(LMF_MODEL_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model)
set(COMMON_MODEL_SRC_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model)

include(${COMMON_MODEL_SRC_DIR}/common_model.cmake)


include_directories(${LMF_MODEL_DIR})

file(GLOB LMF_MODEL_SRC_FILES
    ${LMF_MODEL_DIR}/*.cpp
)

# This generated PCF-only variant duplicates the TransportProtocol_anyOf
# symbols used by LMF and breaks linking when the flattened model directory is
# globbed into the LMF target.
list(REMOVE_ITEM LMF_MODEL_SRC_FILES
    ${LMF_MODEL_DIR}/TransportProtocol_1_anyOf.cpp
)

## CONFIG used in NF_TARGET (main)
if (TARGET ${NF_TARGET})
    target_include_directories(${NF_TARGET} PUBLIC ${LMF_MODEL_DIR})
    target_sources(${NF_TARGET} PRIVATE
            ${LMF_MODEL_SRC_FILES}
            )
endif()
