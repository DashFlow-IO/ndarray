/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_STRIDES2OFFSET_H
#define NDARRAY_BASE_STRIDES2OFFSET_H

#include <stdint.h>

/*
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns the index offset which specifies the location of the first indexed
 * value in a multidimensional array based on a stride array.
 */
int64_t ndarray_strides2offset(int64_t ndims, int64_t* shape, int64_t* strides);

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_BASE_STRIDES2OFFSET_H