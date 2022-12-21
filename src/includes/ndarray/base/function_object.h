/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_FUNCTION_OBJECT_H
#define NDARRAY_BASE_FUNCTION_OBJECT_H

#include <stdint.h>
#include "ndarray.h"

/*
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Function pointer type for an ndarray function.
 *
 * @param arrays   array containing pointers to input and output ndarrays
 * @param data     function "data" (e.g., a callback)
 * @return         status code
 */
typedef int8_t (*ndarrayFcn)(struct ndarray* arrays[], void* data);

/**
 * Structure for grouping ndarray function information.
 *
 * @example
 * #include "ndarray/base/function_object.h"
 * #include "ndarray/base/binary.h"
 * #include "ndarray/dtypes.h"
 * #include <stdlib.h>
 * #include <stdio.h>
 *
 * // Define the function(s) we want to apply to ndarrays:
 * double scale(double x) {
 *     return x * 10.0;
 * }
 *
 * struct ndarrayFunctionObject *obj =
 *    malloc(sizeof(struct ndarrayFunctionObject));
 * if (obj == NULL) {
 *    fprintf(stderr, "Error allocating memory.\n");
 *    exit(1);
 * }
 *
 * // As the function to be applied is a unary function, define a unary ndarray
 * // interface:
 * const char name[] = "unary_ndarray_function";
 * obj->name = name;
 *
 * obj->nin = 1;
 * obj->nout = 1;
 * obj->narrays = 2; // (obj->nin) + (obj->nout)
 *
 * // Define a list of ndarray functions (in this case, as the function to be
 * // applied accepts doubles, we only use ndarray functions which handle
 * // doubles as function arguments and, for the purposes of this example, we
 * // assume that the output ndarray is (almost) always a double-precision
 * // floating-point  number array): obj->nfunctions = 8;
 *
 * ndarrayFcn functions[] = {
 *     ndarray_d_d,
 *     ndarray_f_f_as_d_d,
 *     ndarray_I_d_as_d_d,
 *     ndarray_i_d_as_d_d,
 *     ndarray_H_d_as_d_d,
 *     ndarray_h_d_as_d_d,
 *     ndarray_B_d_as_d_d,
 *     ndarray_b_d_as_d_d
 * };
 * obj->functions = functions;
 *
 * // Define the **ndarray** argument types for each ndarray function:
 * int32_t types[] = {
 *     NDARRAY_FLOAT64, NDARRAY_FLOAT64,
 *     NDARRAY_FLOAT32, NDARRAY_FLOAT32,
 *     NDARRAY_UINT32, NDARRAY_FLOAT64,
 *     NDARRAY_INT32, NDARRAY_FLOAT64,
 *     NDARRAY_UINT16, NDARRAY_FLOAT64,
 *     NDARRAY_INT16, NDARRAY_FLOAT64,
 *     NDARRAY_UINT8, NDARRAY_FLOAT64,
 *     NDARRAY_INT8, NDARRAY_FLOAT64
 * };
 * obj->types = types;
 *
 * // Define a list of ndarray function "data" (in this case, callbacks):
 * void *data[] = {
 *     (void *)scale,
 *     (void *)scale,
 *     (void *)scale,
 *     (void *)scale,
 *     (void *)scale,
 *     (void *)scale,
 *     (void *)scale,
 *     (void *)scale
 * };
 * obj->data = data;
 *
 * // Free allocated memory:
 * free(obj);
 */
struct ndarrayFunctionObject {
  // ndarray function name:
  const char* name;

  // Number of input ndarrays:
  int32_t nin;

  // Number of output ndarrays:
  int32_t nout;

  // Total number of ndarray arguments (nin + nout):
  int32_t narrays;

  // Array containing ndarray functions:
  ndarrayFcn* functions;

  // Number of ndarray functions:
  int32_t nfunctions;

  // Array of type "numbers" (as enumerated elsewhere), where the total number
  // of types equals `narrays * nfunctions` and where each set of `narrays`
  // consecutive types (non-overlapping) corresponds to the set of ndarray
  // argument types for a corresponding ndarray function:
  int32_t* types;

  // Array of void pointers corresponding to the "data" (e.g., callbacks) which
  // should be passed to a respective ndarray function (note: the number of
  // pointers should match the number of ndarray functions):
  void** data;
};

/**
 * Returns a pointer to a dynamically allocated ndarray function object.
 */
struct ndarrayFunctionObject* ndarray_function_allocate(
    const char* name, int32_t nin, int32_t nout, ndarrayFcn* functions,
    int32_t nfunctions, int32_t* types, void* data[]
);

/**
 * Frees an ndarray function object's allocated memory.
 */
void ndarray_function_free(struct ndarrayFunctionObject* obj);

/**
 * Returns the first index of a function whose signature satisfies a provided
 * list of array types.
 */
int64_t ndarray_function_dispatch_index_of(
    const struct ndarrayFunctionObject* obj, const int32_t* types
);

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_BASE_FUNCTION_OBJECT_H
