/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_BASE_MAX_VIEW_BUFFER_INDEX_H
#define NDARRAY_BASE_MAX_VIEW_BUFFER_INDEX_H

#include <stdint.h>

/*
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Computes the maximum linear index (in bytes) in an underlying data buffer
 * accessible to an array view.
 */
int64_t ndarray_max_view_buffer_index(
    int64_t ndims, int64_t* shape, int64_t* strides, int64_t offset
);

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_BASE_MAX_VIEW_BUFFER_INDEX_H
