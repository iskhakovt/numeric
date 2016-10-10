# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


cmake_minimum_required(VERSION 3.0)
project(numeric-core)


if (APPLE)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif (APPLE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror")


set(
	SOURCE_FILES
	tabulate.cpp
	interpolate.cpp
	tabulate_integral.cpp
	linear_system.cpp
	differential_equation.cpp
	model.cpp
)

add_library(numeric-core SHARED ${SOURCE_FILES})

set_target_properties(numeric-core PROPERTIES PREFIX "")
target_link_libraries(numeric-core)


add_subdirectory(extend_py)
