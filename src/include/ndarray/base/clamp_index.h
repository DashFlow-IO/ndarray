/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include <stdint.h>

/**
 * Restricts an index to the interval `[0,max]`.
 *
 * @param idx  index
 * @param max  maximum index (should be nonnegative)
 * @return     index
 *
 * @example
 * #include "ndarray/base/clamp_index.h"
 *
 * int64_t idx = ndarray_clamp_index(10, 8);
 * // returns 8
 */
int64_t ndarray_clamp_index(int64_t idx, int64_t max) {
  if (idx < 0) {
    return 0;
  }
  if (idx > max) {
    return max;
  }
  return idx;
}