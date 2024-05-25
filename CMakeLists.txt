# Set project-name
set(ProjectName "Template")

cmake_minimum_required(VERSION 3.19)

set(CMAKE_GENERATOR_PLATFORM win32)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "You are configuring a 64bit build, this is not supported. Run cmake with `-A Win32`")
endif()

project(${ProjectName})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_compile_definitions(
    "NOMINMAX"
    "_WIN32_WINNT=_WIN32_WINNT_WIN7"
    "WIN32_LEAN_AND_MEAN")

add_compile_options(/MP /permissive-)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/")

add_library(${ProjectName} SHARED)

file(GLOB_RECURSE SOURCES
    "src/*.h"
    "src/*.cpp")

target_sources(${ProjectName} PUBLIC ${SOURCES})
target_precompile_headers(${ProjectName} PRIVATE "src/pch.h")

target_compile_options(${ProjectName} PRIVATE /W4 /WX /ZI)

target_include_directories(${ProjectName} PUBLIC "src/")

include(gwca)
include(imgui)

target_link_libraries(${ProjectName} PRIVATE
    imgui
    gwca)

# cmake -B build -A Win32
# cmake --open build