/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_BYTES_PER_ELEMENT_H
#define NDARRAY_BASE_BYTES_PER_ELEMENT_H

#include <stdint.h>
#include "ndarray/dtypes.h"

/**
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Enumeration of number of bytes necessary to store an ndarray data type.
 */
enum NDARRAY_BYTES_PER_ELEMENT {
  NDARRAY_BOOL_BYTES_PER_ELEMENT       = 1,

  NDARRAY_INT8_BYTES_PER_ELEMENT       = 1,
  NDARRAY_UINT8_BYTES_PER_ELEMENT      = 1,
  NDARRAY_UINT8C_BYTES_PER_ELEMENT     = 1,
  NDARRAY_INT16_BYTES_PER_ELEMENT      = 2,
  NDARRAY_UINT16_BYTES_PER_ELEMENT     = 2,
  NDARRAY_INT32_BYTES_PER_ELEMENT      = 4,
  NDARRAY_UINT32_BYTES_PER_ELEMENT     = 4,
  NDARRAY_INT64_BYTES_PER_ELEMENT      = 8,
  NDARRAY_UINT64_BYTES_PER_ELEMENT     = 8,
  NDARRAY_INT128_BYTES_PER_ELEMENT     = 16,
  NDARRAY_UINT128_BYTES_PER_ELEMENT    = 16,
  NDARRAY_INT256_BYTES_PER_ELEMENT     = 32,
  NDARRAY_UINT256_BYTES_PER_ELEMENT    = 32,

  NDARRAY_FLOAT16_BYTES_PER_ELEMENT    = 2,
  NDARRAY_BFLOAT16_BYTES_PER_ELEMENT   = 2,
  NDARRAY_FLOAT32_BYTES_PER_ELEMENT    = 4,
  NDARRAY_FLOAT64_BYTES_PER_ELEMENT    = 8,
  NDARRAY_FLOAT128_BYTES_PER_ELEMENT   = 16,

  NDARRAY_COMPLEX64_BYTES_PER_ELEMENT  = 8,
  NDARRAY_COMPLEX128_BYTES_PER_ELEMENT = 16,

  NDARRAY_BINARY_BYTES_PER_ELEMENT     = 1
};

/**
 * Returns the number of bytes per element for a given data type.
 */
int64_t ndarray_bytes_per_element(enum NDARRAY_DTYPE dtype);

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_BASE_BYTES_PER_ELEMENT_H
