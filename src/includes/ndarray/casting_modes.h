/**
 * Copyright (c) 2022, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_CASTING_MODES_H
#define NDARRAY_CASTING_MODES_H

/**
 * Enumeration of ndarray casting modes.
 */
enum NDARRAY_CASTING_MODE {
  // Only allow casting between identical types:
  NDARRAY_NO_CASTING = 0,

  // Allow casting between identical and byte swapped types:
  NDARRAY_EQUIV_CASTING = 1,

  // Only allow "safe" casts:
  NDARRAY_SAFE_CASTING = 2,

  // Allow "safe" casts and casts within the same kind (e.g., between signed
  // integers or between floats):
  NDARRAY_SAME_KIND_CASTING = 3,

  // Allow casting between all types (including between integers and floats):
  NDARRAY_UNSAFE_CASTING = 4
};

#endif  // !NDARRAY_CASTING_MODES_H
