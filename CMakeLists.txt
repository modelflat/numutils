cmake_minimum_required(VERSION 3.10)
project(Numerical4thCourse)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

include_directories(include)
include_directories(matplotlib-cpp)

find_package(PythonLibs REQUIRED 3)
set(NUMPY_INCLUDE_DIRS "${PYTHON_INCLUDE_DIRS}/../Lib/site-packages/numpy/core/include") # todo make system-independent
message(${NUMPY_INCLUDE_DIRS})

set(SOURCES
    src/main.cpp)

add_executable(Numerical4thCourse ${SOURCES})
target_include_directories(Numerical4thCourse PRIVATE
                           ${PYTHON_INCLUDE_DIRS}
                           ${NUMPY_INCLUDE_DIRS}
                           )
target_link_libraries(Numerical4thCourse ${PYTHON_LIBRARIES})