/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include "ndarray/base/strides2order.h"
#include <stdint.h>
#include <stdlib.h>

/**
 * Determines the order of a multidimensional array based on a provided stride
 * array.
 *
 * ## Notes
 *
 * The function returns one of the following values:
 *
 * -   `0`: neither row-major nor column-major.
 * -   `1`: row-major (C-style).
 * -   `2`: column-major (Fortran-style).
 * -   `3`: both row-major and column-major.
 *
 * @param ndims    number of dimensions
 * @param strides  array strides
 * @return         order
 *
 * @example
 * #include "ndarray/base/strides2order.h"
 *
 * int64_t ndims = 2;
 * int64_t strides[] = {2, 1};
 *
 * int8_t o = ndarray_strides2order(ndims, strides);
 * // returns 1
 */
int8_t ndarray_strides2order(int64_t ndims, int64_t* strides) {
  int8_t column;
  int8_t row;
  int64_t s1;
  int64_t s2;
  int64_t i;

  if (ndims == 0) {
    return 0;  // 'none'
  }
  column = 1;
  row    = 1;

  s1     = llabs(strides[0]);
  for (i = 1; i < ndims; i++) {
    s2 = llabs(strides[i]);
    if (column && s2 < s1) {
      column = 0;
    } else if (row && s2 > s1) {
      row = 0;
    }
    if (row || column) {
      s1 = s2;
    } else {
      return 0;  // 'none'
    }
  }
  if (row && column) {
    return 3;  // 'both'
  }
  if (row) {
    return 1;  // 'row-major'
  }
  return 2;  // 'column-major'
}
