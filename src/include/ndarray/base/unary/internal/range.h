/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_UNARY_INTERNAL_RANGE_H
#define NDARRAY_BASE_UNARY_INTERNAL_RANGE_H

#include <stdint.h>

/**
 * Writes `n` evenly spaced values from `0` to `n-1` to an output array.
 */
void ndarray_base_unary_internal_range(const int64_t n, int64_t* out);

#endif  // !NDARRAY_BASE_UNARY_INTERNAL_RANGE_H
