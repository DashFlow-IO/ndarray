/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include "ndarray/base/shape2strides.h"
#include <stdint.h>
#include "ndarray/orders.h"

/**
 * Generates a stride array from an array shape.
 *
 * @param ndims  number of dimensions
 * @param shape  array shape (dimensions)
 * @param order  specifies whether an array is row-major (C-style) or
 *               column-major (Fortran-style)
 * @param out    output strides array
 * @return       status code
 *
 * @example
 * #include "ndarray/base/shape2strides.h"
 * #include "ndarray/orders.h"
 *
 * int64_t ndims = 3;
 * int64_t shape[] = {2, 3, 10};
 * int64_t out[3];
 *
 * ndarray_shape2strides(ndims, shape, NDARRAY_ROW_MAJOR, out);
 */
int8_t ndarray_shape2strides(
    int64_t ndims, int64_t* shape, enum NDARRAY_ORDER order, int64_t* out
) {
  int64_t i;
  int64_t s;

  s = 1;
  if (order == NDARRAY_COLUMN_MAJOR) {
    for (i = 0; i < ndims; i++) {
      out[i] = s;
      s *= shape[i];
    }
  } else {  // row-major
    for (i = ndims - 1; i >= 0; i--) {
      out[i] = s;
      s *= shape[i];
    }
  }
  return 0;
}
