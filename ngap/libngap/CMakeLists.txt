################################################################################
# SPDX-License-Identifier: LicenseRef-CSSL-1.0
################################################################################
set(NGAP_DIR "${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap")
file(GLOB NGAP_lib_c_src_files
  *.c
)
add_library (NGAP_LIB_C STATIC
  ${NGAP_lib_c_src_files}
)


target_include_directories(NGAP_LIB_C PUBLIC ${NGAP_DIR}/libngap)

# ---------- Unity build: libngap (opt-in) ----------
option(NGAP_UNITY_BUILD "Enable unity builds for libngap (batch size 20)" OFF)
if(NGAP_UNITY_BUILD)
  set_target_properties(NGAP_LIB_C PROPERTIES
    UNITY_BUILD ON
    UNITY_BUILD_BATCH_SIZE 20
  )
endif()
# --------------------------------------------------
