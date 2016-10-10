# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


cmake_minimum_required(VERSION 3.0)
project(numeric-core)


if (APPLE)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif (APPLE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror")
find_package(PythonLibs 3 REQUIRED)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY .)


set(SOURCE_FILES tabulate.cpp tabulate_integral.cpp)

add_library(numeric-core SHARED ${SOURCE_FILES})

set_target_properties(numeric-core PROPERTIES PREFIX "")
target_link_libraries(numeric-core)


add_subdirectory(extend_py)
