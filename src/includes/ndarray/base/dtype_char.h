/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_DTYPE_CHAR_H
#define NDARRAY_BASE_DTYPE_CHAR_H

#include <stdint.h>
#include "ndarray/dtypes.h"

/*
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Enumeration of the single letter character abbreviation for strided array
 * data types.
 */
enum NDARRAY_DTYPE_CHAR {
  NDARRAY_BOOL_CHAR       = 'x',

  NDARRAY_INT8_CHAR       = 's',  // *s*byte (signed byte)
  NDARRAY_UINT8_CHAR      = 'b',  // *b*yte
  NDARRAY_UINT8C_CHAR     = 'a',
  NDARRAY_INT16_CHAR      = 'k',
  NDARRAY_UINT16_CHAR     = 't',
  NDARRAY_INT32_CHAR      = 'i',  // signed 32-bit *i*nteger
  NDARRAY_UINT32_CHAR     = 'u',  // *u*nsigned 32-bit integer
  NDARRAY_INT64_CHAR      = 'l',  // signed *l*ong long (64-bit) integer
  NDARRAY_UINT64_CHAR     = 'v',
  NDARRAY_INT128_CHAR     = 'm',
  NDARRAY_UINT128_CHAR    = 'w',
  NDARRAY_INT256_CHAR     = 'n',
  NDARRAY_UINT256_CHAR    = 'y',

  NDARRAY_FLOAT16_CHAR    = 'h',  // *h*alf-precision floating-point
  NDARRAY_BFLOAT16_CHAR   = 'e',
  NDARRAY_FLOAT32_CHAR    = 'f',  // *f*loat
  NDARRAY_FLOAT64_CHAR    = 'd',  // *d*ouble
  NDARRAY_FLOAT128_CHAR   = 'g',

  NDARRAY_COMPLEX64_CHAR  = 'c',  // *c*omplex (BLAS convention)
  NDARRAY_COMPLEX128_CHAR = 'z',  // *z* (BLAS convention)

  NDARRAY_BINARY_CHAR     = 'r',  // "*r*aw" data
  NDARRAY_GENERIC_CHAR    = 'o',  // "generic" JavaScript objects
};

/**
 * Returns the single letter character abbreviation for a given data type.
 */
uint8_t stdlib_ndarray_dtype_char(enum NDARRAY_DTYPE dtype);

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_BASE_DTYPE_CHAR_H
