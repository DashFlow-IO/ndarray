/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include "ndarray/base/minmax_view_buffer_index.h"
#include <stdint.h>

/**
 * Computes the minimum and maximum linear indices (in bytes) in an underlying
 * data buffer accessible to an array view.
 *
 * @param ndims    number of dimensions
 * @param shape    array shape (dimensions)
 * @param strides  array strides (in bytes)
 * @param offset   index offset
 * @param out      2-element output array
 * @return         status code
 *
 * @example
 * #include "ndarray/base/minmax_view_buffer_index.h"
 * #include <stdint.h>
 *
 * int64_t ndims = 2;
 * int64_t shape[] = {10, 10};
 * int64_t strides[] = {10, 1};
 * int64_t offset = 0;
 * int64_t out[2];
 *
 * ndarray_minmax_view_buffer_index(ndims, shape, strides, offset, out);
 *
 * int64_t min = out[0];
 * // returns 0
 *
 * int64_t max = out[1];
 * // returns 99
 */
int8_t ndarray_minmax_view_buffer_index(
    int64_t ndims, int64_t* shape, int64_t* strides, int64_t offset,
    int64_t* out
) {
  int64_t min;
  int64_t max;
  int64_t s;
  int64_t i;

  min = offset;
  max = offset;
  for (i = 0; i < ndims; i++) {
    if (shape[i] == 0) {
      out[0] = offset;
      out[1] = offset;
      return 0;
    }
    s = strides[i];
    if (s > 0) {
      max += s * (shape[i] - 1);
    } else if (s < 0) {
      min += s * (shape[i] - 1);  // decrements min
    }
  }
  out[0] = min;
  out[1] = max;

  return 0;
}
