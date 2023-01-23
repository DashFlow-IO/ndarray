/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include "ndarray/base/strides2offset.h"
#include <stdint.h>

/**
 * Returns the index offset which specifies the location of the first indexed
 * value in a multidimensional array based on a stride array.
 *
 * @param ndims    number of dimensions
 * @param shape    array shape (dimensions)
 * @param strides  array strides
 * @return         offset
 *
 * @example
 * #include "ndarray/base/strides2offset.h"
 *
 * int64_t ndims = 3;
 * int64_t shape[] = {2, 3, 10};
 * int64_t strides[] = {30, -10, 1};
 *
 * int64_t offset = ndarray_strides2offset(ndims, shape, strides);
 * // returns 20
 */
int64_t ndarray_strides2offset(
    int64_t ndims, int64_t* shape, int64_t* strides
) {
  int64_t offset;
  int64_t i;

  offset = 0;
  for (i = 0; i < ndims; i++) {
    if (strides[i] < 0) {
      // Note that, since the stride is negative, this operation increments, not
      // decrements, the offset...
      offset -= strides[i] * (shape[i] - 1);
    }
  }
  return offset;
}
