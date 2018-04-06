cmake_minimum_required(VERSION 3.10)
project(NumUtils)

option(BUILD_DEBUG OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (BUILD_DEBUG)
    set(OPTIMIZATIONS "-O0")
else()
    set(OPTIMIZATIONS "-O3")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPTIMIZATIONS} -Wall -Wextra -Werror" )

if (BUILD_DEBUG)
    add_definitions(-DNUMERICALUTILS_DEBUG_OUTPUT)
endif()

include_directories(include)
include_directories(SYSTEM matplotlib-cpp)

find_package(PythonLibs REQUIRED 3)
find_package(NumPy REQUIRED)

include_directories(SYSTEM ${PYTHON_INCLUDE_DIRS} ${NUMPY_INCLUDE_DIR})

find_package(GTest REQUIRED)
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})

set(TEST_SOURCES
    test/TestUtils.hpp
    test/RangeTest.cpp
    )

add_executable       (NumUtilsTest ${TEST_SOURCES})
target_link_libraries(NumUtilsTest ${PYTHON_LIBRARIES} gtest ${GTEST_BOTH_LIBRARIES})

add_executable       (Example_Minimal examples/Minimal.cpp)
target_link_libraries(Example_Minimal ${PYTHON_LIBRARIES})

add_executable       (Example_Galerkin examples/Galerkin.cpp)
target_link_libraries(Example_Galerkin ${PYTHON_LIBRARIES})