/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#include "ndarray.h"
#include <stdint.h>
#include <stdlib.h>
#include "dart_api_dl.h"
#include "ndarray/base/bytes_per_element.h"
#include "ndarray/base/ind.h"
#include "ndarray/base/iteration_order.h"
#include "ndarray/base/minmax_view_buffer_index.h"
#include "ndarray/base/numel.h"
#include "ndarray/base/strides2order.h"
#include "ndarray/complex/float32.h"
#include "ndarray/complex/float64.h"
#include "ndarray/dtypes.h"
#include "ndarray/index_modes.h"
#include "ndarray/orders.h"

////////////////////////////////////////////////////////////////////////////////
// Versioning
#define STRINGIFY_(x) #x
#define STRINGIFY(x)  STRINGIFY_(x)

#define NDARRAY_VERSION 0.0.1

#define NDARRAY_VERSTR STRINGIFY(NDARRAY_VERSION)

const char* VersionString() {
  return NDARRAY_VERSTR;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Initialize `dart_api_dl.h`
intptr_t InitDartApiDL(void* data) {
  return Dart_InitializeApiDL(data);
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Returns a pointer to a dynamically allocated ndarray.
 *
 * ## Notes
 *
 * -   The user is responsible for freeing the allocated memory.
 *
 * @param dtype      data type
 * @param data       pointer to the underlying byte array
 * @param ndims      number of dimensions
 * @param shape      array shape (dimensions)
 * @param strides    array strides (in bytes)
 * @param offset     byte offset specifying the location of the first element
 * @param order      specifies whether an array is row-major (C-style) or
 *                   column-major (Fortran-style)
 * @param imode      specifies how to handle indices which exceed array
 *                   dimensions
 * @param nsubmodes  number of subscript modes
 * @param submodes   specifies how to handle subscripts which exceed array
 *                   dimensions on a per dimension basis (if provided fewer
 *                   submodes than dimensions, submodes are recycled using
 *                   modulo arithmetic)
 * @return           pointer to a dynamically allocated ndarray or, if unable to
 *                   allocate memory, a null pointer
 *
 * @example
 * #include "ndarray/ndarray.h"
 * #include "ndarray/dtypes.h"
 * #include "ndarray/index_modes.h"
 * #include "ndarray/orders.h"
 * #include "ndarray/bytes_per_element.h"
 * #include <stdlib.h>
 * #include <stdio.h>
 * #include <stdint.h>
 *
 * // Specify the underlying data type:
 * enum NDARRAY_DTYPE dtype = NDARRAY_FLOAT64;
 *
 * // Create an underlying byte array:
 * uint8_t buffer[] = {
 *    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
 * };
 *
 * // Specify the number of array dimensions:
 * int64_t ndims = 1;
 *
 * // Specify the array shape:
 * int64_t shape[] = {3}; // vector consisting of 3 doubles
 *
 * // Specify the array strides:
 * int64_t strides[] = {NDARRAY_FLOAT64_BYTES_PER_ELEMENT};
 *
 * // Specify the byte offset:
 * int64_t offset = 0;
 *
 * // Specify the array order (note: this does not matter for a 1-dimensional
 * // array):
 * enum NDARRAY_ORDER order = NDARRAY_ROW_MAJOR;
 *
 * // Specify the index mode:
 * enum NDARRAY_INDEX_MODE imode = NDARRAY_INDEX_ERROR;
 *
 * // Specify the subscript index modes:
 * int8_t submodes[] = {NDARRAY_INDEX_ERROR};
 * int64_t nsubmodes = 1;
 *
 * // Create an ndarray:
 * struct ndarray *x = ndarray_allocate(
 *     dtype, buffer, ndims, shape, strides, offset, order, imode, nsubmodes,
 *     submodes);
 * if (x == NULL) {
 *     fprintf(stderr, "Error allocating memory.\n");
 *     exit(1);
 * }
 *
 * // Free allocated memory:
 * ndarray_free(x);
 */
struct ndarray* ndarray_allocate(
    int16_t dtype, uint8_t* data, int64_t ndims, int64_t* shape,
    int64_t* strides, int64_t offset, int8_t order, int8_t imode,
    int64_t nsubmodes, int8_t* submodes
) {
  int64_t len;

  struct ndarray* arr = malloc(sizeof(struct ndarray));
  if (arr == NULL) {
    return NULL;
  }
  arr->data              = data;
  arr->dtype             = dtype;
  arr->imode             = imode;
  arr->ndims             = ndims;
  arr->nsubmodes         = nsubmodes;
  arr->offset            = offset;
  arr->order             = order;
  arr->shape             = shape;
  arr->strides           = strides;
  arr->submodes          = submodes;

  len                    = ndarray_numel(ndims, shape);
  arr->length            = len;

  arr->BYTES_PER_ELEMENT = ndarray_bytes_per_element(dtype);
  arr->byteLength        = len * (arr->BYTES_PER_ELEMENT);
  arr->flags             = ndarray_flags(arr);

  return arr;
}

/**
 * Returns the size of an ndarray (in bytes).
 *
 * @param arr  input ndarray
 * @return     array size
 */
int64_t ndarray_bytelength(const struct ndarray* arr) {
  return arr->byteLength;
}

/**
 * Returns a pointer to an ndarray's underlying byte array.
 *
 * @param arr  input ndarray
 * @return     underlying byte array
 */
uint8_t* ndarray_data(const struct ndarray* arr) {
  return arr->data;
}

/**
 * Returns an ndarray dimension.
 *
 * ## Notes
 *
 * -   The function does not perform any sanity checks.
 *
 * @param arr  input ndarray
 * @param i    dimension index
 * @return     dimension
 */
int64_t ndarray_dimension(const struct ndarray* arr, const int64_t i) {
  return arr->shape[i];
}

/**
 * Disables specified ndarray flags.
 *
 * ## Notes
 *
 * -   The function does not perform any sanity checks and **assumes** the user
 *     knows what s/he is doing.
 *
 * @param arr    input ndarray
 * @param flags  bit mask to disable flags
 * @return       status code
 */
int8_t ndarray_disable_flags(struct ndarray* arr, const int64_t flags) {
  arr->flags &= ~flags;
  return 0;
}

/**
 * Returns an ndarray data type.
 *
 * @param arr  input ndarray
 * @return     array data type
 */
int16_t ndarray_dtype(const struct ndarray* arr) {
  return arr->dtype;
}

/**
 * Enables specified ndarray flags.
 *
 * ## Notes
 *
 * -   The function does not perform any sanity checks and **assumes** the user
 *     knows what s/he is doing.
 *
 * @param arr    input ndarray
 * @param flags  bit mask to enable flags
 * @return       status code
 */
int8_t ndarray_enable_flags(struct ndarray* arr, const int64_t flags) {
  arr->flags |= flags;
  return 0;
}

/**
 * Returns ndarray flags.
 *
 * @param arr  input ndarray
 * @return     flags
 */
int64_t ndarray_flags(const struct ndarray* arr) {
  uint8_t contiguous;
  int64_t* strides;
  int64_t nbytes;
  int64_t tmp[2];
  int64_t ndims;
  int64_t flags;
  int64_t len;
  int8_t ord;

  // Cache various ndarray data:
  len     = arr->length;
  ndims   = arr->ndims;
  strides = arr->strides;
  nbytes  = arr->BYTES_PER_ELEMENT;

  // Initialize the memory for `flags`:
  flags = 0;

  // Determine if the array can be stored contiguously...
  if (len == 0 || ndarray_iteration_order(ndims, strides) == 0) {
    // If an array does not contain any elements, then no data to store, and, if
    // the array is unordered, adjacent array elements are not guaranteed to be
    // stored next to each other.
    contiguous = 0;
  } else {
    // Ensure that the array is compatible with a single memory segment:
    ndarray_minmax_view_buffer_index(
        ndims, arr->shape, strides, arr->offset, tmp
    );
    if ((len * nbytes) == ((tmp[1] - tmp[0]) + nbytes)) {
      // Compatible:
      contiguous = 1;
    } else {
      // Incompatible:
      contiguous = 0;
    }
  }
  // Determine if the array is row-major/column-major contiguous:
  if (contiguous == 1) {
    // Infer the array "order" from the stride array (this is supplementary to
    // `arr->order`):
    ord = ndarray_strides2order(ndims, strides);

    if (ord == 1 || ord == 3) {
      flags |= NDARRAY_ROW_MAJOR_CONTIGUOUS_FLAG;
    }
    if (ord == 2 || ord == 3) {
      flags |= NDARRAY_COLUMN_MAJOR_CONTIGUOUS_FLAG;
    }
  }
  return flags;
}

/**
 * Frees an ndarray's allocated memory.
 *
 * @param arr  input ndarray
 */
void ndarray_free(struct ndarray* arr) {
  free(arr);
}

/**
 * Tests whether an ndarray has specified flags enabled.
 *
 * @param arr    input ndarray
 * @param flags  bit mask specifying the flags to test against
 * @return       `1` if flags are set and `0` otherwise
 */
int8_t ndarray_has_flags(const struct ndarray* arr, const int64_t flags) {
  if ((arr->flags & flags) == flags) {
    return 1;
  }
  return 0;
}

/**
 * Returns the index mode of an ndarray.
 *
 * @param arr  input ndarray
 * @return     index mode
 */
int8_t ndarray_index_mode(const struct ndarray* arr) {
  return arr->imode;
}

/**
 * Returns the number of elements in an ndarray.
 *
 * @param arr  input ndarray
 * @return     number of elements
 */
int64_t ndarray_length(const struct ndarray* arr) {
  return arr->length;
}

/**
 * Returns the number of ndarray dimensions.
 *
 * @param arr  input ndarray
 * @return     number of dimensions
 */
int64_t ndarray_ndims(const struct ndarray* arr) {
  return arr->ndims;
}

/**
 * Returns the number of ndarray subscript modes.
 *
 * @param arr  input ndarray
 * @return     number of subscript modes
 */
int64_t ndarray_nsubmodes(const struct ndarray* arr) {
  return arr->nsubmodes;
}

/**
 * Returns an ndarray index offset (in bytes).
 *
 * @param arr  input ndarray
 * @return     array strides
 */
int64_t ndarray_offset(const struct ndarray* arr) {
  return arr->offset;
}

/**
 * Returns the order of an ndarray.
 *
 * @param arr  input ndarray
 * @return     array order
 */
int8_t ndarray_order(const struct ndarray* arr) {
  return arr->order;
}

/**
 * Returns a pointer to an array containing an ndarray shape (dimensions).
 *
 * @param arr  input ndarray
 * @return     array shape (dimensions)
 */
int64_t* ndarray_shape(const struct ndarray* arr) {
  return arr->shape;
}

/**
 * Returns an ndarray stride (in bytes).
 *
 * ## Notes
 *
 * -   The function does not perform any sanity checks.
 *
 * @param arr  input ndarray
 * @param i    dimension index
 * @return     array stride
 */
int64_t ndarray_stride(const struct ndarray* arr, const int64_t i) {
  return arr->strides[i];
}

/**
 * Returns a pointer to an array containing ndarray strides (in bytes).
 *
 * @param arr  input ndarray
 * @return     array strides
 */
int64_t* ndarray_strides(const struct ndarray* arr) {
  return arr->strides;
}

/**
 * Returns an ndarray subscript mode.
 *
 * ## Notes
 *
 * -   If an ndarray has fewer subscript modes than dimensions, modes are
 *     recycled using modulo arithmetic.
 * -   The function does not perform any sanity checks.
 *
 * @param arr  input ndarray
 * @param i    dimension index
 * @return     subscript mode
 */
int8_t ndarray_submode(const struct ndarray* arr, const int64_t i) {
  return arr->submodes[i % (arr->nsubmodes)];
}

/**
 * Returns ndarray subscript modes.
 *
 * @param arr  input ndarray
 * @return     subscript modes
 */
int8_t* ndarray_submodes(const struct ndarray* arr) {
  return arr->submodes;
}

/**
 * Returns an ndarray data element.
 *
 * ## Notes
 *
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   The function requires a `void` pointer for the output address `out` in
 *     order to provide a generic API supporting ndarrays having different data
 *     types.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get(const struct ndarray* arr, const int64_t* sub, void* out) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_value(arr, idx, out);
}

/**
 * Returns a double-precision floating-point ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_float64(
    const struct ndarray* arr, const int64_t* sub, double* out
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_float64(idx, out);
}

/**
 * Returns a single-precision floating-point ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_float32(
    const struct ndarray* arr, const int64_t* sub, float* out
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_float32(idx, out);
}

/**
 * Returns an unsigned 64-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_uint64(
    const struct ndarray* arr, const int64_t* sub, uint64_t* out
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_uint64(idx, out);
}

/**
 * Returns a signed 64-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_int64(
    const struct ndarray* arr, const int64_t* sub, int64_t* out
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_int64(idx, out);
}

/**
 * Returns an unsigned 32-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_uint32(
    const struct ndarray* arr, const int64_t* sub, uint32_t* out
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_uint32(idx, out);
}

/**
 * Returns a signed 32-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_int32(
    const struct ndarray* arr, const int64_t* sub, int32_t* out
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_int32(idx, out);
}

/**
 * Returns an unsigned 16-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_uint16(
    const struct ndarray* arr, const int64_t* sub, uint16_t* out
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_uint16(idx, out);
}

/**
 * Returns a signed 16-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_int16(
    const struct ndarray* arr, const int64_t* sub, int16_t* out
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_int16(idx, out);
}

/**
 * Returns an unsigned 8-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_uint8(
    const struct ndarray* arr, const int64_t* sub, uint8_t* out
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_uint8(idx, out);
}

/**
 * Returns a signed 8-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_int8(
    const struct ndarray* arr, const int64_t* sub, int8_t* out
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_int8(idx, out);
}

/**
 * Returns a double-precision complex floating-point ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_complex128(
    const struct ndarray* arr, const int64_t* sub, ndarray_complex128_t* out
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_complex128(idx, out);
}

/**
 * Returns a single-precision complex floating-point ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_complex64(
    const struct ndarray* arr, const int64_t* sub, ndarray_complex64_t* out
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_complex64(idx, out);
}

/**
 * Returns a boolean ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_bool(
    const struct ndarray* arr, const int64_t* sub, bool* out
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_get_ptr_bool(idx, out);
}

/**
 * Returns a pointer to an ndarray data element in the underlying byte array.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @return     underlying byte array pointer
 */
uint8_t* ndarray_get_ptr(const struct ndarray* arr, const int64_t* sub) {
  int8_t* submodes;
  int64_t* strides;
  int64_t* shape;
  int64_t ndims;
  uint8_t* idx;
  int64_t ind;
  int64_t M;
  int64_t i;

  ndims    = arr->ndims;
  shape    = arr->shape;
  strides  = arr->strides;
  submodes = arr->submodes;
  M        = arr->nsubmodes;

  idx      = (arr->data) + (arr->offset);  // pointer arithmetic
  for (i = 0; i < ndims; i++) {
    ind = ndarray_ind(sub[i], shape[i] - 1, submodes[i % M]);
    if (ind < 0) {
      return NULL;
    }
    idx += strides[i] * ind;  // pointer arithmetic
  }
  return idx;
}

/**
 * Returns an ndarray data element specified by a byte array pointer.
 *
 * ## Notes
 *
 * -   The function does **not** perform bounds checking and **assumes** you
 *     know what you are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   The function requires a `void` pointer for the output address `out` in
 *     order to provide a generic API supporting ndarrays having different data
 *     types.
 *
 * @param arr  input ndarray
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_value(
    const struct ndarray* arr, const uint8_t* idx, void* out
) {
  switch (arr->dtype) {
    case NDARRAY_FLOAT64:
      *(double*)out = *(double*)idx;
      return 0;
    case NDARRAY_FLOAT32:
      *(float*)out = *(float*)idx;
      return 0;
    case NDARRAY_UINT64:
      *(uint64_t*)out = *(uint64_t*)idx;
      return 0;
    case NDARRAY_INT64:
      *(int64_t*)out = *(int64_t*)idx;
      return 0;
    case NDARRAY_UINT32:
      *(uint32_t*)out = *(uint32_t*)idx;
      return 0;
    case NDARRAY_INT32:
      *(int32_t*)out = *(int32_t*)idx;
      return 0;
    case NDARRAY_UINT16:
      *(uint16_t*)out = *(uint16_t*)idx;
      return 0;
    case NDARRAY_INT16:
      *(int16_t*)out = *(int16_t*)idx;
      return 0;
    case NDARRAY_UINT8:
      *(uint8_t*)out = *(uint8_t*)idx;
      return 0;
    case NDARRAY_UINT8C:
      *(uint8_t*)out = *(uint8_t*)idx;
      return 0;
    case NDARRAY_INT8:
      *(int8_t*)out = *(int8_t*)idx;
      return 0;
    case NDARRAY_COMPLEX128:
      *(ndarray_complex128_t*)out = *(ndarray_complex128_t*)idx;
      return 0;
    case NDARRAY_COMPLEX64:
      *(ndarray_complex64_t*)out = *(ndarray_complex64_t*)idx;
      return 0;
    case NDARRAY_BOOL:
      *(bool*)out = *(bool*)idx;
      return 0;
    default:
      return -1;
  }
}

/**
 * Returns a double-precision floating-point ndarray data element specified by a
 * byte array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, accessing **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_float64(const uint8_t* idx, double* out) {
  *out = *(double*)idx;
  return 0;
}

/**
 * Returns a single-precision floating-point ndarray data element specified by a
 * byte array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, accessing **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_float32(const uint8_t* idx, float* out) {
  *out = *(float*)idx;
  return 0;
}

/**
 * Returns an unsigned 64-bit integer ndarray data element specified by a byte
 * array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, accessing **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_uint64(const uint8_t* idx, uint64_t* out) {
  *out = *(uint64_t*)idx;
  return 0;
}

/**
 * Returns a signed 64-bit integer ndarray data element specified by a byte
 * array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, accessing **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_int64(const uint8_t* idx, int64_t* out) {
  *out = *(int64_t*)idx;
  return 0;
}

/**
 * Returns an unsigned 32-bit integer ndarray data element specified by a byte
 * array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, accessing **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_uint32(const uint8_t* idx, uint32_t* out) {
  *out = *(uint32_t*)idx;
  return 0;
}

/**
 * Returns a signed 32-bit integer ndarray data element specified by a byte
 * array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, accessing **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_int32(const uint8_t* idx, int32_t* out) {
  *out = *(int32_t*)idx;
  return 0;
}

/**
 * Returns an unsigned 16-bit integer ndarray data element specified by a byte
 * array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, accessing **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_uint16(const uint8_t* idx, uint16_t* out) {
  *out = *(uint16_t*)idx;
  return 0;
}

/**
 * Returns a signed 16-bit integer ndarray data element specified by a byte
 * array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, accessing **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_int16(const uint8_t* idx, int16_t* out) {
  *out = *(int16_t*)idx;
  return 0;
}

/**
 * Returns an unsigned 8-bit integer ndarray data element specified by a byte
 * array pointer.
 *
 * ## Notes
 *
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_uint8(const uint8_t* idx, uint8_t* out) {
  *out = *(uint8_t*)idx;
  return 0;
}

/**
 * Returns a signed 8-bit integer ndarray data element specified by a byte array
 * pointer.
 *
 * ## Notes
 *
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_int8(const uint8_t* idx, int8_t* out) {
  *out = *(int8_t*)idx;
  return 0;
}

/**
 * Returns a double-precision complex floating-point ndarray data element
 * specified by a byte array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, accessing **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_complex128(
    const uint8_t* idx, ndarray_complex128_t* out
) {
  *out = *(ndarray_complex128_t*)idx;
  return 0;
}

/**
 * Returns a single-precision complex floating-point ndarray data element
 * specified by a byte array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, accessing **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_complex64(const uint8_t* idx, ndarray_complex64_t* out) {
  *out = *(ndarray_complex64_t*)idx;
  return 0;
}

/**
 * Returns a boolean ndarray data element specified by a byte array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, accessing **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_get_ptr_bool(const uint8_t* idx, bool* out) {
  *out = *(bool*)idx;
  return 0;
}

/**
 * Returns an ndarray data element located at a specified linear index.
 *
 * ## Notes
 *
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   The function requires a `void` pointer for the output address `out` in
 *     order to provide a generic API supporting ndarrays having different data
 *     types.
 * -   The function places the burden on the user to ensure that the output
 *     address is compatible with the data type of input ndarray data elements.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget(const struct ndarray* arr, const int64_t idx, void* out) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_value(arr, ptr, out);
}

/**
 * Returns a double-precision floating-point ndarray data element located at a
 * specified linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget_float64(
    const struct ndarray* arr, const int64_t idx, double* out
) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_float64(ptr, out);
}

/**
 * Returns a single-precision floating-point ndarray data element located at a
 * specified linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget_float32(
    const struct ndarray* arr, const int64_t idx, float* out
) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_float32(ptr, out);
}

/**
 * Returns an unsigned 64-bit integer ndarray data element located at a
 * specified linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget_uint64(
    const struct ndarray* arr, const int64_t idx, uint64_t* out
) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_uint64(ptr, out);
}

/**
 * Returns a signed 64-bit integer ndarray data element located at a specified
 * linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget_int64(
    const struct ndarray* arr, const int64_t idx, int64_t* out
) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_int64(ptr, out);
}

/**
 * Returns an unsigned 32-bit integer ndarray data element located at a
 * specified linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget_uint32(
    const struct ndarray* arr, const int64_t idx, uint32_t* out
) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_uint32(ptr, out);
}

/**
 * Returns a signed 32-bit integer ndarray data element located at a specified
 * linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget_int32(
    const struct ndarray* arr, const int64_t idx, int32_t* out
) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_int32(ptr, out);
}

/**
 * Returns an unsigned 16-bit integer ndarray data element located at a
 * specified linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget_uint16(
    const struct ndarray* arr, const int64_t idx, uint16_t* out
) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_uint16(ptr, out);
}

/**
 * Returns a signed 16-bit integer ndarray data element located at a specified
 * linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget_int16(
    const struct ndarray* arr, const int64_t idx, int16_t* out
) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_int16(ptr, out);
}

/**
 * Returns an unsigned 8-bit integer ndarray data element located at a specified
 * linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget_uint8(
    const struct ndarray* arr, const int64_t idx, uint8_t* out
) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_uint8(ptr, out);
}

/**
 * Returns a signed 8-bit integer ndarray data element located at a specified
 * linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget_int8(
    const struct ndarray* arr, const int64_t idx, int8_t* out
) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_int8(ptr, out);
}

/**
 * Returns a double-precision complex floating-point ndarray data element
 * located at a specified linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget_complex128(
    const struct ndarray* arr, const int64_t idx, ndarray_complex128_t* out
) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_complex128(ptr, out);
}

/**
 * Returns a single-precision complex floating-point ndarray data element
 * located at a specified linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget_complex64(
    const struct ndarray* arr, const int64_t idx, ndarray_complex64_t* out
) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_complex64(ptr, out);
}

/**
 * Returns a boolean ndarray data element located at a specified linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the output address type matches the
 *     underlying input ndarray data type and **assumes** that you know what you
 *     are doing.
 * -   The function returns `-1` if unable to get an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function returns the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param out  output address
 * @return     status code
 */
int8_t ndarray_iget_bool(
    const struct ndarray* arr, const int64_t idx, bool* out
) {
  uint8_t* ptr = ndarray_iget_ptr(arr, idx);
  if (ptr == NULL) {
    return -1;
  }
  return ndarray_get_ptr_bool(ptr, out);
}

/**
 * Returns a pointer in the underlying byte array for an ndarray data element
 * located at a specified linear index.
 *
 * ## Notes
 *
 * -   For zero-dimensional arrays, the function returns a pointer to the first
 *     (and only) indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @return     underlying byte array pointer
 */
uint8_t* ndarray_iget_ptr(const struct ndarray* arr, const int64_t idx) {
  int64_t* strides;
  int64_t* shape;
  int64_t ndims;
  uint8_t* ind;
  int64_t s;
  int64_t i;
  int64_t j;
  int8_t io;

  // Retrieve the number of dimensions
  ndims = arr->ndims;

  // For zero-dimensional ndarrays, we ignore the index argument and return a
  // pointer to the first (and only) indexed element...
  if (ndims == 0) {
    return (arr->data) + (arr->offset);  // pointer arithmetic
  }
  // Copy index argument to a mutable variable:
  j = idx;

  // Resolve an ndarray index based on the ndarray index mode:
  j = ndarray_ind(j, (arr->length) - 1, arr->imode);
  if (j < 0) {
    return NULL;
  }
  // Determine the pointer to the first indexed element:
  ind = (arr->data) + (arr->offset);  // pointer arithmetic

  // Determine the iteration order based on the ndarray strides:
  strides = arr->strides;
  io      = ndarray_iteration_order(ndims, strides);

  // Check for trivial case...
  if ((arr->flags) & (NDARRAY_ROW_MAJOR_CONTIGUOUS_FLAG |
                      NDARRAY_COLUMN_MAJOR_CONTIGUOUS_FLAG)) {
    // Trivial case where we have all positive strides...
    if (io == 1) {
      return ind + (j * (arr->BYTES_PER_ELEMENT));  // pointer arithmetic
    }
    // Trivial case where we have all negative strides...
    if (io == -1) {
      return ind - (j * (arr->BYTES_PER_ELEMENT));  // pointer arithmetic
    }
  }
  // The approach which follows is to resolve a view index to its subscripts and
  // then plug the subscripts into the standard formula for computing the linear
  // index in the underlying byte array...
  shape = arr->shape;
  if ((arr->order) == NDARRAY_COLUMN_MAJOR) {
    for (i = 0; i < ndims; i++) {
      s = j % shape[i];
      j -= s;
      j /= shape[i];
      ind += s * strides[i];  // pointer arithmetic
    }
    return ind;
  }
  // Case: row-major
  for (i = ndims - 1; i >= 0; i--) {
    s = j % shape[i];
    j -= s;
    j /= shape[i];
    ind += s * strides[i];  // pointer arithmetic
  }
  return ind;
}

/**
 * Sets an ndarray data element.
 *
 * ## Notes
 *
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   The function requires a pointer to a data value `v` in order to provide a
 *     generic API supporting ndarrays having different data types.
 * -   The function has no way of determining whether `v` actually points to a
 *     memory address compatible with the underlying input ndarray data type.
 *     Accordingly, accessing **unowned** memory is possible, and this function
 *     **assumes** you know what you are doing.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set(
    const struct ndarray* arr, const int64_t* sub, const void* v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_value(arr, idx, v);
}

/**
 * Sets a double-precision floating-point ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_float64(
    const struct ndarray* arr, const int64_t* sub, const double v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_float64(idx, v);
}

/**
 * Sets a single-precision floating-point ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_float32(
    const struct ndarray* arr, const int64_t* sub, const float v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_float32(idx, v);
}

/**
 * Sets an unsigned 64-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_uint64(
    const struct ndarray* arr, const int64_t* sub, const uint64_t v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_uint64(idx, v);
}

/**
 * Sets a signed 64-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_int64(
    const struct ndarray* arr, const int64_t* sub, const int64_t v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_int64(idx, v);
}

/**
 * Sets an unsigned 32-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_uint32(
    const struct ndarray* arr, const int64_t* sub, const uint32_t v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_uint32(idx, v);
}

/**
 * Sets a signed 32-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_int32(
    const struct ndarray* arr, const int64_t* sub, const int32_t v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_int32(idx, v);
}

/**
 * Sets an unsigned 16-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_uint16(
    const struct ndarray* arr, const int64_t* sub, const uint16_t v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_uint16(idx, v);
}

/**
 * Sets a signed 16-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_int16(
    const struct ndarray* arr, const int64_t* sub, const int16_t v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_int16(idx, v);
}

/**
 * Sets an unsigned 8-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_uint8(
    const struct ndarray* arr, const int64_t* sub, const uint8_t v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_uint8(idx, v);
}

/**
 * Sets a signed 8-bit integer ndarray data element.
 *
 * ## Notes
 *
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_int8(
    const struct ndarray* arr, const int64_t* sub, const int8_t v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_int8(idx, v);
}

/**
 * Sets a double-precision complex floating-point ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_complex128(
    const struct ndarray* arr, const int64_t* sub, const ndarray_complex128_t v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_complex128(idx, v);
}

/**
 * Sets a single-precision complex floating-point ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_complex64(
    const struct ndarray* arr, const int64_t* sub, const ndarray_complex64_t v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_complex64(idx, v);
}

/**
 * Sets a boolean floating-point ndarray data element.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 *
 * @param arr  input ndarray
 * @param sub  ndarray subscripts
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_bool(
    const struct ndarray* arr, const int64_t* sub, const bool v
) {
  uint8_t* idx = ndarray_get_ptr(arr, sub);
  if (idx == NULL) {
    return -1;
  }
  return ndarray_set_ptr_bool(idx, v);
}

// ---

/**
 * Sets an ndarray data element specified by a byte array pointer.
 *
 * ## Notes
 *
 * -   The function does **not** perform bounds checking, and, thus, the
 *     function does **not** prevent you from overwriting **unowned** memory.
 *     Accordingly, the function **assumes** you know what you are doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   The function requires a pointer to a data value `v` in order to provide a
 *     generic API supporting ndarrays having different data types.
 *
 * @param arr  input ndarray
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_value(
    const struct ndarray* arr, uint8_t* idx, const void* v
) {
  switch (arr->dtype) {
    case NDARRAY_FLOAT64:
      *(double*)idx = *(double*)v;
      return 0;
    case NDARRAY_FLOAT32:
      *(float*)idx = *(float*)v;
      return 0;
    case NDARRAY_UINT64:
      *(uint64_t*)idx = *(uint64_t*)v;
      return 0;
    case NDARRAY_INT64:
      *(int64_t*)idx = *(int64_t*)v;
      return 0;
    case NDARRAY_UINT32:
      *(uint32_t*)idx = *(uint32_t*)v;
      return 0;
    case NDARRAY_INT32:
      *(int32_t*)idx = *(int32_t*)v;
      return 0;
    case NDARRAY_UINT16:
      *(uint16_t*)idx = *(uint16_t*)v;
      return 0;
    case NDARRAY_INT16:
      *(int16_t*)idx = *(int16_t*)v;
      return 0;
    case NDARRAY_UINT8:
      *(uint8_t*)idx = *(uint8_t*)v;
      return 0;
    case NDARRAY_UINT8C:
      *(uint8_t*)idx = *(uint8_t*)v;
      return 0;
    case NDARRAY_INT8:
      *(int8_t*)idx = *(int8_t*)v;
      return 0;
    case NDARRAY_COMPLEX128:
      *(ndarray_complex128_t*)idx = *(ndarray_complex128_t*)v;
      return 0;
    case NDARRAY_COMPLEX64:
      *(ndarray_complex64_t*)idx = *(ndarray_complex64_t*)v;
      return 0;
    case NDARRAY_BOOL:
      *(bool*)idx = *(bool*)v;
      return 0;
    default:
      return -1;
  }
}

/**
 * Sets a double-precision floating-point ndarray data element specified by a
 * byte array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, overwriting **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_float64(uint8_t* idx, const double v) {
  *(double*)idx = v;
  return 0;
}

/**
 * Sets a single-precision floating-point ndarray data element specified by a
 * byte array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, overwriting **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_float32(uint8_t* idx, const float v) {
  *(float*)idx = v;
  return 0;
}

/**
 * Sets an unsigned 64-bit integer ndarray data element specified by a byte
 * array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, overwriting **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_uint64(uint8_t* idx, const uint64_t v) {
  *(uint64_t*)idx = v;
  return 0;
}

/**
 * Sets a signed 64-bit integer ndarray data element specified by a byte array
 * pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, overwriting **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_int64(uint8_t* idx, const int64_t v) {
  *(int64_t*)idx = v;
  return 0;
}

/**
 * Sets an unsigned 32-bit integer ndarray data element specified by a byte
 * array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, overwriting **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_uint32(uint8_t* idx, const uint32_t v) {
  *(uint32_t*)idx = v;
  return 0;
}

/**
 * Sets a signed 32-bit integer ndarray data element specified by a byte array
 * pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, overwriting **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_int32(uint8_t* idx, const int32_t v) {
  *(int32_t*)idx = v;
  return 0;
}

/**
 * Sets an unsigned 16-bit integer ndarray data element specified by a byte
 * array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, overwriting **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_uint16(uint8_t* idx, const uint16_t v) {
  *(uint16_t*)idx = v;
  return 0;
}

/**
 * Sets a signed 16-bit integer ndarray data element specified by a byte array
 * pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, overwriting **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_int16(uint8_t* idx, const int16_t v) {
  *(int16_t*)idx = v;
  return 0;
}

/**
 * Sets an unsigned 8-bit integer ndarray data element specified by a byte array
 * pointer.
 *
 * ## Notes
 *
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_uint8(uint8_t* idx, const uint8_t v) {
  *(uint8_t*)idx = v;
  return 0;
}

/**
 * Sets a signed 8-bit integer ndarray data element specified by a byte array
 * pointer.
 *
 * ## Notes
 *
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_int8(uint8_t* idx, const int8_t v) {
  *(int8_t*)idx = v;
  return 0;
}

/**
 * Sets a double-precision complex floating-point ndarray data element specified
 * by a byte array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, overwriting **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_complex128(uint8_t* idx, const ndarray_complex128_t v) {
  *(ndarray_complex128_t*)idx = v;
  return 0;
}

/**
 * Sets a single-precision complex floating-point ndarray data element specified
 * by a byte array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, overwriting **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_complex64(uint8_t* idx, const ndarray_complex64_t v) {
  *(ndarray_complex64_t*)idx = v;
  return 0;
}

/**
 * Sets a boolean ndarray data element specified by a byte array pointer.
 *
 * ## Notes
 *
 * -   The function has no way of determining whether `idx` actually points to a
 *     compatible memory address. Accordingly, overwriting **unowned** memory is
 *     possible, and this function **assumes** you know what you are doing.
 * -   The function always returns `0`.
 *
 * @param idx  byte array pointer to an ndarray data element
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_set_ptr_bool(uint8_t* idx, const bool v) {
  *(bool*)idx = v;
  return 0;
}

/**
 * Sets an ndarray data element located at a specified linear index.
 *
 * ## Notes
 *
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   The function requires a pointer to a data value `v` in order to provide a
 *     generic API supporting ndarrays having different data types.
 * -   The function has no way of determining whether `v` actually points to a
 *     memory address compatible with the underlying input ndarray data type.
 *     Accordingly, accessing **unowned** memory is possible, and this function
 *     **assumes** you know what you are doing.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset(
    const struct ndarray* arr, const int64_t idx, const void* v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_value(arr, ind, v);
}

/**
 * Sets a double-precision floating-point ndarray data element located at a
 * specified linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset_float64(
    const struct ndarray* arr, const int64_t idx, const double v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_float64(ind, v);
}

/**
 * Sets a single-precision floating-point ndarray data element located at a
 * specified linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset_float32(
    const struct ndarray* arr, const int64_t idx, const float v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_float32(ind, v);
}

/**
 * Sets an unsigned 64-bit integer ndarray data element located at a specified
 * linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset_uint64(
    const struct ndarray* arr, const int64_t idx, const uint64_t v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_uint64(ind, v);
}

/**
 * Sets a signed 64-bit integer ndarray data element located at a specified
 * linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset_int64(
    const struct ndarray* arr, const int64_t idx, const int64_t v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_int64(ind, v);
}

/**
 * Sets an unsigned 32-bit integer ndarray data element located at a specified
 * linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset_uint32(
    const struct ndarray* arr, const int64_t idx, const uint32_t v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_uint32(ind, v);
}

/**
 * Sets a signed 32-bit integer ndarray data element located at a specified
 * linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset_int32(
    const struct ndarray* arr, const int64_t idx, const int32_t v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_int32(ind, v);
}

/**
 * Sets an unsigned 16-bit integer ndarray data element located at a specified
 * linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset_uint16(
    const struct ndarray* arr, const int64_t idx, const uint16_t v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_uint16(ind, v);
}

/**
 * Sets a signed 16-bit integer ndarray data element located at a specified
 * linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset_int16(
    const struct ndarray* arr, const int64_t idx, const int16_t v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_int16(ind, v);
}

/**
 * Sets an unsigned 8-bit integer ndarray data element located at a specified
 * linear index.
 *
 * ## Notes
 *
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset_uint8(
    const struct ndarray* arr, const int64_t idx, const uint8_t v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_uint8(ind, v);
}

/**
 * Sets a signed 8-bit integer ndarray data element located at a specified
 * linear index.
 *
 * ## Notes
 *
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset_int8(
    const struct ndarray* arr, const int64_t idx, const int8_t v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_int8(ind, v);
}

/**
 * Sets a double-precision complex floating-point ndarray data element located
 * at a specified linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset_complex128(
    const struct ndarray* arr, const int64_t idx, const ndarray_complex128_t v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_complex128(ind, v);
}

/**
 * Sets a single-precision complex floating-point ndarray data element located
 * at a specified linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset_complex64(
    const struct ndarray* arr, const int64_t idx, const ndarray_complex64_t v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_complex64(ind, v);
}

/**
 * Sets a boolean ndarray data element located at a specified linear index.
 *
 * ## Notes
 *
 * -   The function does **not** verify that the type of `v` matches the
 *     underlying input ndarray data type, and, thus, overwriting **unowned**
 *     memory is possible. The function **assumes** that you know what you are
 *     doing.
 * -   The function returns `-1` if unable to set an element and `0` otherwise.
 * -   For zero-dimensional arrays, the function sets the first (and only)
 *     indexed element, regardless of the value of `idx`.
 *
 * @param arr  input ndarray
 * @param idx  linear view index
 * @param v    value to set
 * @return     status code
 */
int8_t ndarray_iset_bool(
    const struct ndarray* arr, const int64_t idx, const bool v
) {
  uint8_t* ind = ndarray_iget_ptr(arr, idx);
  if (ind == NULL) {
    return -1;
  }
  return ndarray_set_ptr_bool(ind, v);
}