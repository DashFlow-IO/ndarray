/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_BROADCAST_SHAPES_H
#define NDARRAY_BASE_BROADCAST_SHAPES_H

#include <stdint.h>

/*
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Broadcasts array shapes to a single shape.
 */
int8_t ndarray_broadcast_shapes(
    int64_t M, int64_t* shapes[], int64_t ndims[], int64_t* out
);

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_BASE_BROADCAST_SHAPES_H
