/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_EXPORT_H_
#define NDARRAY_EXPORT_H_

#if defined(WIN32)
#define NDARRAY_EXPORT __declspec(dllexport)
#else
#define NDARRAY_EXPORT __attribute__((visibility("default")))
#endif

#endif  //  NDARRAY_EXPORT_H_