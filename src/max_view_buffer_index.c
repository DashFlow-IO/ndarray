/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include "ndarray/base/max_view_buffer_index.h"
#include <stdint.h>

/**
 * Computes the maximum linear index (in bytes) in an underlying data buffer
 * accessible to an array view.
 *
 * @param ndims    number of dimensions
 * @param shape    array shape (dimensions)
 * @param strides  array strides (in bytes)
 * @param offset   index offset
 * @return         maximum linear index
 *
 * @example
 * #include "ndarray/base/max_view_buffer_index.h"
 * #include <stdint.h>
 *
 * int64_t ndims = 2;
 * int64_t shape[] = {10, 10};
 * int64_t strides[] = {10, 1};
 * int64_t offset = 0;
 *
 * int64_t idx = ndarray_max_view_buffer_index(ndims, shape, strides, offset);
 * // returns 99
 */
int64_t ndarray_max_view_buffer_index(
    int64_t ndims, int64_t* shape, int64_t* strides, int64_t offset
) {
  int64_t idx;
  int64_t i;

  idx = offset;
  for (i = 0; i < ndims; i++) {
    if (shape[i] == 0) {
      return offset;
    }
    if (strides[i] > 0) {
      idx += strides[i] * (shape[i] - 1);
    }
  }
  return idx;
}
