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
  "assert.c"
  "bind2vind.c"
  "broadcast_shapes.c"
  "bytes_per_element.c"
  "dtype_char.c"
  "function_object.c"
  "ind2sub.c"
  "iteration_order.c"
  "max_view_buffer_index.c"
  "min_view_buffer_index.c"
  "minmax_view_buffer_index.c"
  "ndarray.c"
  "nonsingleton_dimensions.c"
  "numel.c"
  "shape2strides.c"
  "singleton_dimensions.c"
  "strides2offset.c"
  "strides2order.c"
  "sub2ind.c"
  "vind2bind.c"
)

target_include_directories(ndarray PRIVATE "${INCLUDES_DIR}")

set_target_properties(ndarray PROPERTIES
  PUBLIC_HEADER ndarray.h
  OUTPUT_NAME "ndarray"
)

target_compile_definitions(ndarray PUBLIC DART_SHARED_LIB)
