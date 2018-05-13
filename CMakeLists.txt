cmake_minimum_required(VERSION 2.8)

PROJECT(pulse_simulate)
MESSAGE(STATUS "## Making project " ${CMAKE_PROJECT_NAME})

SET(ROOTPATH ${CMAKE_SOURCE_DIR})

#指定编译器路径
set(CROSS_TOOLCHAIN_PERFIX "/usr/local/arm/4.4.3/bin")

#指定编译器
set(CMAKE_C_COMPILER "${CROSS_TOOLCHAIN_PERFIX}/arm-linux-gcc")
set(CMAKE_CXX_COMPILER "${CROSS_TOOLCHAIN_PERFIX}/arm-linux-g++")
#set(CMAKE_C_COMPILER "/usr/local/arm-2011.03/bin/arm-none-linux-gnueabi-gcc")
#set(CMAKE_CXX_COMPILER"/usr/local/arm-2011.03/bin/arm-none-linux-gnueabi-g++")

#指定编译目录
set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)

#添加子目录
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

