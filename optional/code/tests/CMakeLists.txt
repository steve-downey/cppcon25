# cmake-format: off
# src/beman/optional/tests/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

target_sources(
    slide_test
    PRIVATE
        breathing.t.cpp
        slides.t.cpp
)


target_link_libraries(
    slide_test
    PRIVATE beman_optional GTest::gtest GTest::gtest_main
)
include(GoogleTest)
gtest_add_tests(TARGET slide_test "" AUTO)
