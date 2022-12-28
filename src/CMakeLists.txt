# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME "ndarray")

project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES C CXX)

# Generates compile_commands.json used by vscode-clangd.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 11)

if(NOT (ANDROID AND IOS))
  add_compile_definitions(DART_SHARED_LIB)
endif ()

add_library(${PROJECT_NAME} SHARED
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
  "wrap_index.c"
  "${DART_SDK}/include/dart_api_dl.c"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden
)


target_include_directories(${PROJECT_NAME} PRIVATE 
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${DART_SDK}/include"
  "${DART_SDK}/include/internal"
)