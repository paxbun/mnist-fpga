# Copyright (c) 2021 Chanjung Kim. All rights reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.13)
project(mnist-fpga)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Vitis REQUIRED)

add_executable(mnist-fpga
    ${PROJECT_SOURCE_DIR}/Source/ClFactory.cc
    ${PROJECT_SOURCE_DIR}/Source/Config.cc
    ${PROJECT_SOURCE_DIR}/Source/Main.cc
)
target_include_directories(mnist-fpga
    PUBLIC ${PROJECT_SOURCE_DIR}/Public
    PRIVATE ${Vitis_INCLUDE_DIRS}
)
target_link_libraries(mnist-fpga
    ${Vitis_LIBRARIES}
)