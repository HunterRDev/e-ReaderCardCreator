cmake_minimum_required(VERSION 3.12)
project(nedclib VERSION 1.4.0 LANGUAGES C CXX)

option(BUILD_TOOLS "Build standalone tools" OFF)

set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(NEDCLIB_SRCS
    nedclib.cpp
    nes.cpp
    rawbin/binraw.cpp
    rawbin/rs.cpp
    rawbmp/address.cpp
    rawbmp/dcs_decode.cpp
    rawbmp/dcs_encode.cpp
    rawbmp/rawbmp.cpp
    stdafx.cpp
    vpk/vpk.cpp
    nedclib.h
    rawbin/rs.h
    rawbmp/dcs.h
    stdafx.h
)
add_library(nedclib SHARED "${NEDCLIB_SRCS}")
target_include_directories(nedclib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/rawbmp")
target_compile_definitions(nedclib PRIVATE "NEDCLIB_EXPORTS=1")

if(BUILD_TOOLS)
    add_executable(raw2bmp "raw2bmp.cpp")
    target_link_libraries(raw2bmp PRIVATE nedclib)
    add_executable(nedcenc "nedcenc.cpp")
    target_link_libraries(nedcenc PRIVATE nedclib)
    add_executable(nevpk "nevpk.cpp")
    target_link_libraries(nevpk PRIVATE nedclib)
    add_executable(nedcmaker "nedcmake/nedcmaker.cpp")
    if(UNIX)
        target_compile_definitions(nedcmaker PRIVATE "stricmp=strcmp")
    endif()
    # Special tool: headerfix, which is outside this project
    set(HEADER_FIX_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/../Source Code/headerfix.c")
    if(EXISTS "${HEADER_FIX_SOURCE}")
        add_executable(headerfix "${HEADER_FIX_SOURCE}")
    endif()
endif()
