# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.19)

# Generates compile_commands.json used by vscode-clangd.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ndarray_library VERSION 0.0.1 LANGUAGES C)

get_filename_component(PROJECT_DIR ../ ABSOLUTE)

set(CMAKE_BINARY_DIR "${PROJECT_DIR}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

variable_watch(INCLUDES_DIR)

set(INCLUDES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/includes")

add_library(ndarray SHARED
  "ndarray.c"
)

target_include_directories(ndarray PRIVATE "${INCLUDES_DIR}")

set_target_properties(ndarray PROPERTIES
  PUBLIC_HEADER ndarray.h
  OUTPUT_NAME "ndarray"
)

target_compile_definitions(ndarray PUBLIC DART_SHARED_LIB)