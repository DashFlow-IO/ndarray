/*
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_H
#define NDARRAY_H

#include <stdbool.h>
#include <stdint.h>
#include "ndarray/bytes_per_element.h"
#include "ndarray/complex/float32.h"
#include "ndarray/complex/float64.h"
#include "ndarray/dtypes.h"
#include "ndarray/index_modes.h"
#include "ndarray/macros.h"
#include "ndarray/orders.h"

/*
 * ndarray structure.
 *
 * @example
 * #include "ndarray.h"
 * #include "ndarray/dtypes.h"
 * #include "ndarray/index_modes.h"
 * #include "ndarray/orders.h"
 * #include "ndarray/base/bytes_per_element.h"
 * #include <stdlib.h>
 * #include <stdio.h>
 * #include <stdint.h>
 *
 * struct ndarray *x = malloc(sizeof(struct ndarray));
 * if (x == NULL) {
 *     fprintf(stderr, "Error allocating memory.\n");
 *     exit(1);
 * }
 *
 * // Create an underlying byte array:
 * uint8_t buffer[] = {
 *     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
 * };
 * x->data = buffer;
 *
 * // Specify the underlying data type:
 * x->dtype = NDARRAY_FLOAT64;
 *
 * // Explicitly specify the number of bytes per element:
 * x->BYTES_PER_ELEMENT = NDARRAY_FLOAT64_BYTES_PER_ELEMENT;
 *
 * // Specify the array shape:
 * int64_t shape[] = {3}; // vector consisting of 3 doubles
 * x->shape = shape;
 *
 * // Specify the array strides:
 * int64_t strides[] = {x->BYTES_PER_ELEMENT};
 * x->strides = strides;
 *
 * // Specify the byte offset:
 * x->offset = 0;
 *
 * // Specify the array order (note: this does not matter for a 1-dimensional
 * // array):
 * x->order = NDARRAY_ROW_MAJOR;
 *
 * // Specify the index mode:
 * x->imode = NDARRAY_INDEX_ERROR;
 *
 * // Specify subscript index modes:
 * x->submodes = {NDARRAY_INDEX_ERROR};
 * x->nsubmodes = 1;
 *
 * // Explicitly specify the number of array dimensions:
 * x->ndims = 1; // vector
 *
 * // Explicitly specify the number of array elements (doubles):
 * x->length = x->shape[0];
 *
 * // Explicitly specify the number of bytes:
 * x->byteLength = (x->length) * (x->BYTES_PER_ELEMENT);
 *
 * // Explicitly set the array flags:
 * x->flags = ndarray_flags(x);
 *
 * // Free allocated memory:
 * ndarray_free(x);
 */
struct ndarray {
  // Underlying data type:
  int16_t dtype;

  // Pointer to the underlying byte array:
  uint8_t* data;

  // Number of array dimensions:
  int64_t ndims;

  // Array shape (dimensions):
  int64_t* shape;

  // Array strides (in bytes) specifying how to iterate over a strided array:
  int64_t* strides;

  // Byte offset which specifies the location at which to start iterating
  // over array elements:
  int64_t offset;

  // Array order (either row-major (C-style) or column-major (Fortran-style)):
  int8_t order;

  // Mode specifying how to handle indices which exceed array dimensions:
  int8_t imode;

  // Number of subscript modes:
  int64_t nsubmodes;

  // Mode(s) specifying how to handle subscripts which exceed array dimensions
  // on a per dimension basis :
  int8_t* submodes;

  // Number of array elements:
  int64_t length;

  // Size in bytes:
  int64_t byteLength;

  // Number of bytes per element (i.e., item size):
  int64_t BYTES_PER_ELEMENT;

  // Bit mask providing information regarding the memory layout of the array
  // (e.g., see macros):
  int64_t flags;
};

// ****************************************************************************
//                            properties interface
// ****************************************************************************

/*
 * Returns a pointer to a dynamically allocated ndarray.
 */
struct ndarray* ndarray_allocate(
    int16_t dtype, uint8_t* data, int64_t ndims, int64_t* shape,
    int64_t* strides, int64_t offset, int8_t order, int8_t imode,
    int64_t nsubmodes, int8_t* submodes
);

/*
 * Returns the size of an ndarray (in bytes).
 */
int64_t ndarray_bytelength(const struct ndarray* arr);

/*
 * Returns a pointer to an ndarray's underlying byte array.
 */
uint8_t* ndarray_data(const struct ndarray* arr);

/*
 * Returns an ndarray dimension.
 */
int64_t ndarray_dimension(const struct ndarray* arr, const int64_t i);

/*
 * Disables specified ndarray flags.
 */
int8_t ndarray_disable_flags(struct ndarray* arr, const int64_t flags);

/*
 * Returns an ndarray data type.
 */
int16_t ndarray_dtype(const struct ndarray* arr);

/*
 * Enables specified ndarray flags.
 */
int8_t ndarray_enable_flags(struct ndarray* arr, const int64_t flags);

/*
 * Returns ndarray flags.
 */
int64_t ndarray_flags(const struct ndarray* arr);

/*
 * Frees an ndarray's allocated memory.
 */
void ndarray_free(struct ndarray* arr);

/*
 * Tests whether an ndarray has specified flags enabled.
 */
int8_t ndarray_has_flags(const struct ndarray* arr, const int64_t flags);

/*
 * Returns the index mode of an ndarray.
 */
int8_t ndarray_index_mode(const struct ndarray* arr);

/*
 * Returns the number of elements in an ndarray.
 */
int64_t ndarray_length(const struct ndarray* arr);

/*
 * Returns the number of ndarray dimensions.
 */
int64_t ndarray_ndims(const struct ndarray* arr);

/*
 * Returns an ndarray index offset (in bytes).
 */
int64_t ndarray_offset(const struct ndarray* arr);

/*
 * Returns the order of an ndarray.
 */
int8_t ndarray_order(const struct ndarray* arr);

/*
 * Returns the number of ndarray subscript modes.
 */
int64_t ndarray_nsubmodes(const struct ndarray* arr);

/*
 * Returns a pointer to an array containing an ndarray shape (dimensions).
 */
int64_t* ndarray_shape(const struct ndarray* arr);

/*
 * Returns an ndarray stride (in bytes).
 */
int64_t ndarray_stride(const struct ndarray* arr, const int64_t i);

/*
 * Returns a pointer to an array containing ndarray strides (in bytes).
 */
int64_t* ndarray_strides(const struct ndarray* arr);

/*
 * Returns ndarray subscript modes.
 */
int8_t* ndarray_submodes(const struct ndarray* arr);

/*
 * Returns an ndarray subscript mode.
 */
int8_t ndarray_submode(const struct ndarray* arr, const int64_t i);

// ****************************************************************************
//                                get interface
// ****************************************************************************

/**
 * Returns an ndarray data element.
 */
int8_t ndarray_get(const struct ndarray* arr, const int64_t* sub, void* out);

/**
 * Returns a double-precision floating-point ndarray data element.
 */
int8_t ndarray_get_float64(
    const struct ndarray* arr, const int64_t* sub, double* out
);

/**
 * Returns a single-precision floating-point ndarray data element.
 */
int8_t ndarray_get_float32(
    const struct ndarray* arr, const int64_t* sub, float* out
);

/**
 * Returns an unsigned 64-bit integer ndarray data element.
 */
int8_t ndarray_get_uint64(
    const struct ndarray* arr, const int64_t* sub, uint64_t* out
);

/**
 * Returns a signed 64-bit integer ndarray data element.
 */
int8_t ndarray_get_int64(
    const struct ndarray* arr, const int64_t* sub, int64_t* out
);

/**
 * Returns an unsigned 32-bit integer ndarray data element.
 */
int8_t ndarray_get_uint32(
    const struct ndarray* arr, const int64_t* sub, uint32_t* out
);

/**
 * Returns a signed 32-bit integer ndarray data element.
 */
int8_t ndarray_get_int32(
    const struct ndarray* arr, const int64_t* sub, int32_t* out
);

/**
 * Returns an unsigned 16-bit integer ndarray data element.
 */
int8_t ndarray_get_uint16(
    const struct ndarray* arr, const int64_t* sub, uint16_t* out
);

/**
 * Returns a signed 16-bit integer ndarray data element.
 */
int8_t ndarray_get_int16(
    const struct ndarray* arr, const int64_t* sub, int16_t* out
);

/**
 * Returns an unsigned 8-bit integer ndarray data element.
 */
int8_t ndarray_get_uint8(
    const struct ndarray* arr, const int64_t* sub, uint8_t* out
);

/**
 * Returns a signed 8-bit integer ndarray data element.
 */
int8_t ndarray_get_int8(
    const struct ndarray* arr, const int64_t* sub, int8_t* out
);

/**
 * Returns a double-precision complex floating-point ndarray data element.
 */
int8_t ndarray_get_complex128(
    const struct ndarray* arr, const int64_t* sub, ndarray_complex128_t* out
);

/**
 * Returns a single-precision complex floating-point ndarray data element.
 */
int8_t ndarray_get_complex64(
    const struct ndarray* arr, const int64_t* sub, ndarray_complex64_t* out
);

/**
 * Returns a boolean ndarray data element.
 */
int8_t ndarray_get_bool(
    const struct ndarray* arr, const int64_t* sub, bool* out
);

// ****************************************************************************
//                             get pointer interface
// ****************************************************************************

/**
 * Returns a pointer to an ndarray data element in the underlying byte array.
 */
uint8_t* ndarray_get_ptr(const struct ndarray* arr, const int64_t* sub);

/**
 * Returns an ndarray data element specified by a byte array pointer.
 */
int8_t ndarray_get_ptr_value(
    const struct ndarray* arr, const uint8_t* idx, void* out
);

/**
 * Returns a double-precision floating-point ndarray data element specified by a
 * byte array pointer.
 */
int8_t ndarray_get_ptr_float64(const uint8_t* idx, double* out);

/**
 * Returns a single-precision floating-point ndarray data element specified by a
 * byte array pointer.
 */
int8_t ndarray_get_ptr_float32(const uint8_t* idx, float* out);

/**
 * Returns an unsigned 64-bit integer ndarray data element specified by a byte
 * array pointer.
 */
int8_t ndarray_get_ptr_uint64(const uint8_t* idx, uint64_t* out);

/**
 * Returns a signed 64-bit integer ndarray data element specified by a byte
 * array pointer.
 */
int8_t ndarray_get_ptr_int64(const uint8_t* idx, int64_t* out);

/**
 * Returns an unsigned 32-bit integer ndarray data element specified by a byte
 * array pointer.
 */
int8_t ndarray_get_ptr_uint32(const uint8_t* idx, uint32_t* out);

/**
 * Returns a signed 32-bit integer ndarray data element specified by a byte
 * array pointer.
 */
int8_t ndarray_get_ptr_int32(const uint8_t* idx, int32_t* out);

/**
 * Returns an unsigned 16-bit integer ndarray data element specified by a byte
 * array pointer.
 */
int8_t ndarray_get_ptr_uint16(const uint8_t* idx, uint16_t* out);

/**
 * Returns a signed 16-bit integer ndarray data element specified by a byte
 * array pointer.
 */
int8_t ndarray_get_ptr_int16(const uint8_t* idx, int16_t* out);

/**
 * Returns an unsigned 8-bit integer ndarray data element specified by a byte
 * array pointer.
 */
int8_t ndarray_get_ptr_uint8(const uint8_t* idx, uint8_t* out);

/**
 * Returns a signed 8-bit integer ndarray data element specified by a byte array
 * pointer.
 */
int8_t ndarray_get_ptr_int8(const uint8_t* idx, int8_t* out);

/**
 * Returns a double-precision complex floating-point ndarray data element
 * specified by a byte array pointer.
 */
int8_t ndarray_get_ptr_complex128(
    const uint8_t* idx, ndarray_complex128_t* out
);

/**
 * Returns a single-precision complex floating-point ndarray data element
 * specified by a byte array pointer.
 */
int8_t ndarray_get_ptr_complex64(const uint8_t* idx, ndarray_complex64_t* out);

/**
 * Returns a boolean ndarray data element specified by a byte array pointer.
 */
int8_t ndarray_get_ptr_bool(const uint8_t* idx, bool* out);

// ****************************************************************************
//                           get at index interface
// ****************************************************************************

/**
 * Returns an ndarray data element located at a specified linear index.
 */
int8_t ndarray_iget(const struct ndarray* arr, const int64_t idx, void* out);

/**
 * Returns a double-precision floating-point ndarray data element located at a
 * specified linear index.
 */
int8_t ndarray_iget_float64(
    const struct ndarray* arr, const int64_t idx, double* out
);

/**
 * Returns a single-precision floating-point ndarray data element located at a
 * specified linear index.
 */
int8_t ndarray_iget_float32(
    const struct ndarray* arr, const int64_t idx, float* out
);

/**
 * Returns an unsigned 64-bit integer ndarray data element located at a
 * specified linear index.
 */
int8_t ndarray_iget_uint64(
    const struct ndarray* arr, const int64_t idx, uint64_t* out
);

/**
 * Returns a signed 64-bit integer ndarray data element located at a specified
 * linear index.
 */
int8_t ndarray_iget_int64(
    const struct ndarray* arr, const int64_t idx, int64_t* out
);

/**
 * Returns an unsigned 32-bit integer ndarray data element located at a
 * specified linear index.
 */
int8_t ndarray_iget_uint32(
    const struct ndarray* arr, const int64_t idx, uint32_t* out
);

/**
 * Returns a signed 32-bit integer ndarray data element located at a specified
 * linear index.
 */
int8_t ndarray_iget_int32(
    const struct ndarray* arr, const int64_t idx, int32_t* out
);

/**
 * Returns an unsigned 16-bit integer ndarray data element located at a
 * specified linear index.
 */
int8_t ndarray_iget_uint16(
    const struct ndarray* arr, const int64_t idx, uint16_t* out
);

/**
 * Returns a signed 16-bit integer ndarray data element located at a specified
 * linear index.
 */
int8_t ndarray_iget_int16(
    const struct ndarray* arr, const int64_t idx, int16_t* out
);

/**
 * Returns an unsigned 8-bit integer ndarray data element located at a specified
 * linear index.
 */
int8_t ndarray_iget_uint8(
    const struct ndarray* arr, const int64_t idx, uint8_t* out
);

/**
 * Returns a signed 8-bit integer ndarray data element located at a specified
 * linear index.
 */
int8_t ndarray_iget_int8(
    const struct ndarray* arr, const int64_t idx, int8_t* out
);

/**
 * Returns a double-precision complex floating-point ndarray data element
 * located at a specified linear index.
 */
int8_t ndarray_iget_complex128(
    const struct ndarray* arr, const int64_t idx, ndarray_complex128_t* out
);

/**
 * Returns a single-precision complex floating-point ndarray data element
 * located at a specified linear index.
 */
int8_t ndarray_iget_complex64(
    const struct ndarray* arr, const int64_t idx, ndarray_complex64_t* out
);

/**
 * Returns a boolean ndarray data element located at a specified linear index.
 */
int8_t ndarray_iget_bool(
    const struct ndarray* arr, const int64_t idx, bool* out
);

// ****************************************************************************
//                       get pointer at index interface
// ****************************************************************************

/**
 * Returns a pointer in the underlying byte array for an ndarray data element
 * located at a specified linear index.
 */
uint8_t* ndarray_iget_ptr(const struct ndarray* arr, const int64_t idx);

// ****************************************************************************
//                           set interface
// ****************************************************************************

/**
 * Sets an ndarray data element.
 */
int8_t ndarray_set(
    const struct ndarray* arr, const int64_t* sub, const void* v
);

/**
 * Sets a double-precision floating-point ndarray data element.
 */
int8_t ndarray_set_float64(
    const struct ndarray* arr, const int64_t* sub, const double v
);

/**
 * Sets a single-precision floating-point ndarray data element.
 */
int8_t ndarray_set_float32(
    const struct ndarray* arr, const int64_t* sub, const float v
);

/**
 * Sets an unsigned 64-bit integer ndarray data element.
 */
int8_t ndarray_set_uint64(
    const struct ndarray* arr, const int64_t* sub, const uint64_t v
);

/**
 * Sets a signed 64-bit integer ndarray data element.
 */
int8_t ndarray_set_int64(
    const struct ndarray* arr, const int64_t* sub, const int64_t v
);

/**
 * Sets an unsigned 32-bit integer ndarray data element.
 */
int8_t ndarray_set_uint32(
    const struct ndarray* arr, const int64_t* sub, const uint32_t v
);

/**
 * Sets a signed 32-bit integer ndarray data element.
 */
int8_t ndarray_set_int32(
    const struct ndarray* arr, const int64_t* sub, const int32_t v
);

/**
 * Sets an unsigned 16-bit integer ndarray data element.
 */
int8_t ndarray_set_uint16(
    const struct ndarray* arr, const int64_t* sub, const uint16_t v
);

/**
 * Sets a signed 16-bit integer ndarray data element.
 */
int8_t ndarray_set_int16(
    const struct ndarray* arr, const int64_t* sub, const int16_t v
);

/**
 * Sets an unsigned 8-bit integer ndarray data element.
 */
int8_t ndarray_set_uint8(
    const struct ndarray* arr, const int64_t* sub, const uint8_t v
);

/**
 * Sets a signed 8-bit integer ndarray data element.
 */
int8_t ndarray_set_int8(
    const struct ndarray* arr, const int64_t* sub, const int8_t v
);

/**
 * Sets a double-precision complex floating-point ndarray data element.
 */
int8_t ndarray_set_complex128(
    const struct ndarray* arr, const int64_t* sub, const ndarray_complex128_t v
);

/**
 * Sets a single-precision complex floating-point ndarray data element.
 */
int8_t ndarray_set_complex64(
    const struct ndarray* arr, const int64_t* sub, const ndarray_complex64_t v
);

/**
 * Sets a boolean ndarray data element.
 */
int8_t ndarray_set_bool(
    const struct ndarray* arr, const int64_t* sub, const bool v
);

// ****************************************************************************
//                           set pointer interface
// ****************************************************************************

/**
 * Sets an ndarray data element specified by a byte array pointer.
 */
int8_t ndarray_set_ptr_value(
    const struct ndarray* arr, uint8_t* idx, const void* v
);

/**
 * Sets a double-precision floating-point ndarray data element specified by a
 * byte array pointer.
 */
int8_t ndarray_set_ptr_float64(uint8_t* idx, const double v);

/**
 * Sets a single-precision floating-point ndarray data element specified by a
 * byte array pointer.
 */
int8_t ndarray_set_ptr_float32(uint8_t* idx, const float v);

/**
 * Sets an unsigned 64-bit integer ndarray data element specified by a byte
 * array pointer.
 */
int8_t ndarray_set_ptr_uint64(uint8_t* idx, const uint64_t v);

/**
 * Sets a signed 64-bit integer ndarray data element specified by a byte array
 * pointer.
 */
int8_t ndarray_set_ptr_int64(uint8_t* idx, const int64_t v);

/**
 * Sets an unsigned 32-bit integer ndarray data element specified by a byte
 * array pointer.
 */
int8_t ndarray_set_ptr_uint32(uint8_t* idx, const uint32_t v);

/**
 * Sets a signed 32-bit integer ndarray data element specified by a byte array
 * pointer.
 */
int8_t ndarray_set_ptr_int32(uint8_t* idx, const int32_t v);

/**
 * Sets an unsigned 16-bit integer ndarray data element specified by a byte
 * array pointer.
 */
int8_t ndarray_set_ptr_uint16(uint8_t* idx, const uint16_t v);

/**
 * Sets a signed 16-bit integer ndarray data element specified by a byte array
 * pointer.
 */
int8_t ndarray_set_ptr_int16(uint8_t* idx, const int16_t v);

/**
 * Sets an unsigned 8-bit integer ndarray data element specified by a byte array
 * pointer.
 */
int8_t ndarray_set_ptr_uint8(uint8_t* idx, const uint8_t v);

/**
 * Sets a signed 8-bit integer ndarray data element specified by a byte array
 * pointer.
 */
int8_t ndarray_set_ptr_int8(uint8_t* idx, const int8_t v);

/**
 * Sets a double-precision complex floating-point ndarray data element specified
 * by a byte array pointer.
 */
int8_t ndarray_set_ptr_complex128(uint8_t* idx, const ndarray_complex128_t v);

/**
 * Sets a single-precision complex floating-point ndarray data element specified
 * by a byte array pointer.
 */
int8_t ndarray_set_ptr_complex64(uint8_t* idx, const ndarray_complex64_t v);

/**
 * Sets a boolean ndarray data element specified by a byte array pointer.
 */
int8_t ndarray_set_ptr_bool(uint8_t* idx, const bool v);

// ****************************************************************************
//                           set at index interface
// ****************************************************************************

/**
 * Sets an ndarray data element located at a specified linear index.
 */
int8_t ndarray_iset(
    const struct ndarray* arr, const int64_t idx, const void* v
);

/**
 * Sets a double-precision floating-point ndarray data element located at a
 * specified linear index.
 */
int8_t ndarray_iset_float64(
    const struct ndarray* arr, const int64_t idx, const double v
);

/**
 * Sets a single-precision floating-point ndarray data element located at a
 * specified linear index.
 */
int8_t ndarray_iset_float32(
    const struct ndarray* arr, const int64_t idx, const float v
);

/**
 * Sets an unsigned 64-bit integer ndarray data element located at a specified
 * linear index.
 */
int8_t ndarray_iset_uint64(
    const struct ndarray* arr, const int64_t idx, const uint64_t v
);

/**
 * Sets a signed 64-bit integer ndarray data element located at a specified
 * linear index.
 */
int8_t ndarray_iset_int64(
    const struct ndarray* arr, const int64_t idx, const int64_t v
);

/**
 * Sets an unsigned 32-bit integer ndarray data element located at a specified
 * linear index.
 */
int8_t ndarray_iset_uint32(
    const struct ndarray* arr, const int64_t idx, const uint32_t v
);

/**
 * Sets a signed 32-bit integer ndarray data element located at a specified
 * linear index.
 */
int8_t ndarray_iset_int32(
    const struct ndarray* arr, const int64_t idx, const int32_t v
);

/**
 * Sets an unsigned 16-bit integer ndarray data element located at a specified
 * linear index.
 */
int8_t ndarray_iset_uint16(
    const struct ndarray* arr, const int64_t idx, const uint16_t v
);

/**
 * Sets a signed 16-bit integer ndarray data element located at a specified
 * linear index.
 */
int8_t ndarray_iset_int16(
    const struct ndarray* arr, const int64_t idx, const int16_t v
);

/**
 * Sets an unsigned 8-bit integer ndarray data element located at a specified
 * linear index.
 */
int8_t ndarray_iset_uint8(
    const struct ndarray* arr, const int64_t idx, const uint8_t v
);

/**
 * Sets a signed 8-bit integer ndarray data element located at a specified
 * linear index.
 */
int8_t ndarray_iset_int8(
    const struct ndarray* arr, const int64_t idx, const int8_t v
);

/**
 * Sets a double-precision complex floating-point ndarray data element located
 * at a specified linear index.
 */
int8_t ndarray_iset_complex128(
    const struct ndarray* arr, const int64_t idx, const ndarray_complex128_t v
);

/**
 * Sets a single-precision complex floating-point ndarray data element located
 * at a specified linear index.
 */
int8_t ndarray_iset_complex64(
    const struct ndarray* arr, const int64_t idx, const ndarray_complex64_t v
);

/**
 * Sets a boolean ndarray data element located at a specified linear index.
 */
int8_t ndarray_iset_bool(
    const struct ndarray* arr, const int64_t idx, const bool v
);

#endif  // !NDARRAY_H
