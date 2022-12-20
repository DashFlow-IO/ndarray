/*
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_MACROS_H
#define NDARRAY_MACROS_H

/**
 * Flag indicating whether an ndarray is row-major (C-style) contiguous.
 *
 * ## Notes
 *
 * -   Row-major order indicates that the last ndarray index varies the fastest.
 * -   Contiguous means that an ndarray is compatible with being stored in a
 *     single memory segment and that ndarray elements are adjacent to each
 *     other in memory.
 * -   `strides` array is in reverse order to that of column-major order.
 * -   An ndarray can be both row-major and column-major contiguous (e.g., if an
 *     ndarray is one-dimensional).
 */
#define NDARRAY_ROW_MAJOR_CONTIGUOUS_FLAG 0x0000000000000001

/**
 * Flag indicating whether an ndarray is column-major (Fortran-style)
 * contiguous.
 *
 * ## Notes
 *
 * -   Column-major order indicates that the first ndarray index varies the
 *     fastest.
 * -   Contiguous means that an ndarray is compatible with being stored in a
 *     single memory segment and that ndarray elements are adjacent to each
 *     other in memory.
 * -   `strides` array is in reverse order to that of row-major order.
 * -   An ndarray can be both row-major and column-major contiguous (e.g., if an
 *     ndarray is one-dimensional).
 */
#define NDARRAY_COLUMN_MAJOR_CONTIGUOUS_FLAG 0x0000000000000002

#endif  // !NDARRAY_MACROS_H