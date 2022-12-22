/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_UNARY_DISPATCH_H
#define NDARRAY_BASE_UNARY_DISPATCH_H

#include <stdint.h>
#include "ndarray.h"
#include "ndarray/base/unary/dispatch_object.h"

/*
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Dispatches to a unary ndarray function according to the dimensionality of
 * provided ndarray arguments.
 */
int8_t ndarray_unary_dispatch(
    const struct ndarrayUnaryDispatchObject* obj, struct ndarray* arrays[],
    void* fcn
);

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_BASE_UNARY_DISPATCH_H