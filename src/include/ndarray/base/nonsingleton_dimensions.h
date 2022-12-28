/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_NONSINGELTON_DIMENSIONS_H
#define NDARRAY_BASE_NONSINGELTON_DIMENSIONS_H

#include <stdint.h>

/*
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns the number of non-singleton dimensions.
 */
int64_t ndarray_nonsingleton_dimensions(int64_t ndims, int64_t* shape);

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_BASE_NONSINGELTON_DIMENSIONS_H