/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_UNARY_TYPEDEFS_H
#define NDARRAY_BASE_UNARY_TYPEDEFS_H

#include <stdint.h>
#include "ndarray.h"

/**
 * Function pointer type for a unary ndarray function.
 *
 * ## Note
 *
 * -   This must match the definition of an `ndarrayFcn` found in
 *     ndarray/base/function-object`.
 *
 * @param arrays   array containing pointers to input and output ndarrays
 * @param data     function "data" (e.g., a callback)
 * @return         status code
 */
typedef int8_t (*ndarrayUnaryFcn)(struct ndarray* arrays[], void* data);

#endif  // !NDARRAY_BASE_UNARY_TYPEDEFS_H
