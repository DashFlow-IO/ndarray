/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include <stdint.h>
#include "ndarray/base/clamp_index.h"
#include "ndarray/base/wrap_index.h"
#include "ndarray/index_modes.h"

/**
 * Returns an index given an index mode.
 *
 * ## Notes
 *
 * -   The function returns `-1` if an index is out-of-bounds.
 *
 * @param idx   index
 * @param max   maximum index (should be nonnegative)
 * @param mode  index mode specifying how to handle an index outside the
 *              interval `[0,max]`
 * @return      index
 *
 * @example
 * #include "ndarray/index_modes.h"
 * #include "ndarray/base/ind.h"
 *
 * int64_t idx = ndarray_ind(10, 8, NDARRAY_INDEX_CLAMP);
 * // returns 8
 *
 * @example
 * #include "ndarray/index_modes.h"
 * #include "ndarray/base/ind.h"
 *
 * int64_t idx = ndarray_ind(13, 10, NDARRAY_INDEX_WRAP);
 * // returns 2
 *
 * @example
 * #include "ndarray/index_modes.h"
 * #include "ndarray/base/ind.h"
 *
 * int64_t idx = ndarray_ind(10, 8, NDARRAY_INDEX_ERROR);
 * // returns -1
 */
int64_t ndarray_ind(
    const int64_t idx, const int64_t max, const enum NDARRAY_INDEX_MODE mode
) {
  if (mode == NDARRAY_INDEX_CLAMP) {
    return ndarray_clamp_index(idx, max);
  }
  if (mode == NDARRAY_INDEX_WRAP) {
    return ndarray_wrap_index(idx, max);
  }
  if (idx < 0 || idx > max) {
    return -1;  // out-of-bounds
  }
  return idx;
}
