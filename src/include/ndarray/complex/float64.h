/**
 * Copyright (c) 2023, the ndarray project authors. Please see
 * the CONTRIBUTORS file for details. All rights reserved. Use
 * of this source code is governed by a MIT-style license
 * that can be found in the LICENSE file.
 */

#ifndef NDARRAY_COMPLEX_FLOAT64_H
#define NDARRAY_COMPLEX_FLOAT64_H

#include <complex.h>
#include <stdint.h>
#include "float32.h"

/**
 * If C++, prevent name mangling so that the compiler emits a binary file having
 * undecorated names, thus mirroring the behavior of a C compiler.
 */
#ifdef __cplusplus
extern "C" {
#endif

// Check for C11 support where we can precisely define a complex number and thus
// avoid issues concerning infinities and NaNs as real and/or imaginary
// components...
#if defined(_Imaginary_I) && defined(CMPLX)

/**
 * An opaque type definition for a double-precision complex floating-point
 * number.
 */
typedef double complex ndarray_complex128_t;

// If we aren't going to use the native complex number type, we need to define a
// complex number as an "opaque" struct (here, "opaque" meaning type consumers
// should **not** be accessing the components directly, but only through
// dedicated functions) for storing the real and imaginary components...
#else

/**
 * An opaque type definition for a double-precision complex floating-point
 * number.
 *
 * @example
 * ndarray_complex128_t z;
 *
 * // Set the real component:
 * z.re = 5.0;
 *
 * // Set the imaginary component:
 * z.im = 2.0;
 */
typedef struct {
  /**
   * Real component.
   */
  double re;

  /**
   * Imaginary component.
   */
  double im;
} ndarray_complex128_t;

#endif

/**
 * An opaque type definition for a union for accessing the real and imaginary
 * parts of a double-precision complex floating-point number.
 *
 * @example
 * double real(const ndarray_complex128_t z) {
 *   ndarray_complex128_parts_t v;
 *
 *   // Assign a double-precision complex floating-point number:
 *   v.value = z;
 *
 *   // Extract the real component:
 *   double re = v.parts[0];
 *
 *   return re;
 * }
 *
 * // ...
 *
 * // Create a complex number:
 * ndarray_complex128_t z = ndarray_complex128(5.0, 2.0);
 *
 * // ...
 *
 * // Access the real component:
 * double re = real(z);
 * // returns 5.0
 */
typedef union {
  // An opaque type for the output value (e.g., could be a `struct` or a C99
  // complex number):
  ndarray_complex128_t value;

  // Leverage the fact that C99 specifies that complex numbers have the same
  // representation and alignment as a two-element array (see
  // <https://en.cppreference.com/w/c/language/arithmetic_types#Complex_floating_types>),
  // where the first element is the real component and the second element is the
  // imaginary component, thus allowing us to create a complex number
  // irrespective of its native data type (e.g., `struct` vs `double complex`):
  double parts[2];
} ndarray_complex128_parts_t;

/**
 * Returns a double-precision complex floating-point number.
 */
ndarray_complex128_t ndarray_complex128(const double real, const double imag);

/**
 * Converts a single-precision floating-point number to a double-precision
 * complex floating-point number.
 */
ndarray_complex128_t ndarray_complex128_from_float32(const float real);

/**
 * Converts a double-precision floating-point number to a double-precision
 * complex floating-point number.
 */
ndarray_complex128_t ndarray_complex128_from_float64(const double real);

/**
 * Converts a single-precision complex floating-point number to a
 * double-precision complex floating-point number.
 */
ndarray_complex128_t ndarray_complex128_from_complex64(
    const ndarray_complex64_t z
);

/**
 * Converts (copies) a double-precision complex floating-point number to a
 * double-precision complex floating-point number.
 */
ndarray_complex128_t ndarray_complex128_from_complex128(
    const ndarray_complex128_t z
);

/**
 * Converts a signed 8-bit integer to a double-precision complex floating-point
 * number.
 */
ndarray_complex128_t ndarray_complex128_from_int8(const int8_t real);

/**
 * Converts an unsigned 8-bit integer to a double-precision complex
 * floating-point number.
 */
ndarray_complex128_t ndarray_complex128_from_uint8(const uint8_t real);

/**
 * Converts a signed 16-bit integer to a double-precision complex floating-point
 * number.
 */
ndarray_complex128_t ndarray_complex128_from_int16(const int16_t real);

/**
 * Converts an unsigned 16-bit integer to a double-precision complex
 * floating-point number.
 */
ndarray_complex128_t ndarray_complex128_from_uint16(const uint16_t real);

/**
 * Converts a signed 32-bit integer to a double-precision complex floating-point
 * number.
 */
ndarray_complex128_t ndarray_complex128_from_int32(const int32_t real);

/**
 * Converts an unsigned 32-bit integer to a double-precision complex
 * floating-point number.
 */
ndarray_complex128_t ndarray_complex128_from_uint32(const uint32_t real);

/**
 * Converts a double-precision complex floating-point number to a
 * single-precision complex floating-point number.
 */
// clang-format off
ndarray_complex64_t ndarray_complex128_to_complex64(const ndarray_complex128_t z);
// clang-format on

#ifdef __cplusplus
}
#endif

#endif  // !NDARRAY_COMPLEX_FLOAT64_H
