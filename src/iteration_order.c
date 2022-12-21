/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include "ndarray/base/iteration_order.h"
#include <stdint.h>

/**
 * Determines array iteration order given a stride array.
 *
 * ## Notes
 *
 * The function returns one of the following values:
 *
 * -   `1`: left-to-right iteration order (strides are all nonnegative).
 * -   `-1`: right-to-left iteration order (strides are all negative).
 * -   `0`: unordered (strides are of mixed sign).
 *
 * @param ndims    number of dimensions
 * @param strides  array strides
 * @return         iteration order
 *
 * @example
 * #include "ndarray/base/iteration_order.h"
 *
 * uint64_t ndims = 2;
 * int64_t strides[] = {2, 1};
 *
 * int8_t o = ndarray_iteration_order(ndims, strides);
 * // returns 1
 */
int8_t ndarray_iteration_order(int64_t ndims, int64_t* strides) {
  int64_t cnt;
  int64_t i;

  cnt = 0;
  for (i = 0; i < ndims; i++) {
    if (strides[i] < 0) {
      cnt += 1;
    }
  }
  if (cnt == 0) {
    // All nonnegative strides:
    return 1;
  }
  if (cnt == ndims) {
    // All negative strides:
    return -1;
  }
  // Strides of mixed signs:
  return 0;
}
