# CMakeLists.txt                                                    -*-CMake-*-
#
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 4.0)

project(optional_slides VERSION 0.0.0 LANGUAGES CXX)

# Includes
include(CTest)
include(FetchContent)

set(TARGETS_EXPORT_NAME ${CMAKE_PROJECT_NAME}Targets)

option(
  SLIDES_ENABLE_TESTING
  "Enable building tests and test infrastructure"
  ${PROJECT_IS_TOP_LEVEL}
)

set(CMAKE_VERIFY_INTERFACE_HEADER_SETS ON)


if(SLIDES_ENABLE_TESTING)
  find_package(GTest QUIET)
  if(GTest_FOUND)
    # Create the library target and named header set for testing beman_optional
    # and mark the set private
    add_executable(slide_test)
    # Tests
    add_subdirectory(tests)
  else()
    message(
      WARNING
      "
No provider for GTest. Unable to build tests.
Consider using CMAKE_PROJECT_TOP_LEVEL_INCLUDES=./cmake/use-fetch-content.cmake as documented in the README.md
"
    )
  endif()
endif()


add_subdirectory(bemanoptional EXCLUDE_FROM_ALL)
add_subdirectory(examples)
