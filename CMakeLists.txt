cmake_minimum_required(VERSION 3.9)
project(NumUtils)

option(BUILD_EXAMPLES ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra" )
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

if (BUILD_DEBUG)
    add_definitions(-DNUMERICALUTILS_DEBUG_OUTPUT)
endif()

include_directories(include)
include_directories(SYSTEM matplotlib-cpp)

if (BUILD_EXAMPLES OR CLION_IDE_FIX_CONDITIONAL_TARGETS)
    # conditional targets are not visible in CLion for some reason, even if BUILD_EXAMPLES == ON
    find_package(PythonLibs REQUIRED 3)
    find_package(NumPy REQUIRED)
    include_directories(SYSTEM ${PYTHON_INCLUDE_DIRS} ${NUMPY_INCLUDE_DIR})

    add_executable       (Example_Minimal examples/Minimal.cpp)
    target_link_libraries(Example_Minimal ${PYTHON_LIBRARIES})

    add_executable       (Example_Galerkin examples/Galerkin.cpp)
    target_link_libraries(Example_Galerkin ${PYTHON_LIBRARIES})
endif ()

enable_testing()

find_package(GTest REQUIRED)
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})

set(TEST_SOURCES
    test/TestUtils.hpp
    test/RangeTest.cpp
    test/NumUtilsTest.cpp)

add_executable       (NumUtilsTest ${TEST_SOURCES})
if (${CMAKE_BUILD_TYPE} MATCHES Coverage)
    include(CodeCoverage)
    target_compile_options(NumUtilsTest PRIVATE
                           -g -O0 --coverage -fprofile-arcs -ftest-coverage
                           -fno-inline -fno-inline-small-functions -fno-default-inline)
    SETUP_TARGET_FOR_COVERAGE(coverage NumUtilsTest coverage_out)
endif()
target_link_libraries(NumUtilsTest ${PYTHON_LIBRARIES} gtest ${GTEST_BOTH_LIBRARIES})

add_test ( NUTests NumUtilsTest )

