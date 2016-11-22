# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


cmake_minimum_required(VERSION 3.0)
project(numeric-core)


if (APPLE)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif (APPLE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3 -lm -Wall -Werror -fPIC -fstack-protector -fstack-protector-all -Wl,--export-dynamic -Wl,-E -static-libstdc++")


set(
    SOURCE_FILES
    cubic_spline.cpp
    differential_equation.cpp
    tabulate.cpp
    tabulated.cpp
    integral.cpp
    linear_system.cpp
    model.cpp
    polynomial.cpp
    real.cpp
)

add_library(numeric-core STATIC ${SOURCE_FILES})

set_target_properties(numeric-core PROPERTIES PREFIX "")
target_link_libraries(numeric-core)


add_subdirectory(extend_py)
