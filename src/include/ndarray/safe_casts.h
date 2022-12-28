/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_SAFE_CASTS_H
#define NDARRAY_SAFE_CASTS_H

#include <stdint.h>

#include "dtypes.h"

/*
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

const int8_t NDARRAY_SAFE_CASTS_FLOAT64[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,

    [NDARRAY_INT16]      = 0,
    [NDARRAY_UINT16]     = 0,
    [NDARRAY_INT32]      = 0,
    [NDARRAY_UINT32]     = 0,
    [NDARRAY_INT64]      = 0,
    [NDARRAY_UINT64]     = 0,

    [NDARRAY_FLOAT32]    = 0,
    [NDARRAY_FLOAT64]    = 1,

    [NDARRAY_COMPLEX64]  = 0,
    [NDARRAY_COMPLEX128] = 1,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0};

const int8_t NDARRAY_SAFE_CASTS_FLOAT32[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,
    [NDARRAY_INT16]      = 0,
    [NDARRAY_UINT16]     = 0,
    [NDARRAY_INT32]      = 0,
    [NDARRAY_UINT32]     = 0,
    [NDARRAY_INT64]      = 0,
    [NDARRAY_UINT64]     = 0,

    [NDARRAY_FLOAT32]    = 1,
    [NDARRAY_FLOAT64]    = 1,

    [NDARRAY_COMPLEX64]  = 1,
    [NDARRAY_COMPLEX128] = 1,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_UINT64[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,
    [NDARRAY_INT16]      = 0,
    [NDARRAY_UINT16]     = 0,
    [NDARRAY_INT32]      = 0,
    [NDARRAY_UINT32]     = 0,
    [NDARRAY_INT64]      = 0,
    [NDARRAY_UINT64]     = 1,

    [NDARRAY_FLOAT32]    = 0,
    [NDARRAY_FLOAT64]    = 0,

    [NDARRAY_COMPLEX64]  = 0,
    [NDARRAY_COMPLEX128] = 0,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_INT64[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,
    [NDARRAY_INT16]      = 0,
    [NDARRAY_UINT16]     = 0,
    [NDARRAY_INT32]      = 0,
    [NDARRAY_UINT32]     = 0,
    [NDARRAY_INT64]      = 1,
    [NDARRAY_UINT64]     = 0,

    [NDARRAY_FLOAT32]    = 0,
    [NDARRAY_FLOAT64]    = 0,

    [NDARRAY_COMPLEX64]  = 0,
    [NDARRAY_COMPLEX128] = 0,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_UINT32[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,
    [NDARRAY_INT16]      = 0,
    [NDARRAY_UINT16]     = 0,
    [NDARRAY_INT32]      = 0,
    [NDARRAY_UINT32]     = 1,
    [NDARRAY_INT64]      = 1,
    [NDARRAY_UINT64]     = 1,

    [NDARRAY_FLOAT32]    = 0,
    [NDARRAY_FLOAT64]    = 1,

    [NDARRAY_COMPLEX64]  = 0,
    [NDARRAY_COMPLEX128] = 1,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_INT32[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,
    [NDARRAY_INT16]      = 0,
    [NDARRAY_UINT16]     = 0,
    [NDARRAY_INT32]      = 1,
    [NDARRAY_UINT32]     = 0,
    [NDARRAY_INT64]      = 1,
    [NDARRAY_UINT64]     = 0,

    [NDARRAY_FLOAT32]    = 0,
    [NDARRAY_FLOAT64]    = 1,

    [NDARRAY_COMPLEX64]  = 0,
    [NDARRAY_COMPLEX128] = 1,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_UINT16[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,
    [NDARRAY_INT16]      = 0,
    [NDARRAY_UINT16]     = 1,
    [NDARRAY_INT32]      = 1,
    [NDARRAY_UINT32]     = 1,
    [NDARRAY_INT64]      = 1,
    [NDARRAY_UINT64]     = 1,

    [NDARRAY_FLOAT32]    = 1,
    [NDARRAY_FLOAT64]    = 1,

    [NDARRAY_COMPLEX64]  = 1,
    [NDARRAY_COMPLEX128] = 1,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_INT16[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,
    [NDARRAY_INT16]      = 1,
    [NDARRAY_UINT16]     = 0,
    [NDARRAY_INT32]      = 1,
    [NDARRAY_UINT32]     = 0,
    [NDARRAY_INT64]      = 1,
    [NDARRAY_UINT64]     = 0,

    [NDARRAY_FLOAT32]    = 1,
    [NDARRAY_FLOAT64]    = 1,

    [NDARRAY_COMPLEX64]  = 1,
    [NDARRAY_COMPLEX128] = 1,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_UINT8[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 1,
    [NDARRAY_UINT8C]     = 1,
    [NDARRAY_INT16]      = 1,
    [NDARRAY_UINT16]     = 1,
    [NDARRAY_INT32]      = 1,
    [NDARRAY_UINT32]     = 1,
    [NDARRAY_INT64]      = 1,
    [NDARRAY_UINT64]     = 1,

    [NDARRAY_FLOAT32]    = 1,
    [NDARRAY_FLOAT64]    = 1,

    [NDARRAY_COMPLEX64]  = 1,
    [NDARRAY_COMPLEX128] = 1,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_UINT8C[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 1,
    [NDARRAY_UINT8C]     = 1,
    [NDARRAY_INT16]      = 1,
    [NDARRAY_UINT16]     = 1,
    [NDARRAY_INT32]      = 1,
    [NDARRAY_UINT32]     = 1,
    [NDARRAY_INT64]      = 1,
    [NDARRAY_UINT64]     = 1,

    [NDARRAY_FLOAT32]    = 1,
    [NDARRAY_FLOAT64]    = 1,

    [NDARRAY_COMPLEX64]  = 1,
    [NDARRAY_COMPLEX128] = 1,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_INT8[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 1,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,
    [NDARRAY_INT16]      = 1,
    [NDARRAY_UINT16]     = 0,
    [NDARRAY_INT32]      = 1,
    [NDARRAY_UINT32]     = 0,
    [NDARRAY_INT64]      = 1,
    [NDARRAY_UINT64]     = 0,

    [NDARRAY_FLOAT32]    = 1,
    [NDARRAY_FLOAT64]    = 1,

    [NDARRAY_COMPLEX64]  = 1,
    [NDARRAY_COMPLEX128] = 1,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_COMPLEX128[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,
    [NDARRAY_INT16]      = 0,
    [NDARRAY_UINT16]     = 0,
    [NDARRAY_INT32]      = 0,
    [NDARRAY_UINT32]     = 0,
    [NDARRAY_INT64]      = 0,
    [NDARRAY_UINT64]     = 0,

    [NDARRAY_FLOAT32]    = 0,
    [NDARRAY_FLOAT64]    = 0,

    [NDARRAY_COMPLEX64]  = 0,
    [NDARRAY_COMPLEX128] = 1,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_COMPLEX64[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,
    [NDARRAY_INT16]      = 0,
    [NDARRAY_UINT16]     = 0,
    [NDARRAY_INT32]      = 0,
    [NDARRAY_UINT32]     = 0,
    [NDARRAY_INT64]      = 0,
    [NDARRAY_UINT64]     = 0,

    [NDARRAY_FLOAT32]    = 0,
    [NDARRAY_FLOAT64]    = 0,

    [NDARRAY_COMPLEX64]  = 1,
    [NDARRAY_COMPLEX128] = 1,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_BOOL[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,
    [NDARRAY_INT16]      = 0,
    [NDARRAY_UINT16]     = 0,
    [NDARRAY_INT32]      = 0,
    [NDARRAY_UINT32]     = 0,
    [NDARRAY_INT64]      = 0,
    [NDARRAY_UINT64]     = 0,

    [NDARRAY_FLOAT32]    = 0,
    [NDARRAY_FLOAT64]    = 0,

    [NDARRAY_COMPLEX64]  = 0,
    [NDARRAY_COMPLEX128] = 0,

    [NDARRAY_BOOL]       = 1,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_BINARY[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,
    [NDARRAY_INT16]      = 0,
    [NDARRAY_UINT16]     = 0,
    [NDARRAY_INT32]      = 0,
    [NDARRAY_UINT32]     = 0,
    [NDARRAY_INT64]      = 0,
    [NDARRAY_UINT64]     = 0,

    [NDARRAY_FLOAT32]    = 0,
    [NDARRAY_FLOAT64]    = 0,

    [NDARRAY_COMPLEX64]  = 0,
    [NDARRAY_COMPLEX128] = 0,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 1,
    [NDARRAY_GENERIC]    = 0,
};

const int8_t NDARRAY_SAFE_CASTS_GENERIC[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = 0,
    [NDARRAY_UINT8]      = 0,
    [NDARRAY_UINT8C]     = 0,
    [NDARRAY_INT16]      = 0,
    [NDARRAY_UINT16]     = 0,
    [NDARRAY_INT32]      = 0,
    [NDARRAY_UINT32]     = 0,
    [NDARRAY_INT64]      = 0,
    [NDARRAY_UINT64]     = 0,

    [NDARRAY_FLOAT32]    = 0,
    [NDARRAY_FLOAT64]    = 0,

    [NDARRAY_COMPLEX64]  = 0,
    [NDARRAY_COMPLEX128] = 0,

    [NDARRAY_BOOL]       = 0,

    [NDARRAY_BINARY]     = 0,
    [NDARRAY_GENERIC]    = 1,
};

// Define a table of pointers containing the above casting tables:
const int8_t* NDARRAY_SAFE_CASTS[NDARRAY_NDTYPES] = {

    [NDARRAY_INT8]       = NDARRAY_SAFE_CASTS_INT8,
    [NDARRAY_UINT8]      = NDARRAY_SAFE_CASTS_UINT8,
    [NDARRAY_UINT8C]     = NDARRAY_SAFE_CASTS_UINT8C,
    [NDARRAY_INT16]      = NDARRAY_SAFE_CASTS_INT16,
    [NDARRAY_UINT16]     = NDARRAY_SAFE_CASTS_UINT16,
    [NDARRAY_INT32]      = NDARRAY_SAFE_CASTS_INT32,
    [NDARRAY_UINT32]     = NDARRAY_SAFE_CASTS_UINT32,
    [NDARRAY_INT64]      = NDARRAY_SAFE_CASTS_INT64,
    [NDARRAY_UINT64]     = NDARRAY_SAFE_CASTS_UINT64,

    [NDARRAY_FLOAT32]    = NDARRAY_SAFE_CASTS_FLOAT32,
    [NDARRAY_FLOAT64]    = NDARRAY_SAFE_CASTS_FLOAT64,

    [NDARRAY_COMPLEX64]  = NDARRAY_SAFE_CASTS_COMPLEX64,
    [NDARRAY_COMPLEX128] = NDARRAY_SAFE_CASTS_COMPLEX128,

    [NDARRAY_BOOL]       = NDARRAY_SAFE_CASTS_BOOL,

    [NDARRAY_BINARY]     = NDARRAY_SAFE_CASTS_BINARY,
    [NDARRAY_GENERIC]    = NDARRAY_SAFE_CASTS_GENERIC,
};

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_SAFE_CASTS_H
