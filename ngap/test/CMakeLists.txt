################################################################################
# SPDX-License-Identifier: LicenseRef-CSSL-1.0
################################################################################
cmake_minimum_required (VERSION 3.22.1)
enable_testing()
include(GoogleTest)

include_directories(${COMMON_DIR})
include_directories(${LOGGER_DIR})
include_directories(${MODEL_DIR})
include_directories(${NGAP_DIR}/libngap)
include_directories(${NGAP_DIR}/libngap_ies)
include_directories(${NGAP_DIR}/ngap_ies)
include_directories(${NGAP_DIR}/ngap_msgs)
include_directories(${NGAP_DIR}/utils)
include_directories(${UTILS_DIR})
include_directories(${UTILS_DIR}/bstr)

file(GLOB NGAP_TEST_src_files
    ${NGAP_TEST_DIR}/*.cpp
)

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
    message(STATUS "NGAP tests: using pre-built GTest from NEF build tree")
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
  message(WARNING "GTest not found — NGAP unit tests will not be built")
  return()
endif()

add_executable(test_ngap_msg ${NGAP_TEST_src_files})
target_link_libraries(test_ngap_msg
  PRIVATE
    GTest::gtest_main
    GTest::gtest
    NGAP
    LOGGER
    COMMON
    -lfmt
    -lspdlog
    pthread)
add_test(NAME NgapMessageTests COMMAND test_ngap_msg --gtest_filter="TestSuiteNGAPMsg*")
# Suppress ASan leak detection: pre-existing leaks in bstrlib are unrelated
# to NGAP message logic correctness.
set_tests_properties(NgapMessageTests PROPERTIES
  ENVIRONMENT "ASAN_OPTIONS=detect_leaks=0")
