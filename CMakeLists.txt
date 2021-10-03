cmake_minimum_required(VERSION 3.20)
project(net)

set(CMAKE_CXX_STANDARD 17)

set(LIB_NAME net)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(header_files
        ${CMAKE_CURRENT_SOURCE_DIR}/include/net/socket.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/net/address_resolver.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/net/tcp_client.hpp)

add_library(${LIB_NAME} INTERFACE)
target_sources(${LIB_NAME} INTERFACE "$<BUILD_INTERFACE:${header_files}>")
target_include_directories(${LIB_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_include_directories(${LIB_NAME} SYSTEM INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)

option(NET_BUILD_TEST "build test" ON)
if(${NET_BUILD_TEST})
    enable_testing()
    add_subdirectory(test)
endif()

option(NET_BUILD_EXAMPLE "build example" ON)
if(${NET_BUILD_EXAMPLE})
    add_subdirectory(example)
endif()