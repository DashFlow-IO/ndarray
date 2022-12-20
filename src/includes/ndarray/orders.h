/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_ORDERS_H
#define NDARRAY_ORDERS_H

/**
 * Enumeration of ndarray orders (i.e., memory layout/iteration order).
 */
enum NDARRAY_ORDER {
  // Row-major (C-style):
  NDARRAY_ROW_MAJOR = 1,

  // Column-major (Fortran-style):
  NDARRAY_COLUMN_MAJOR = 2
};

#endif  // !NDARRAY_ORDERS_H
