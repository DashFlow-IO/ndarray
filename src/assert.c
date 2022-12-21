/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include "ndarray/base/assert.h"
#include <stdint.h>
#include <stdlib.h>
#include "ndarray/base/iteration_order.h"
#include "ndarray/base/minmax_view_buffer_index.h"
#include "ndarray/base/numel.h"
#include "ndarray/bytes_per_element.h"
#include "ndarray/casting_modes.h"
#include "ndarray/safe_casts.h"
#include "ndarray/same_kind_casts.h"

/**
 * Determines if an array data type can be cast to another array data type
 * according to a specified casting rule.
 *
 * ## Notes
 *
 * -   The function returns `1` if a data type can be cast and `0` otherwise.
 *
 * @param from     array data type
 * @param to       array data type
 * @param casting  casting mode
 * @return         value indicating if a data type can be cast
 *
 * @example
 * #include "ndarray/casting_modes.h"
 * #include "ndarray/base/assert.h"
 *
 * int8_t b = ndarray_is_allowed_data_type_cast(2, 2,
 * NDARRAY_SAFE_CASTING);
 * // returns 1
 */
int8_t ndarray_is_allowed_data_type_cast(
    const int8_t from, const int8_t to, const enum NDARRAY_CASTING_MODE casting
) {
  // Anything goes for "unsafe" casting...
  if (casting == NDARRAY_UNSAFE_CASTING) {
    return 1;
  }
  // "Casting" to the same data type is always allowed, regardless of the
  // casting mode...
  if (from == to) {
    return 1;
  }
  // No casts between different data types are allowed in "no" or "equiv"
  // casting modes...
  if (casting == NDARRAY_NO_CASTING || casting == NDARRAY_EQUIV_CASTING) {
    return 0;
  }
  // In "safe" casting mode, only casts which preserve values are allowed...
  if (casting == NDARRAY_SAFE_CASTING) {
    return ndarray_is_safe_data_type_cast(from, to);
  }
  // In "same-kind" casting mode, in addition to "safe" casts, casts within a
  // "kind" (e.g., between signed integers or between floating-point numbers)
  // are allowed...
  return ndarray_is_same_kind_data_type_cast(from, to);
}

/**
 * Determines if a buffer length is compatible with provided ndarray meta data.
 *
 * ## Notes
 *
 * -   The function returns `1` if the buffer length is compatible and `0`
 * otherwise.
 *
 * @param dtype    buffer data type
 * @param len      number of elements in data buffer
 * @param ndims    number of dimensions
 * @param shape    array shape (dimensions)
 * @param strides  array strides
 * @param offset   index offset
 * @return         value indicating if a buffer length is compatible
 *
 * @example
 * #include "ndarray/base/assert.h"
 * #include "ndarray/dtypes.h"
 *
 * int64_t ndims = 2;
 * int64_t shape[] = {10, 10};
 * int64_t strides[] = {10, 1};
 * int64_t offset = 0;
 *
 * int8_t b = ndarray_is_buffer_length_compatible(NDARRAY_UINT8,
 * 1000, ndims, shape, strides, offset);
 * // returns 1
 *
 * int8_t b = ndarray_is_buffer_length_compatible(NDARRAY_UINT8,
 * 10, ndims, shape, strides, offset);
 * // returns 0
 */
int8_t ndarray_is_buffer_length_compatible(
    enum NDARRAY_DTYPE dtype, int64_t len, int64_t ndims, int64_t* shape,
    int64_t* strides, int64_t offset
) {
  int64_t tmp[2];
  int64_t nbytes;

  // Determine the number of bytes per element:
  nbytes = ndarray_bytes_per_element(dtype);

  // Determine the minimum and maximum linear indices which are accessible by
  // the array view:
  ndarray_minmax_view_buffer_index(ndims, shape, strides, offset, tmp);

  // If the indices are "inbounds", then the buffer length is compatible:
  if ((tmp[0] / nbytes) >= 0 && (tmp[1] / nbytes) < len) {
    return 1;
  }
  return 0;
}

/**
 * Determines if a buffer length is compatible with a provided shape array.
 *
 * ## Notes
 *
 * -   The function returns `1` if the buffer length is compatible and `0`
 * otherwise.
 *
 * @param len      number of elements in data buffer
 * @param ndims    number of dimensions
 * @param shape    array shape (dimensions)
 * @return         value indicating if a buffer length is compatible
 *
 * @example
 * #include "ndarray/base/assert.h"
 *
 * int64_t ndims = 2;
 * int64_t shape[] = {10, 10};
 *
 * int8_t b = ndarray_is_buffer_length_compatible_shape(1000, ndims, shape);
 * // returns 1
 *
 * int8_t b = ndarray_is_buffer_length_compatible_shape(10, ndims, shape);
 * // returns 0
 */
int8_t ndarray_is_buffer_length_compatible_shape(
    int64_t len, int64_t ndims, int64_t* shape
) {
  if (len > ndarray_numel(ndims, shape)) {
    return 1;
  }
  return 0;
}

/**
 * Determines if an array is column-major based on a provided stride array.
 *
 * ## Notes
 *
 * -   The function returns `1` if column-major and `0` otherwise.
 *
 * @param ndims    number of dimensions
 * @param strides  array strides
 * @return         value indicating if column-major
 *
 * @example
 * #include "ndarray/base/assert.h"
 *
 * int64_t ndims = 2;
 * int64_t strides[] = {1, 10};
 *
 * int8_t b = ndarray_is_column_major(ndims, strides);
 * // returns 1
 */
int8_t ndarray_is_column_major(int64_t ndims, int64_t* strides) {
  int64_t s1;
  int64_t s2;
  int64_t i;

  if (ndims == 0) {
    return 0;
  }
  s1 = llabs(strides[0]);
  for (i = 1; i < ndims; i++) {
    s2 = llabs(strides[i]);
    if (s2 < s1) {
      return 0;
    }
    s1 = s2;
  }
  return 1;
}

/**
 * Determines if an array is column-major contiguous.
 *
 * ## Notes
 *
 * -   The function returns `1` if column-major contiguous and `0` otherwise.
 *
 * @param dtype    data type
 * @param ndims    number of dimensions
 * @param shape    array shape (dimensions)
 * @param strides  array strides (in bytes)
 * @param offset   index offset
 * @return         value indicating if column-major contiguous
 *
 * @example
 * #include "ndarray/base/assert.h"
 * #include "ndarray/dtypes.h"
 *
 * int64_t ndims = 2;
 * int64_t shape[] = {10, 10};
 * int64_t strides[] = {1, 10};
 * int64_t offset = 0;
 *
 * int8_t b = ndarray_is_column_major_contiguous(NDARRAY_UINT8, ndims, shape,
 * strides, offset);
 * // returns 1
 */
int8_t ndarray_is_column_major_contiguous(
    enum NDARRAY_DTYPE dtype, int64_t ndims, int64_t* shape, int64_t* strides,
    int64_t offset
) {
  if (ndarray_iteration_order(ndims, strides) != 0 &&
      ndarray_is_column_major(ndims, strides) != 0 &&
      ndarray_is_single_segment_compatible(
          dtype, ndims, shape, strides, offset
      ) != 0) {
    return 1;
  }
  return 0;
}

/**
 * Determines if an array is contiguous.
 *
 * ## Notes
 *
 * -   The function returns `1` if contiguous and `0` otherwise.
 *
 * @param dtype    data type
 * @param ndims    number of dimensions
 * @param shape    array shape (dimensions)
 * @param strides  array strides (in bytes)
 * @param offset   index offset
 * @return         value indicating if contiguous
 *
 * @example
 * #include "ndarray/base/assert.h"
 * #include "ndarray/dtypes.h"
 *
 * int64_t ndims = 2;
 * int64_t shape[] = {10, 10};
 * int64_t strides[] = {10, 1};
 * int64_t offset = 0;
 *
 * int8_t b = ndarray_is_contiguous(NDARRAY_UINT8, ndims, shape, strides,
 * offset);
 * // returns 1
 */
int8_t ndarray_is_contiguous(
    enum NDARRAY_DTYPE dtype, int64_t ndims, int64_t* shape, int64_t* strides,
    int64_t offset
) {
  if (ndarray_iteration_order(ndims, strides) != 0 &&
      ndarray_is_single_segment_compatible(
          dtype, ndims, shape, strides, offset
      ) != 0) {
    return 1;
  }
  return 0;
}

/**
 * Determines if an array is row-major based on a provided stride array.
 *
 * ## Notes
 *
 * -   The function returns `1` if row-major and `0` otherwise.
 *
 * @param ndims    number of dimensions
 * @param strides  array strides
 * @return         value indicating if row-major
 *
 * @example
 * #include "ndarray/base/assert.h"
 *
 * int64_t ndims = 2;
 * int64_t strides[] = {10, 1};
 *
 * int8_t b = ndarray_is_row_major(ndims, strides);
 * // returns 1
 */
int8_t ndarray_is_row_major(int64_t ndims, int64_t* strides) {
  int64_t s1;
  int64_t s2;
  int64_t i;

  if (ndims == 0) {
    return 0;
  }
  s1 = llabs(strides[0]);
  for (i = 1; i < ndims; i++) {
    s2 = llabs(strides[i]);
    if (s2 > s1) {
      return 0;
    }
    s1 = s2;
  }
  return 1;
}

/**
 * Determines if an array is row-major contiguous.
 *
 * ## Notes
 *
 * -   The function returns `1` if row-major contiguous and `0` otherwise.
 *
 * @param dtype    data type
 * @param ndims    number of dimensions
 * @param shape    array shape (dimensions)
 * @param strides  array strides (in bytes)
 * @param offset   index offset
 * @return         value indicating if row-major contiguous
 *
 * @example
 * #include "ndarray/base/assert.h"
 * #include "ndarray/dtypes.h"
 *
 * int64_t ndims = 2;
 * int64_t shape[] = {10, 10};
 * int64_t strides[] = {10, 1};
 * int64_t offset = 0;
 *
 * int8_t b = ndarray_is_row_major_contiguous(NDARRAY_UINT8, ndims, shape,
 * strides, offset);
 * // returns 1
 */
int8_t ndarray_is_row_major_contiguous(
    enum NDARRAY_DTYPE dtype, int64_t ndims, int64_t* shape, int64_t* strides,
    int64_t offset
) {
  if (ndarray_iteration_order(ndims, strides) != 0 &&
      ndarray_is_row_major(ndims, strides) != 0 &&
      ndarray_is_single_segment_compatible(
          dtype, ndims, shape, strides, offset
      ) != 0) {
    return 1;
  }
  return 0;
}

/**
 * Determines if an array data type can be safely cast to another array data
 * type.
 *
 * ## Notes
 *
 * -   The function returns `1` if a data type can be safely cast and `0`
 * otherwise.
 *
 * @param from     array data type
 * @param to       array data type
 * @return         value indicating if a data type can be safely cast
 *
 * @example
 * #include "ndarray/base/assert.h"
 *
 * int8_t b = ndarray_is_safe_data_type_cast(2, 2);
 * // returns 1
 */
int8_t ndarray_is_safe_data_type_cast(const int8_t from, const int8_t to) {
  if (from == to) {
    return 1;
  }
  if (from < NDARRAY_NDTYPES && to < NDARRAY_NDTYPES) {
    return NDARRAY_SAFE_CASTS[from][to];
  }
  return 0;
}

/**
 * Determines if an array data type can be safely cast to, or is of the same
 * "kind" as, another array data type.
 *
 * ## Notes
 *
 * -   The function returns `1` if a data type can be cast and `0` otherwise.
 *
 * @param from     array data type
 * @param to       array data type
 * @return         value indicating if a data type can be cast
 *
 * @example
 * #include "ndarray/base/assert.h"
 *
 * int8_t b = ndarray_is_same_kind_data_type_cast(2, 2);
 * // returns 1
 */
int8_t ndarray_is_same_kind_data_type_cast(const int8_t from, const int8_t to) {
  if (from == to) {
    return 1;
  }
  if (from < NDARRAY_NDTYPES && to < NDARRAY_NDTYPES) {
    return NDARRAY_SAME_KIND_CASTS[from][to];
  }
  return 0;
}

/**
 * Determines if an array is compatible with a single memory segment.
 *
 * ## Notes
 *
 * -   The function returns `1` if compatible with a single memory segment and
 * `0` otherwise.
 *
 * @param dtype    data type
 * @param ndims    number of dimensions
 * @param shape    array shape (dimensions)
 * @param strides  array strides (in bytes)
 * @param offset   index offset
 * @return         value indicating if compatible with a single memory segment
 *
 * @example
 * #include "ndarray/base/assert.h"
 * #include "ndarray/dtypes.h"
 *
 * int64_t ndims = 2;
 * int64_t shape[] = {10, 10};
 * int64_t strides[] = {10, 1};
 * int64_t offset = 0;
 *
 * int8_t b = ndarray_is_single_segment_compatible(NDARRAY_UINT8, ndims, shape,
 * strides, offset);
 * // returns 1
 */
int8_t ndarray_is_single_segment_compatible(
    enum NDARRAY_DTYPE dtype, int64_t ndims, int64_t* shape, int64_t* strides,
    int64_t offset
) {
  int64_t tmp[2];
  int64_t nbytes;
  int64_t len;

  // Compute the total number of elements:
  len = ndarray_numel(ndims, shape);
  if (len == 0) {
    return 0;
  }
  // Determine the minimum and maximum linear indices which are accessible by
  // the array view:
  ndarray_minmax_view_buffer_index(ndims, shape, strides, offset, tmp);

  // Determine the number of bytes per element:
  nbytes = ndarray_bytes_per_element(dtype);

  // If the number of elements matches the view size, then the array is
  // single-segment compatible:
  if ((len * nbytes) == ((tmp[1] - tmp[0]) + nbytes)) {
    return 1;
  }
  return 0;
}
