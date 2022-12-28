/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_UNARY_MACROS_1D_H
#define NDARRAY_BASE_UNARY_MACROS_1D_H

#include <stdint.h>
#include "ndarray.h"

/**
 * Macro containing the preamble for a loop which operates on elements of a
 * one-dimensional ndarray.
 *
 * ## Notes
 *
 * -   Variable naming conventions:
 *
 *     -   `sx#`, `px#`, and `d@x#` where `#` corresponds to the ndarray
 *         argument number, starting at `1`.
 *     -   `S@`, `i@`, and `d@x#` where `@` corresponds to the loop number, with
 *         `0` being the innermost loop.
 *
 * @example
 * NDARRAY_UNARY_1D_LOOP_PREMABLE {
 *     // Innermost loop body...
 * }
 * NDARRAY_UNARY_1D_LOOP_EPILOGUE
 */
#define NDARRAY_UNARY_1D_LOOP_PREAMBLE                            \
  struct ndarray* x1 = arrays[0];                                 \
  struct ndarray* x2 = arrays[1];                                 \
  int64_t* shape     = ndarray_shape(x1);                         \
  int64_t* sx1       = ndarray_strides(x1);                       \
  int64_t* sx2       = ndarray_strides(x2);                       \
  uint8_t* px1       = ndarray_data(x1);                          \
  uint8_t* px2       = ndarray_data(x2);                          \
  int64_t d0x1;                                                   \
  int64_t d0x2;                                                   \
  int64_t S0;                                                     \
  int64_t i0;                                                     \
  /* Extract loop variables: dimensions and loop offset (pointer) \
   * increments... */                                             \
  S0   = shape[0];                                                \
  d0x1 = sx1[0];                                                  \
  d0x2 = sx2[0];                                                  \
  /* Set the pointers to the first indexed elements... */         \
  px1 += ndarray_offset(x1);                                      \
  px2 += ndarray_offset(x2);                                      \
  /* Iterate over the ndarray dimensions... */                    \
  for (i0 = 0; i0 < S0; i0++, px1 += d0x1, px2 += d0x2)

/**
 * Macro containing the preamble for a loop which operates on elements of a
 * one-dimensional input ndarray and updates two output ndarrays.
 *
 * ## Notes
 *
 * -   Variable naming conventions:
 *
 *     -   `sx#`, `px#`, and `d@x#` where `#` corresponds to the ndarray
 *         argument number, starting at `1`.
 *     -   `S@`, `i@`, and `d@x#` where `@` corresponds to the loop number, with
 *         `0` being the innermost loop.
 *
 * @example
 * NDARRAY_UNARY_1D_LOOP_TWO_OUT_PREMABLE {
 *     // Innermost loop body...
 * }
 * NDARRAY_UNARY_1D_LOOP_EPILOGUE
 */
#define NDARRAY_UNARY_1D_LOOP_TWO_OUT_PREAMBLE                                 \
  struct ndarray* x1 = arrays[0];                                              \
  struct ndarray* x2 = arrays[1];                                              \
  struct ndarray* x3 = arrays[2];                                              \
  int64_t* shape     = ndarray_shape(x1);                                      \
  int64_t* sx1       = ndarray_strides(x1);                                    \
  int64_t* sx2       = ndarray_strides(x2);                                    \
  int64_t* sx3       = ndarray_strides(x3);                                    \
  uint8_t* px1       = ndarray_data(x1);                                       \
  uint8_t* px2       = ndarray_data(x2);                                       \
  uint8_t* px3       = ndarray_data(x3);                                       \
  int64_t d0x1;                                                                \
  int64_t d0x2;                                                                \
  int64_t d0x3;                                                                \
  int64_t S0;                                                                  \
  int64_t i0;                                                                  \
  /* Extract loop variable: dimensions and loop offset (pointer) increments... \
   */                                                                          \
  S0   = shape[0];                                                             \
  d0x1 = sx1[0];                                                               \
  d0x2 = sx2[0];                                                               \
  d0x3 = sx3[0];                                                               \
  /* Set the pointers to the first indexed elements... */                      \
  px1 += ndarray_offset(x1);                                                   \
  px2 += ndarray_offset(x2);                                                   \
  px3 += ndarray_offset(x3);                                                   \
  /* Iterate over the ndarray dimensions... */                                 \
  for (i0 = 0; i0 < S0; i0++, px1 += d0x1, px2 += d0x2, px3 += d0x3)

/**
 * Macro containing the epilogue for loops which operate on elements of a
 * one-dimensional ndarray.
 *
 * @example
 * NDARRAY_UNARY_1D_LOOP_PREMABLE {
 *     // Innermost loop body...
 * }
 * NDARRAY_UNARY_1D_LOOP_EPILOGUE
 */
#define NDARRAY_UNARY_1D_LOOP_EPILOGUE

/**
 * Macro for a unary one-dimensional ndarray loop which inlines an expression.
 *
 * ## Notes
 *
 * -   Retrieves each ndarray element according to type `tin` via the pointer
 *     `px1` as `in1`.
 * -   Creates a pointer `tout *out` to the output ndarray element.
 * -   Expects a provided expression to operate on `tin in1` and to store the
 *     result in `tout *out`.
 *
 * @param tin   input type
 * @param tout  output type
 * @param expr  expression to inline
 *
 * @example
 * NDARRAY_UNARY_1D_LOOP_INLINE(double, double, *out = in1 * in1)
 */
#define NDARRAY_UNARY_1D_LOOP_INLINE(tin, tout, expr) \
  NDARRAY_UNARY_1D_LOOP_PREAMBLE {                    \
    const tin in1 = *(tin*)px1;                       \
    tout* out     = (tout*)px2;                       \
    expr;                                             \
  }                                                   \
  NDARRAY_UNARY_1D_LOOP_EPILOGUE

/**
 * Macro for a unary one-dimensional ndarray loop which invokes a callback.
 *
 * ## Notes
 *
 * -   Retrieves each ndarray element according to type `tin` via the pointer
 *     `px1`.
 * -   Explicitly casts each function `f` invocation result to `tout`.
 * -   Stores the result in an output ndarray via the pointer `px2`.
 *
 * @param tin   input type
 * @param tout  output type
 *
 * @example
 * // e.g., d_d
 * NDARRAY_UNARY_1D_LOOP_CLBK(double, double)
 */
#define NDARRAY_UNARY_1D_LOOP_CLBK(tin, tout) \
  NDARRAY_UNARY_1D_LOOP_PREAMBLE {            \
    const tin x = *(tin*)px1;                 \
    *(tout*)px2 = (tout)f(x);                 \
  }                                           \
  NDARRAY_UNARY_1D_LOOP_EPILOGUE

/**
 * Macro for a unary one-dimensional loop which invokes a callback and does not
 * cast the return callback's return value (e.g., a `struct`).
 *
 * ## Notes
 *
 * -   Retrieves each ndarray element according to type `tin` via a pointer
 *     `px1`.
 * -   Stores the result in an output ndarray of type `tout` via the pointer
 *     `px2`.
 *
 * @param tin   input type
 * @param tout  output type
 *
 * @example
 * #include "ndarray/complex/float64.h"
 *
 * // e.g., z_z
 * NDARRAY_UNARY_1D_LOOP_CLBK_RET_NOCAST(
 *     ndarray_complex128_t, ndarray_complex128_t)
 */
#define NDARRAY_UNARY_1D_LOOP_CLBK_RET_NOCAST(tin, tout) \
  NDARRAY_UNARY_1D_LOOP_PREAMBLE {                       \
    const tin x = *(tin*)px1;                            \
    *(tout*)px2 = f(x);                                  \
  }                                                      \
  NDARRAY_UNARY_1D_LOOP_EPILOGUE

/**
 * Macro for a unary one-dimensional ndarray loop which invokes a callback
 * requiring arguments be explicitly cast to a different type.
 *
 * ## Notes
 *
 * -   Retrieves each ndarray element according to type `tin` via the pointer
 *     `px1`.
 * -   Explicitly casts each function argument to `fin`.
 * -   Explicitly casts each function `f` invocation result to `tout`.
 * -   Stores the result in an output ndarray via the pointer `px2`.
 *
 * @param tin   input type
 * @param tout  output type
 * @param fin   callback argument type
 *
 * @example
 * // e.g., f_f_as_d_d
 * NDARRAY_UNARY_1D_LOOP_CLBK_ARG_CAST(float, float, double)
 */
#define NDARRAY_UNARY_1D_LOOP_CLBK_ARG_CAST(tin, tout, fin) \
  NDARRAY_UNARY_1D_LOOP_PREAMBLE {                          \
    const tin x = *(tin*)px1;                               \
    *(tout*)px2 = (tout)f((fin)x);                          \
  }                                                         \
  NDARRAY_UNARY_1D_LOOP_EPILOGUE

/**
 * Macro for a unary one-dimensional ndarray loop which invokes a callback
 * requiring arguments be cast to a different type via casting functions.
 *
 * ## Notes
 *
 * -   Retrieves each ndarray element according to type `tin` via a pointer
 *     `px1`.
 * -   Explicitly casts each function argument via `cin`.
 * -   Explicitly casts each function `f` invocation result via `cout`.
 * -   Stores the result in an output ndarray of type `tout` via the pointer
 *     `px2`.
 *
 * @param tin   input type
 * @param tout  output type
 * @param cin   input casting function
 * @param cout  output casting function
 *
 * @example
 * #include "ndarray/complex/float32.h"
 * #include "ndarray/complex/float64.h"
 *
 * // e.g., f_c_as_z_z
 * NDARRAY_UNARY_1D_LOOP_CLBK_ARG_CAST_FCN(
 *     float, ndarray_complex64_t, ndarray_complex128_from_float32,
 *     ndarray_complex128_to_complex64)
 */
#define NDARRAY_UNARY_1D_LOOP_CLBK_ARG_CAST_FCN(tin, tout, cin, cout) \
  NDARRAY_UNARY_1D_LOOP_PREAMBLE {                                    \
    const tin x = *(tin*)px1;                                         \
    *(tout*)px2 = cout(f(cin(x)));                                    \
  }                                                                   \
  NDARRAY_UNARY_1D_LOOP_EPILOGUE

/**
 * Macro for a unary one-dimensional ndarray loop which invokes a callback whose
 * return value must be cast to a different type via a casting function.
 *
 * ## Notes
 *
 * -   Retrieves each ndarray element according to type `tin` via a pointer
 *     `px1`.
 * -   Explicitly casts each function `f` invocation result via `cout`.
 * -   Stores the result in an output ndarray of type `tout` via the pointer
 *     `px2`.
 *
 * @param tin   input type
 * @param tout  output type
 * @param cout  output casting function
 *
 * @example
 * #include "ndarray/complex/float64.h"
 *
 * // e.g., d_z
 * NDARRAY_UNARY_1D_LOOP_CLBK_RET_CAST_FCN(
 *     double, ndarray_complex128_t, ndarray_complex128_from_float64)
 */
#define NDARRAY_UNARY_1D_LOOP_CLBK_RET_CAST_FCN(tin, tout, cout) \
  NDARRAY_UNARY_1D_LOOP_PREAMBLE {                               \
    const tin x = *(tin*)px1;                                    \
    *(tout*)px2 = cout(f(x));                                    \
  }                                                              \
  NDARRAY_UNARY_1D_LOOP_EPILOGUE

/**
* Macro for operating on elements of a one-dimensional ndarray using a unary
* strided array function.
*
* @param strided_array_fcn   strided array function

* @example
* #include "strided/base/unary.h"
*
* NDARRAY_UNARY_1D_VIA_STRIDED(strided_b_b)
*/
#define NDARRAY_UNARY_1D_VIA_STRIDED(strided_array_fcn)                       \
  struct ndarray* x1 = arrays[0];                                             \
  struct ndarray* x2 = arrays[1];                                             \
  int64_t shape[]    = {ndarray_dimension(x1, 0)};                            \
  int64_t strides[]  = {ndarray_stride(x1, 0), ndarray_stride(x2, 0)};        \
  /* Set pointers to the first indexed element in the ndarray view: */        \
  uint8_t* strided_arrays[] = {                                               \
      ndarray_data(x1) + ndarray_offset(x1),                                  \
      ndarray_data(x2) + ndarray_offset(x2)};                                 \
  /* For negative strides, we need to adjust the pointers to the last indexed \
   * element, as strided implementations expect data pointers to point to     \
   * where the data buffer begins in memory... */                             \
  if (strides[0] < 0) {                                                       \
    strided_arrays[0] += strides[0] * shape[0];                               \
  }                                                                           \
  if (strides[1] < 0) {                                                       \
    strided_arrays[1] += strides[1] * shape[0];                               \
  }                                                                           \
  strided_array_fcn(strided_arrays, shape, strides, fcn);

#endif  // !NDARRAY_BASE_UNARY_MACROS_1D_H
