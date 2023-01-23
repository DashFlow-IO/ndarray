/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_UNARY_DISPATCH_OBJECT_H
#define NDARRAY_BASE_UNARY_DISPATCH_OBJECT_H

#include <stdint.h>
#include "ndarray/base/unary/typedefs.h"

/*
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
* Structure for grouping unary function dispatch information.
*
* @example
* #include "ndarray/base/unary/b_b.h"
* #include <stdlib.h>
* #include <stdio.h>
*
* ndarrayUnaryFcn functions[] = {
*     ndarray_b_b_0d,
*     ndarray_b_b_1d,
*     ndarray_b_b_2d,
*     ndarray_b_b_3d,
*     ndarray_b_b_4d,
*     ndarray_b_b_5d,
*     ndarray_b_b_6d,
*     ndarray_b_b_7d,
*     ndarray_b_b_8d,
*     ndarray_b_b_9d,
*     ndarray_b_b_10d,
*     ndarray_b_b_nd,
* };

* ndarrayUnaryFcn blocked_functions[] = {
*     ndarray_b_b_2d_blocked,
*     ndarray_b_b_3d_blocked,
*     ndarray_b_b_4d_blocked,
*     ndarray_b_b_5d_blocked,
*     ndarray_b_b_6d_blocked,
*     ndarray_b_b_7d_blocked,
*     ndarray_b_b_8d_blocked,
*     ndarray_b_b_9d_blocked,
*     ndarray_b_b_10d_blocked,
* };
*
* ndarrayUnaryDispatchObject obj = {
*     functions,
*     12,
*     blocked_functions,
*     9,
* };
*
* // ...
*/
struct ndarrayUnaryDispatchObject {
  // Array containing unary ndarray functions for performing element-wise
  // computation:
  ndarrayUnaryFcn* functions;

  // Number of unary ndarray functions:
  int32_t nfunctions;

  // Array containing unary ndarray functions for performing element-wise
  // computation using loop blocking:
  ndarrayUnaryFcn* blocked_functions;

  // Number of blocked unary ndarray functions:
  int32_t nblockedfunctions;
};

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_BASE_UNARY_DISPATCH_OBJECT_H
