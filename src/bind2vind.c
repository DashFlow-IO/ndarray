/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include "ndarray/base/bind2vind.h"
#include <stdint.h>
#include <stdlib.h>
#include "ndarray/index_modes.h"
#include "ndarray/orders.h"

/**
 * Converts a linear index in an underlying data buffer to a linear index in an
 * array view.
 *
 * ## Notes
 *
 * -   In "error" mode, the function returns `-1` if an index is out-of-bounds.
 *
 * @param ndims    number of dimensions
 * @param shape    array shape (dimensions)
 * @param strides  array strides
 * @param offset   location of the first indexed value **based** on the stride
 *                 array
 * @param order    specifies whether an array is row-major (C-style) or
 *                 column-major (Fortran-style)
 * @param idx      linear index in an array view
 * @param mode     specifies how to handle a linear index which exceeds array
 * dimensions
 * @return         index
 *
 * @example
 * #include "ndarray/index_modes.h"
 * #include "ndarray/orders.h"
 * #include "ndarray/base/bind2vind.h"
 *
 * int64_t ndims = 2;
 * int64_t shape[] = {3, 3};
 * int64_t strides[] = {-3, 1};
 * int64_t offset = 6;
 *
 * int64_t idx = ndarray_bind2vind(
 *     ndims, shape, strides, offset, NDARRAY_ROW_MAJOR, 7,
 *     NDARRAY_INDEX_ERROR);
 * // returns 1
 */
int64_t ndarray_bind2vind(
    int64_t ndims, int64_t* shape, int64_t* strides, int64_t offset,
    enum NDARRAY_ORDER order, int64_t idx, enum NDARRAY_INDEX_MODE mode
) {
  int64_t len;
  int64_t ind;
  int64_t s;
  int64_t k;
  int64_t i;

  len = 1;
  for (i = 0; i < ndims; i++) {
    len *= shape[i];
  }
  if (mode == NDARRAY_INDEX_CLAMP) {
    if (idx < 0) {
      idx = 0;
    } else if (idx >= len) {
      idx = len - 1;
    }
  } else if (mode == NDARRAY_INDEX_WRAP) {
    if (idx < 0) {
      idx += len;  // slight optimization to avoid modulo arithmetic when
                   // |idx| <= len
      if (idx < 0) {
        idx -= len *
               ((int64_t)(idx / len)
               );  // this is equivalent to `idx mod len`, where the result has
                   // same sign as dividend (i.e., `idx`); cannot use `%` as the
                   // sign of the result is implementation defined in C
        if (idx != 0) {
          idx += len;
        }
      }
    } else if (idx >= len) {
      idx -= len;  // slight optimization to avoid modulo arithmetic when
                   // len < idx <= 2*len
      if (idx >= len) {
        idx %= len;
      }
    }
  } else if (idx < 0 || idx >= len) {
    return -1;
  }
  // The approach which follows is to resolve a buffer index to its subscripts
  // and then plug the subscripts into the standard formula for computing the
  // linear index in the array view (i.e., where all strides are positive and
  // offset is 0)...
  ind = 0;
  if (order == NDARRAY_COLUMN_MAJOR) {
    for (i = ndims - 1; i >= 0; i--) {
      s = strides[i];
      if (s < 0) {
        k = idx / s;  // truncates
        idx -= k * s;
        k += shape[i] - 1;
      } else {
        k = idx / s;  // cppcheck-suppress zerodivcond // truncates
        idx -= k * s;
      }
      ind += k * llabs(s);
    }
    return ind;
  }
  // Case: row-major
  for (i = 0; i < ndims; i++) {
    s = strides[i];
    if (s < 0) {
      k = idx / s;  // truncates
      idx -= k * s;
      k += shape[i] - 1;
    } else {
      k = idx / s;  // cppcheck-suppress zerodivcond // truncates
      idx -= k * s;
    }
    ind += k * llabs(s);
  }
  return ind;
}
