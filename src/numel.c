/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include "ndarray/base/numel.h"
#include <stdint.h>

/**
 * Returns the number of elements in an array.
 *
 * @param ndims  number of dimensions
 * @param shape  array shape (dimensions)
 * @return       number of elements
 *
 * @example
 * #include "ndarray/base/numel.h"
 *
 * int64_t ndims = 2;
 * int64_t shape[] = {10, 8};
 *
 * int64_t n = ndarray_numel(ndims, shape);
 * // returns 80
 */
int64_t ndarray_numel(int64_t ndims, int64_t* shape) {
  int64_t n;
  int64_t i;

  if (ndims == 0) {
    return 0;
  }
  n = 1;
  for (i = 0; i < ndims; i++) {
    if (shape[i] < 0) {
      return 0;
    }
    n *= shape[i];
  }
  return n;
}
