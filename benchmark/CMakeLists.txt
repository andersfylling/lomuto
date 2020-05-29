cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(LomutoBenchmarks
  LANGUAGES CXX
)

set("CMAKE_BUILD_TYPE" "Release")
set("BENCHMARK_ENABLE_LTO" "true")

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

if (TEST_INSTALLED_VERSION)
  find_package(Lomuto REQUIRED)
else()
  CPMAddPackage(
    NAME Lomuto
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
  )
endif()

CPMAddPackage(
  NAME Format.cmake
  GITHUB_REPOSITORY TheLartians/Format.cmake
  VERSION 1.3
)

CPMAddPackage(
        NAME benchmark
        GITHUB_REPOSITORY google/benchmark
        VERSION 1.5.0
        OPTIONS
        "BENCHMARK_ENABLE_TESTING Off"
)

if (benchmark_ADDED)
  set_target_properties(benchmark PROPERTIES CXX_STANDARD 20)
  target_compile_options(benchmark PUBLIC -Wall -pedantic -Wextra -Wno-error) # otherwise it will fail to build
endif()

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
add_executable(LomutoBenchmarks ${sources})
target_link_libraries(LomutoBenchmarks Lomuto benchmark)

set_target_properties(LomutoBenchmarks PROPERTIES CXX_STANDARD 20)

# enable compiler warnings
if (NOT TEST_INSTALLED_VERSION)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(Lomuto INTERFACE -Wall -pedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(Lomuto INTERFACE /W4 /WX)
    target_compile_definitions(LomutoBenchmarks PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# ---- Add GreeterTests ----

ENABLE_TESTING()

# ---- code coverage ----

if (ENABLE_TEST_COVERAGE)
  target_compile_options(Lomuto INTERFACE -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(Lomuto INTERFACE -fprofile-arcs -ftest-coverage)
endif()
