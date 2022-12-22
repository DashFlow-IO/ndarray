/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_UNARY_INTERNAL_SORT2INS_H
#define NDARRAY_BASE_UNARY_INTERNAL_SORT2INS_H

#include <stdint.h>

/**
 * Simultaneously sorts two arrays based on the sort order of the first array
 * using insertion sort.
 */
void ndarray_base_unary_internal_sort2ins(
    const int64_t n, int64_t* x, int64_t* y
);

#endif  // !NDARRAY_BASE_UNARY_INTERNAL_SORT2INS_H