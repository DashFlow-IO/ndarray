/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include "ndarray/base/singleton_dimensions.h"
#include <stdint.h>

/**
 * Returns the number of singleton dimensions.
 *
 * @param ndims  number of dimensions
 * @param shape  array shape (dimensions)
 * @return       number of singleton dimensions
 *
 * @example
 * #include "ndarray/base/singleton_dimensions.h"
 *
 * int64_t ndims = 2;
 * int64_t shape[] = {10, 1};
 *
 * int64_t n = ndarray_singleton_dimensions(ndims, shape);
 * // returns 1
 */
int64_t ndarray_singleton_dimensions(int64_t ndims, int64_t* shape) {
  int64_t n;
  int64_t i;

  n = 0;
  for (i = 0; i < ndims; i++) {
    if (shape[i] == 1) {
      n += 1;
    }
  }
  return n;
}
