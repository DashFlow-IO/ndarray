/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_SHAPE2STRIDES_H
#define NDARRAY_BASE_SHAPE2STRIDES_H

#include <stdint.h>
#include "ndarray/orders.h"

/*
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Generates a stride array from an array shape.
 */
int8_t ndarray_shape2strides(
    int64_t ndims, int64_t* shape, enum NDARRAY_ORDER order, int64_t* out
);

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_BASE_SHAPE2STRIDES_H
