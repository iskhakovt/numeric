# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


cmake_minimum_required(VERSION 3.0)


find_package(PythonLibs 3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ..)

set(SOURCE_FILES numericmodule.cpp)

add_library(numeric SHARED ${SOURCE_FILES})

set_target_properties(numeric PROPERTIES PREFIX "")
target_link_libraries(numeric numeric-core ${PYTHON_LIBRARIES})
