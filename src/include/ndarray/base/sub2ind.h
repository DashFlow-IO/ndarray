/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_SUB2IND_H
#define NDARRAY_BASE_SUB2IND_H

#include <stdint.h>
#include "ndarray/index_modes.h"

/*
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Converts subscripts to a linear index.
 */
int64_t ndarray_sub2ind(
    int64_t ndims, int64_t* shape, int64_t* strides, int64_t offset,
    int64_t* sub, int64_t nmodes, int8_t* modes
);

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_BASE_SUB2IND_H
