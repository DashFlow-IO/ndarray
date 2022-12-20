/*
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_H
#define NDARRAY_H

#include "ndarray/dtypes.h"
#include "ndarray/index_modes.h"
#include "ndarray/orders.h"
#include "ndarray/bytes_per_element.h"
#include <stdint.h>

 /**
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

#endif // !NDARRAY_H