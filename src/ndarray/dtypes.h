/* 
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_DTYPES_H
#define NDARRAY_DTYPES_H

/*
* If C++, prevent name mangling so that the compiler emits a binary file
* having undecorated names, thus mirroring the behavior of a C compiler.
*/
#ifdef __cplusplus
extern "C" {
#endif

	/**
	* Enumeration of underlying ndarray data types.
	*/
	enum NDARRAY_DTYPE {
		// Boolean data types:
		NDARRAY_BOOL = 0,

		// Integer data types:
		NDARRAY_INT8,
		NDARRAY_UINT8,
		NDARRAY_UINT8C,
		NDARRAY_INT16,
		NDARRAY_UINT16,
		NDARRAY_INT32,
		NDARRAY_UINT32,
		NDARRAY_INT64,
		NDARRAY_UINT64,
		NDARRAY_INT128,
		NDARRAY_UINT128,
		NDARRAY_INT256,
		NDARRAY_UINT256,

		// Floating-point data types:
		NDARRAY_FLOAT16,
		NDARRAY_BFLOAT16,
		NDARRAY_FLOAT32,
		NDARRAY_FLOAT64,
		NDARRAY_FLOAT128,

		// Complex floating-point number data types:
		NDARRAY_COMPLEX64,
		NDARRAY_COMPLEX128,

		// Define a data type for "binary" data:
		NDARRAY_BINARY,

		// Define a data type for "generic" Dart values (objects):
		NDARRAY_GENERIC,

		// "Compute" the number of data types (this works because of how `enum` 
		// works: the value is automatically set to the last enumerated type plus 1):
		NDARRAY_NDTYPES,

		// Reserve a signaling value which is guaranteed not to be a valid type 
		// enumeration number:
		NDARRAY_NOTYPE,

		// Indicate the start of user defined type numbers (leaving room for type 
		// growth above):
		NDARRAY_USERDEFINED_TYPE = 256,
	};

#ifdef __cplusplus
}
#endif

#endif // !NDARRAY_DTYPES_H
