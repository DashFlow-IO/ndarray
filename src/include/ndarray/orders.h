#ifndef NDARRAY_ORDERS_H
#define NDARRAY_ORDERS_H

/*
 * Enumeration of ndarray orders (i.e., memory layout/iteration order).
 */
enum NDARRAY_ORDER {
  // Row-major (C-style):
  NDARRAY_ROW_MAJOR = 1,

  // Column-major (Fortran-style):
  NDARRAY_COLUMN_MAJOR = 2
};

#endif  // !NDARRAY_ORDERS_H
