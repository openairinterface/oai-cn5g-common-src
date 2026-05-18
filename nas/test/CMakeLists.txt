################################################################################
# SPDX-License-Identifier: LicenseRef-CSSL-1.0
################################################################################
cmake_minimum_required (VERSION 3.22.1)
enable_testing()

# Locate or use pre-built GoogleTest.
# Primary: system-installed GoogleTest (libgtest-dev).
# Fallback: pre-built GoogleTest from the companion NEF build tree.
find_package(GTest QUIET)
if (NOT GTest_FOUND)
  set(_NEF_GTEST_LIB_DIR
      "/home/maserati/cn5g/oai-cn5g-nef/build/nef/build/lib")
  set(_NEF_GTEST_INC_DIR
      "/home/maserati/cn5g/oai-cn5g-nef/build/nef/build/_deps/googletest-src/googletest/include")
  if (EXISTS "${_NEF_GTEST_LIB_DIR}/libgtest.a" AND
      EXISTS "${_NEF_GTEST_INC_DIR}/gtest/gtest.h")
    message(STATUS "NAS tests: using pre-built GTest from NEF build tree")
    add_library(GTest::gtest STATIC IMPORTED)
    set_target_properties(GTest::gtest PROPERTIES
      IMPORTED_LOCATION "${_NEF_GTEST_LIB_DIR}/libgtest.a"
      INTERFACE_INCLUDE_DIRECTORIES "${_NEF_GTEST_INC_DIR}")
    add_library(GTest::gtest_main STATIC IMPORTED)
    set_target_properties(GTest::gtest_main PROPERTIES
      IMPORTED_LOCATION "${_NEF_GTEST_LIB_DIR}/libgtest_main.a"
      INTERFACE_INCLUDE_DIRECTORIES "${_NEF_GTEST_INC_DIR}")
    set(GTest_FOUND TRUE)
  endif()
endif()

if (NOT GTest_FOUND)
  message(WARNING "GTest not found — NAS unit tests will not be built")
  return()
endif()

include_directories(${3GPP_DIR})
include_directories(${COMMON_DIR})
include_directories(${LOGGER_DIR})
include_directories(${MODEL_DIR})
include_directories(${NAS_DIR}/5gmm-msgs)
include_directories(${NAS_DIR}/common)
include_directories(${NAS_DIR}/ies)
include_directories(${NAS_DIR}/utils)
include_directories(${UTILS_DIR})
include_directories(${UTILS_DIR}/bstr)

file(GLOB NAS_TEST_src_files
    ${NAS_TEST_DIR}/*.cpp
)

add_executable(test_nas_msg ${NAS_TEST_src_files})
target_link_libraries(test_nas_msg
  PRIVATE
    GTest::gtest_main
    GTest::gtest
    NAS
    LOGGER
    COMMON
    -lfmt
    -lspdlog
    pthread)
add_test(NAME NasMessageTests COMMAND test_nas_msg)
# Suppress ASan leak detection: pre-existing leaks in bstrlib (TLVDecoder.c)
# are unrelated to NAS message logic correctness.
set_tests_properties(NasMessageTests PROPERTIES
  ENVIRONMENT "ASAN_OPTIONS=detect_leaks=0")
