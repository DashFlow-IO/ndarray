/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include "ndarray/base/broadcast_shapes.h"
#include <stdint.h>

/**
 * Broadcasts array shapes to a single shape.
 *
 * ## Notes
 *
 * -   Two respective dimensions in two shape arrays are compatible if
 *
 *     1.  the dimensions are equal.
 *     2.  one dimension is `1`.
 *
 * -   If successful, the function returns `0`; otherwise, the function returns
 *     `-1` (e.g., due to incompatible shapes).
 *
 * -   Even if the function is unsuccessful, the function may still overwrite
 *     elements in the output array before returning. In other words, do not
 *     assume that providing incompatible shapes is a no-op with regard to the
 *     output array.
 *
 * @param M        number of array shapes
 * @param shapes   pointer to an array containing shape arrays
 * @param ndims    array containing the number of dimensions for (i.e., length
 *                 of) each respective shape array
 * @param out      pointer to output array
 * @return         status
 *
 * @example
 * #include "ndarray/base/broadcast_shapes.h"
 * #include <stdint.h>
 *
 * int64_t N1 = 4;
 * int64_t sh1[] = {8, 1, 6, 1};
 *
 * int64_t N2 = 3;
 * int64_t sh2[] = {7, 1, 5};
 *
 * int64_t ndims[] = {N1, N2};
 * int64_t *shapes[] = {sh1, sh2};
 *
 * int64_t out[] = {0, 0, 0, 0};
 * int8_t status = ndarray_broadcast_shapes(2, shapes, ndims, out);
 */
int8_t ndarray_broadcast_shapes(
    int64_t M, int64_t* shapes[], int64_t ndims[], int64_t* out
) {
  int64_t dim;
  int64_t* sh;
  int64_t n1;
  int64_t n2;
  int64_t d;
  int64_t N;
  int64_t i;
  int64_t j;

  if (M == 0) {
    return 0;
  }
  sh = shapes[0];
  N  = ndims[0];

  // If provided a single input shape array, then the broadcast shape is input
  // shape...
  if (M == 1) {
    for (i = 0; i < N; i++) {
      out[i] = sh[i];
    }
    return 0;
  }
  // Determine the maximum dimensionality...
  for (i = 1; i < M; i++) {
    if (ndims[i] > N) {
      N = ndims[i];
    }
  }
  // Compute the broadcast shape...
  i = N - 1;
  while (i >= 0) {
    n1 = ndims[0] - N + i;
    if (n1 >= 0) {
      dim = sh[n1];
    } else {
      dim = 1;
    }
    for (j = 1; j < M; j++) {
      n2 = ndims[j] - N + i;
      if (n2 >= 0) {
        d = shapes[j][n2];
      } else {
        d = 1;
      }
      if (dim == 1) {
        dim = d;
        continue;
      }
      if (d == 1 || dim == d) {
        // When either `d` is `1` or `d` equals the current output shape
        // dimension, the current output shape dimension remains the same...
        continue;
      }
      // The current shape cannot be broadcast against one of the other
      // shapes...
      return -1;
    }
    out[i] = dim;
    i -= 1;
  }
  return 0;
}
