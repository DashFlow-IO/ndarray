/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_UNARY_INTERNAL_PERMUTE_H
#define NDARRAY_BASE_UNARY_INTERNAL_PERMUTE_H

#include <stdint.h>

/**
 * Permutes an input array according to a provided index array.
 */
void ndarray_base_unary_internal_permute(
    const int64_t n, const int64_t* arr, const int64_t* idx, int64_t* out
);

#endif  // !NDARRAY_BASE_UNARY_INTERNAL_PERMUTE_H