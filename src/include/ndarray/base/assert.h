/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_ASSERT_H
#define NDARRAY_BASE_ASSERT_H

#include <stdint.h>
#include "ndarray/casting_modes.h"
#include "ndarray/dtypes.h"

/*
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Determines if an array data type can be cast to another data type according
 * to a specified casting rule.
 */
int8_t ndarray_is_allowed_data_type_cast(
    const int8_t from, const int8_t to, const enum NDARRAY_CASTING_MODE casting
);

/**
 * Determines if a buffer length is compatible with provided ndarray meta data.
 */
int8_t ndarray_is_buffer_length_compatible(
    enum NDARRAY_DTYPE dtype, int64_t len, int64_t ndims, int64_t* shape,
    int64_t* strides, int64_t offset
);

/**
 * Determines if a buffer length is compatible with a provided shape array.
 */
int8_t ndarray_is_buffer_length_compatible_shape(
    int64_t len, int64_t ndims, int64_t* shape
);

/**
 * Determines if an array is column-major based on a provided stride array.
 */
int8_t ndarray_is_column_major(int64_t ndims, int64_t* strides);

/**
 * Determines if an array is column-major contiguous.
 */
int8_t ndarray_is_column_major_contiguous(
    enum NDARRAY_DTYPE dtype, int64_t ndims, int64_t* shape, int64_t* strides,
    int64_t offset
);

/**
 * Determines if an array is contiguous.
 */
int8_t ndarray_is_contiguous(
    enum NDARRAY_DTYPE dtype, int64_t ndims, int64_t* shape, int64_t* strides,
    int64_t offset
);

/**
 * Determines if an array is row-major based on a provided stride array.
 */
int8_t ndarray_is_row_major(int64_t ndims, int64_t* strides);

/**
 * Determines if an array is row-major contiguous.
 */
int8_t ndarray_is_row_major_contiguous(
    enum NDARRAY_DTYPE dtype, int64_t ndims, int64_t* shape, int64_t* strides,
    int64_t offset
);

/**
 * Determines if an array data type can be safely cast to another data type.
 */
int8_t ndarray_is_safe_data_type_cast(const int8_t from, const int8_t to);

/**
 * Determines if an array data type can be safely cast to, or is of the same
 * "kind" as, another data type.
 */
int8_t ndarray_is_same_kind_data_type_cast(const int8_t from, const int8_t to);

/**
 * Determines if an array is compatible with a single memory segment.
 */
int8_t ndarray_is_single_segment_compatible(
    enum NDARRAY_DTYPE dtype, int64_t ndims, int64_t* shape, int64_t* strides,
    int64_t offset
);

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_BASE_ASSERT_H