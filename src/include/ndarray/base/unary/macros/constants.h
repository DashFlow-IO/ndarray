/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_UNARY_MACROS_CONSTANTS_H
#define NDARRAY_BASE_UNARY_MACROS_CONSTANTS_H

// Define a default block size in units of bytes (Note: 64b is a common cache
// line size. How applicable the common cache line size is here is debatable,
// given that, depending on the associated stride(s), the innermost loop may not
// iterate over adjacent elements. The primary goal is to have a block size in
// which all data within a block can always fit in (L1) cache, regardless of
// cache size (i.e., cache-oblivious). For reference, a common L1 cache size is
// 32kB per core. For best performance, block sizes should be tuned based on
// system hardware; however, such tuning is not readily available to us here.
// Without obvious better alternatives, 64b has some theoretical (and practical)
// underpinning, and it should be good enough for most inputs, especially for
// ndarrays with near contiguity.):
#define NDARRAY_UNARY_BLOCK_SIZE_IN_BYTES 64

// Define a default block size in units of elements (Note: 64 bytes / 8 bytes
// per element; i.e., default element size is same as a double):
#define NDARRAY_UNARY_BLOCK_SIZE_IN_ELEMENTS 8

#endif  // !NDARRAY_BASE_UNARY_MACROS_CONSTANTS_H
