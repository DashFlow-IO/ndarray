/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include "ndarray/base/dtype_char.h"
#include <stdint.h>
#include "ndarray/dtypes.h"

/**
 * Returns the one letter character abbreviation for a provided data type.
 *
 * ## Notes
 *
 * -   The function returns `0` if provided an unknown/unsupported data type.
 *
 * @param dtype  data type (enumeration constant)
 * @return       one letter character abbreviation
 *
 * @example
 * #include "ndarray/base/dtype_char.h"
 * #include "ndarray/dtypes.h"
 * #include <stdint.h>
 *
 * uint8_t ch = ndarray_dtype_char(NDARRAY_FLOAT64);
 * // returns 100
 */
uint8_t ndarray_dtype_char(enum NDARRAY_DTYPE dtype) {
  switch (dtype) {
    case NDARRAY_FLOAT64:
      return NDARRAY_FLOAT64_CHAR;
    case NDARRAY_FLOAT32:
      return NDARRAY_FLOAT32_CHAR;

    case NDARRAY_INT8:
      return NDARRAY_INT8_CHAR;
    case NDARRAY_UINT8:
      return NDARRAY_UINT8_CHAR;
    case NDARRAY_UINT8C:
      return NDARRAY_UINT8C_CHAR;

    case NDARRAY_INT16:
      return NDARRAY_INT16_CHAR;
    case NDARRAY_UINT16:
      return NDARRAY_UINT16_CHAR;

    case NDARRAY_INT32:
      return NDARRAY_INT32_CHAR;
    case NDARRAY_UINT32:
      return NDARRAY_UINT32_CHAR;

    case NDARRAY_INT64:
      return NDARRAY_INT64_CHAR;
    case NDARRAY_UINT64:
      return NDARRAY_UINT64_CHAR;

    case NDARRAY_BOOL:
      return NDARRAY_BOOL_CHAR;

    case NDARRAY_BINARY:
      return NDARRAY_BINARY_CHAR;

    case NDARRAY_COMPLEX64:
      return NDARRAY_COMPLEX64_CHAR;
    case NDARRAY_COMPLEX128:
      return NDARRAY_COMPLEX128_CHAR;

    case NDARRAY_GENERIC:
      return NDARRAY_GENERIC_CHAR;

    default:
      return 0;
  }
}
