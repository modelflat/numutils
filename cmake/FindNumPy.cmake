cmake_minimum_required(VERSION 3.10)

if(NOT PYTHON_EXECUTABLE)
    find_package(PythonInterp REQUIRED)
endif()

if (PYTHON_EXECUTABLE)
    execute_process(COMMAND "${PYTHON_EXECUTABLE}" -c
        "try: import numpy; print(numpy.get_include(), end='');\nexcept: pass"
        OUTPUT_VARIABLE NUMPY_PATH)
else()
    message(STATUS "Python executable not found.")
endif()

find_path(NUMPY_INCLUDE_DIR numpy
          HINTS "${NUMPY_PATH}" "${PYTHON_INCLUDE_PATH}" NO_DEFAULT_PATH)

if(NUMPY_INCLUDE_DIR)
    set(NUMPY_FOUND 1 CACHE INTERNAL "Python numpy found")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NumPy REQUIRED_VARS NUMPY_INCLUDE_DIR)