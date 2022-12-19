/* 
 * Copyright (c) 2022, the ndarray project authors. Please see 
 * the CONTRIBUTORS file for details. All rights reserved. Use 
 * of this source code is governed by a MIT-style license 
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_INDEX_MODES_H
#define NDARRAY_INDEX_MODES_H

/**
 * Enumeration of ndarray index modes.
 */
enum NDARRAY_INDEX_MODE {
    NDARRAY_INDEX_ERROR = 1,
    NDARRAY_INDEX_CLAMP = 2,
    NDARRAY_INDEX_WRAP = 3
};

#endif // !NDARRAY_INDEX_MODES_H