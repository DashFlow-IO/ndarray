/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_UNARY_MACROS_2D_BLOCKED_H
#define NDARRAY_BASE_UNARY_MACROS_2D_BLOCKED_H

#include <stdint.h>
#include <string.h>
#include "ndarray.h"
#include "ndarray/base/bytes_per_element.h"
#include "ndarray/base/unary/internal/permute.h"
#include "ndarray/base/unary/internal/range.h"
#include "ndarray/base/unary/internal/sort2ins.h"
#include "ndarray/base/unary/macros/constants.h"

/**
 * Macro containing the preamble for blocked nested loops which operate on
 * elements of a two-dimensional ndarray.
 *
 * ## Notes
 *
 * -   Variable naming conventions:
 *
 *     -   `sx#`, `pbx#`, `px#`, `ox#`, `nbx#`, and `d@x#` where `#` corresponds
 *         to the ndarray argument number, starting at `1`.
 *     -   `S@`, `i@`, `j@`, `o@x#`, and `d@x#` where `@` corresponds to the
 *         loop number, with `0` being the innermost loop.
 *
 * @example
 * NDARRAY_UNARY_2D_BLOCKED_LOOP_PREMABLE {
 *     // Innermost loop body...
 * }
 * NDARRAY_UNARY_2D_BLOCKED_LOOP_EPILOGUE
 */
#define NDARRAY_UNARY_2D_BLOCKED_LOOP_PREAMBLE                                 \
  struct ndarray* x1 = arrays[0];                                              \
  struct ndarray* x2 = arrays[1];                                              \
  int64_t shape[2];                                                            \
  int64_t sx1[2];                                                              \
  int64_t sx2[2];                                                              \
  int64_t idx[2];                                                              \
  int64_t tmp[2];                                                              \
  int64_t bsize;                                                               \
  uint8_t* pbx1;                                                               \
  uint8_t* pbx2;                                                               \
  uint8_t* px1;                                                                \
  uint8_t* px2;                                                                \
  int64_t d0x1;                                                                \
  int64_t d1x1;                                                                \
  int64_t d0x2;                                                                \
  int64_t d1x2;                                                                \
  int64_t o1x1;                                                                \
  int64_t o1x2;                                                                \
  int64_t nbx1;                                                                \
  int64_t nbx2;                                                                \
  int64_t ox1;                                                                 \
  int64_t ox2;                                                                 \
  int64_t s0;                                                                  \
  int64_t s1;                                                                  \
  int64_t i0;                                                                  \
  int64_t i1;                                                                  \
  int64_t j0;                                                                  \
  int64_t j1;                                                                  \
  /* Copy strides to prevent mutation to the original ndarray: */              \
  memcpy(sx1, ndarray_strides(x1), sizeof sx1);                                \
  /* Create a loop interchange index array for loop order permutation: */      \
  ndarray_base_unary_internal_range(2, idx);                                   \
  /* Sort the input array strides in increasing order (of magnitude): */       \
  ndarray_base_unary_internal_sort2ins(2, sx1, idx);                           \
  /* Permute the shape and array strides (avoiding mutation) according to loop \
   * order: */                                                                 \
  ndarray_base_unary_internal_permute(2, ndarray_shape(x1), idx, tmp);         \
  memcpy(shape, tmp, sizeof shape);                                            \
  ndarray_base_unary_internal_permute(2, ndarray_strides(x2), idx, tmp);       \
  memcpy(sx2, tmp, sizeof sx2);                                                \
  /* Determine the block size... */                                            \
  nbx1 = ndarray_bytes_per_element(ndarray_dtype(x1));                         \
  nbx2 = ndarray_bytes_per_element(ndarray_dtype(x2));                         \
  if (nbx1 == 0 && nbx2 == 0) {                                                \
    bsize = NDARRAY_UNARY_BLOCK_SIZE_IN_ELEMENTS;                              \
  } else if (nbx1 > nbx2) {                                                    \
    bsize = NDARRAY_UNARY_BLOCK_SIZE_IN_BYTES / nbx1;                          \
  } else {                                                                     \
    bsize = NDARRAY_UNARY_BLOCK_SIZE_IN_BYTES / nbx2;                          \
  }                                                                            \
  /* Cache pointers to the ndarray buffers... */                               \
  pbx1 = ndarray_data(x1);                                                     \
  pbx2 = ndarray_data(x2);                                                     \
  /* Cache byte offsets to the first indexed elements... */                    \
  ox1 = ndarray_offset(x1);                                                    \
  ox2 = ndarray_offset(x2);                                                    \
  /* Cache offset increments for the innermost loop... */                      \
  d0x1 = sx1[0];                                                               \
  d0x2 = sx2[0];                                                               \
  /* Iterate over blocks... */                                                 \
  for (j1 = shape[1]; j1 > 0;) {                                               \
    if (j1 < bsize) {                                                          \
      s1 = j1;                                                                 \
      j1 = 0;                                                                  \
    } else {                                                                   \
      s1 = bsize;                                                              \
      j1 -= bsize;                                                             \
    }                                                                          \
    o1x1 = ox1 + (j1 * sx1[1]);                                                \
    o1x2 = ox2 + (j1 * sx2[1]);                                                \
    for (j0 = shape[0]; j0 > 0;) {                                             \
      if (j0 < bsize) {                                                        \
        s0 = j0;                                                               \
        j0 = 0;                                                                \
      } else {                                                                 \
        s0 = bsize;                                                            \
        j0 -= bsize;                                                           \
      }                                                                        \
      /* Compute pointers to the first ndarray elements in the current         \
       * block... */                                                           \
      px1 = pbx1 + o1x1 + (j0 * sx1[0]);                                       \
      px2 = pbx2 + o1x2 + (j0 * sx2[0]);                                       \
      /* Compute loop offset increments... */                                  \
      d1x1 = sx1[1] - (s0 * sx1[0]);                                           \
      d1x2 = sx2[1] - (s0 * sx2[0]);                                           \
      /* Iterate over the ndarray dimensions... */                             \
      for (i1 = 0; i1 < s1; i1++, px1 += d1x1, px2 += d1x2) {                  \
        for (i0 = 0; i0 < s0; i0++, px1 += d0x1, px2 += d0x2)

/**
 * Macro containing the preamble for blocked nested loops which operate on
 * elements of a two-dimensional input ndarray and updates two output ndarrays.
 *
 * ## Notes
 *
 * -   Variable naming conventions:
 *
 *     -   `sx#`, `pbx#`, `px#`, `ox#`, `nbx#`, and `d@x#` where `#` corresponds
 *         to the ndarray argument number, starting at `1`.
 *     -   `S@`, `i@`, `j@`, `o@x#`, and `d@x#` where `@` corresponds to the
 *         loop number, with `0` being the innermost loop.
 *
 * @example
 * NDARRAY_UNARY_2D_BLOCKED_LOOP_TWO_OUT_PREMABLE {
 *     // Innermost loop body...
 * }
 * NDARRAY_UNARY_2D_BLOCKED_LOOP_EPILOGUE
 */
#define NDARRAY_UNARY_2D_BLOCKED_LOOP_TWO_OUT_PREAMBLE                         \
  struct ndarray* x1 = arrays[0];                                              \
  struct ndarray* x2 = arrays[1];                                              \
  struct ndarray* x3 = arrays[2];                                              \
  int64_t shape[2];                                                            \
  int64_t sx1[2];                                                              \
  int64_t sx2[2];                                                              \
  int64_t sx3[2];                                                              \
  int64_t idx[2];                                                              \
  int64_t tmp[2];                                                              \
  int64_t bsize;                                                               \
  uint8_t* pbx1;                                                               \
  uint8_t* pbx2;                                                               \
  uint8_t* pbx3;                                                               \
  uint8_t* px1;                                                                \
  uint8_t* px2;                                                                \
  uint8_t* px3;                                                                \
  int64_t d0x1;                                                                \
  int64_t d1x1;                                                                \
  int64_t d0x2;                                                                \
  int64_t d1x2;                                                                \
  int64_t d0x3;                                                                \
  int64_t d1x3;                                                                \
  int64_t o1x1;                                                                \
  int64_t o1x2;                                                                \
  int64_t o1x3;                                                                \
  int64_t nbx1;                                                                \
  int64_t nbx2;                                                                \
  int64_t nbx3;                                                                \
  int64_t ox1;                                                                 \
  int64_t ox2;                                                                 \
  int64_t ox3;                                                                 \
  int64_t s0;                                                                  \
  int64_t s1;                                                                  \
  int64_t i0;                                                                  \
  int64_t i1;                                                                  \
  int64_t j0;                                                                  \
  int64_t j1;                                                                  \
  /* Copy strides to prevent mutation to the original ndarray: */              \
  memcpy(sx1, ndarray_strides(x1), sizeof sx1);                                \
  /* Create a loop interchange index array for loop order permutation: */      \
  ndarray_base_unary_internal_range(2, idx);                                   \
  /* Sort the input array strides in increasing order (of magnitude): */       \
  ndarray_base_unary_internal_sort2ins(2, sx1, idx);                           \
  /* Permute the shape and array strides (avoiding mutation) according to loop \
   * order: */                                                                 \
  ndarray_base_unary_internal_permute(2, ndarray_shape(x1), idx, tmp);         \
  memcpy(shape, tmp, sizeof shape);                                            \
  ndarray_base_unary_internal_permute(2, ndarray_strides(x2), idx, tmp);       \
  memcpy(sx2, tmp, sizeof sx2);                                                \
  ndarray_base_unary_internal_permute(2, ndarray_strides(x3), idx, tmp);       \
  memcpy(sx3, tmp, sizeof sx3);                                                \
  /* Determine the block size... */                                            \
  nbx1 = ndarray_bytes_per_element(ndarray_dtype(x1));                         \
  nbx2 = ndarray_bytes_per_element(ndarray_dtype(x2));                         \
  nbx3 = ndarray_bytes_per_element(ndarray_dtype(x3));                         \
  if (nbx1 == 0 && nbx2 == 0 && nbx3 == 0) {                                   \
    bsize = NDARRAY_UNARY_BLOCK_SIZE_IN_ELEMENTS;                              \
  } else if (nbx1 > nbx2 && nbx1 > nbx3) {                                     \
    bsize = NDARRAY_UNARY_BLOCK_SIZE_IN_BYTES / nbx1;                          \
  } else if (nbx2 > nbx3) {                                                    \
    bsize = NDARRAY_UNARY_BLOCK_SIZE_IN_BYTES / nbx2;                          \
  } else {                                                                     \
    bsize = NDARRAY_UNARY_BLOCK_SIZE_IN_BYTES / nbx3;                          \
  }                                                                            \
  /* Cache pointers to the ndarray buffers... */                               \
  pbx1 = ndarray_data(x1);                                                     \
  pbx2 = ndarray_data(x2);                                                     \
  pbx3 = ndarray_data(x3);                                                     \
  /* Cache byte offsets to the first indexed elements... */                    \
  ox1 = ndarray_offset(x1);                                                    \
  ox2 = ndarray_offset(x2);                                                    \
  ox3 = ndarray_offset(x3);                                                    \
  /* Cache offset increments for the innermost loop ... */                     \
  d0x1 = sx1[0];                                                               \
  d0x2 = sx2[0];                                                               \
  d0x3 = sx3[0];                                                               \
  /* Iterate over blocks... */                                                 \
  for (j1 = shape[1]; j1 > 0;) {                                               \
    if (j1 < bsize) {                                                          \
      s1 = j1;                                                                 \
      j1 = 0;                                                                  \
    } else {                                                                   \
      s1 = bsize;                                                              \
      j1 -= bsize;                                                             \
    }                                                                          \
    o1x1 = ox1 + (j1 * sx1[1]);                                                \
    o1x2 = ox2 + (j1 * sx2[1]);                                                \
    o1x3 = ox3 + (j1 * sx3[1]);                                                \
    for (j0 = shape[0]; j0 > 0;) {                                             \
      if (j0 < bsize) {                                                        \
        s0 = j0;                                                               \
        j0 = 0;                                                                \
      } else {                                                                 \
        s0 = bsize;                                                            \
        j0 -= bsize;                                                           \
      }                                                                        \
      /* Compute pointers to the first ndarray elements in the current         \
       * block... */                                                           \
      px1 = pbx1 + o1x1 + (j0 * sx1[0]);                                       \
      px2 = pbx2 + o1x2 + (j0 * sx2[0]);                                       \
      px3 = pbx3 + o1x3 + (j0 * sx3[0]);                                       \
      /* Compute loop offset increments... */                                  \
      d1x1 = sx1[1] - (s0 * sx1[0]);                                           \
      d1x2 = sx2[1] - (s0 * sx2[0]);                                           \
      d1x3 = sx3[1] - (s0 * sx3[0]);                                           \
      /* Iterate over the ndarray dimensions... */                             \
      for (i1 = 0; i1 < s1; i1++, px1 += d1x1, px2 += d1x2, px3 += d1x3) {     \
        for (i0 = 0; i0 < s0; i0++, px1 += d0x1, px2 += d0x2, px3 += d0x3)

/**
 * Macro containing the epilogue for blocked nested loops which operate on
 * elements of a two-dimensional ndarray.
 *
 * @example
 * NDARRAY_UNARY_2D_BLOCKED_LOOP_PREMABLE {
 *     // Innermost loop body...
 * }
 * NDARRAY_UNARY_2D_BLOCKED_LOOP_EPILOGUE
 */
#define NDARRAY_UNARY_2D_BLOCKED_LOOP_EPILOGUE \
  }                                            \
  }                                            \
  }

/**
 * Macro for a blocked unary two-dimensional ndarray loop which inlines an
 * expression.
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
 * NDARRAY_UNARY_2D_BLOCKED_LOOP_INLINE(double, double, *out = in1 * in1)
 */
#define NDARRAY_UNARY_2D_BLOCKED_LOOP_INLINE(tin, tout, expr) \
  NDARRAY_UNARY_2D_BLOCKED_LOOP_PREAMBLE {                    \
    const tin in1 = *(tin*)px1;                               \
    tout* out     = (tout*)px2;                               \
    expr;                                                     \
  }                                                           \
  NDARRAY_UNARY_2D_BLOCKED_LOOP_EPILOGUE

/**
 * Macro for a blocked unary two-dimensional ndarray loop which invokes a
 * callback.
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
 * NDARRAY_UNARY_2D_BLOCKED_LOOP_CLBK(double, double)
 */
#define NDARRAY_UNARY_2D_BLOCKED_LOOP_CLBK(tin, tout) \
  NDARRAY_UNARY_2D_BLOCKED_LOOP_PREAMBLE {            \
    const tin x = *(tin*)px1;                         \
    *(tout*)px2 = (tout)f(x);                         \
  }                                                   \
  NDARRAY_UNARY_2D_BLOCKED_LOOP_EPILOGUE

/**
 * Macro for a blocked unary two-dimensional loop which invokes a callback and
 * does not cast the return callback's return value (e.g., a `struct`).
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
 * NDARRAY_UNARY_2D_BLOCKED_LOOP_CLBK_RET_NOCAST(
 *     ndarray_complex128_t, ndarray_complex128_t)
 */
#define NDARRAY_UNARY_2D_BLOCKED_LOOP_CLBK_RET_NOCAST(tin, tout) \
  NDARRAY_UNARY_2D_BLOCKED_LOOP_PREAMBLE {                       \
    const tin x = *(tin*)px1;                                    \
    *(tout*)px2 = f(x);                                          \
  }                                                              \
  NDARRAY_UNARY_2D_BLOCKED_LOOP_EPILOGUE

/**
 * Macro for a blocked unary two-dimensional ndarray loop which invokes a
 * callback requiring arguments be explicitly cast to a different type.
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
 * NDARRAY_UNARY_2D_BLOCKED_LOOP_CLBK_ARG_CAST(float, float, double)
 */
#define NDARRAY_UNARY_2D_BLOCKED_LOOP_CLBK_ARG_CAST(tin, tout, fin) \
  NDARRAY_UNARY_2D_BLOCKED_LOOP_PREAMBLE {                          \
    const tin x = *(tin*)px1;                                       \
    *(tout*)px2 = (tout)f((fin)x);                                  \
  }                                                                 \
  NDARRAY_UNARY_2D_BLOCKED_LOOP_EPILOGUE

/**
 * Macro for a blocked unary two-dimensional ndarray loop which invokes a
 * callback requiring arguments be cast to a different type via casting
 * functions.
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
 * NDARRAY_UNARY_2D_BLOCKED_LOOP_CLBK_ARG_CAST_FCN(
 *     float, ndarray_complex64_t, ndarray_complex128_from_float32,
 *     ndarray_complex128_to_complex64)
 */
#define NDARRAY_UNARY_2D_BLOCKED_LOOP_CLBK_ARG_CAST_FCN(tin, tout, cin, cout) \
  NDARRAY_UNARY_2D_BLOCKED_LOOP_PREAMBLE {                                    \
    const tin x = *(tin*)px1;                                                 \
    *(tout*)px2 = cout(f(cin(x)));                                            \
  }                                                                           \
  NDARRAY_UNARY_2D_BLOCKED_LOOP_EPILOGUE

/**
 * Macro for a blocked unary two-dimensional ndarray loop which invokes a
 * callback whose return value must be cast to a different type via a casting
 * function.
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
 * NDARRAY_UNARY_2D_BLOCKED_LOOP_CLBK_RET_CAST_FCN(
 *      double, ndarray_complex128_t, ndarray_complex128_from_float64)
 */
#define NDARRAY_UNARY_2D_BLOCKED_LOOP_CLBK_RET_CAST_FCN(tin, tout, cout) \
  NDARRAY_UNARY_2D_BLOCKED_LOOP_PREAMBLE {                               \
    const tin x = *(tin*)px1;                                            \
    *(tout*)px2 = cout(f(x));                                            \
  }                                                                      \
  NDARRAY_UNARY_2D_BLOCKED_LOOP_EPILOGUE

#endif  // !NDARRAY_BASE_UNARY_MACROS_2D_BLOCKED_H
