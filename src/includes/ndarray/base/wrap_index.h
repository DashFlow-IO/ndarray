/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include <stdint.h>

/**
 * Wraps an index on the interval `[0,max]`.
 *
 * @param idx  index
 * @param max  maximum index (should be nonnegative)
 * @return     index
 *
 * @example
 * #include "ndarray/base/wrap_index.h"
 *
 * int64_t idx = ndarray_wrap_index(13, 10);
 * // returns 2
 */
int64_t ndarray_wrap_index(int64_t idx, int64_t max) {
  int64_t mp1 = max + 1;  // WARNING: possibility of overflow (although, in
                          // practice, `max` should never be that large)
  if (idx < 0) {
    idx += mp1;  // slight optimization to avoid modulo arithmetic when
                 // |idx| <= max+1
    if (idx < 0) {
      idx -=
          mp1 * ((int64_t)(idx / mp1)
                );  // this is equivalent to `idx mod mp1`, where the result has
                    // same sign as dividend (i.e., `idx`); cannot use `%` as
                    // the sign of the result is implementation defined in C
      if (idx != 0) {
        idx += mp1;
      }
    }
    return idx;
  }
  if (idx > max) {
    idx -= mp1;  // slight optimization to avoid modulo arithmetic when
                 // max+1 < idx <= 2*(max+1)
    if (idx > max) {
      idx %= mp1;
    }
    return idx;
  }
  return idx;
}
