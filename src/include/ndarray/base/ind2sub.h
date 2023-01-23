/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_IND2SUB_H
#define NDARRAY_BASE_IND2SUB_H

#include <stdint.h>
#include "ndarray/index_modes.h"
#include "ndarray/orders.h"

/*
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Converts a linear index to an array of subscripts.
 */
int8_t ndarray_ind2sub(
    int64_t ndims, int64_t* shape, int64_t* strides, int64_t offset,
    enum NDARRAY_ORDER order, int64_t idx, enum NDARRAY_INDEX_MODE mode,
    int64_t* out
);

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_BASE_IND2SUB_H
