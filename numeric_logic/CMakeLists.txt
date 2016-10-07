# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


cmake_minimum_required(VERSION 3.6)
project(numeric_logic)

if (APPLE)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif (APPLE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror")

find_package(PythonLibs 3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

set(SOURCE_FILES extend_py/numericmodule.cpp)

add_library(numeric SHARED ${SOURCE_FILES})

set_target_properties(numeric PROPERTIES PREFIX "")
target_link_libraries(numeric ${PYTHON_LIBRARIES})
