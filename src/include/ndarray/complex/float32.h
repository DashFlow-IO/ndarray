#ifndef NDARRAY_COMPLEX_FLOAT32_H
#define NDARRAY_COMPLEX_FLOAT32_H

#include <complex.h>
#include <stdint.h>

/*
* If C++, prevent name mangling so that the compiler emits a binary file
* having undecorated names, thus mirroring the behavior of a C compiler.
*/
#ifdef __cplusplus
extern "C" {
#endif

	// Check for C11 support where we can precisely define a complex number 
	// and thus avoid issues concerning infinities and NaNs as realand /or 
	// imaginary components...
#if defined(_Imaginary_I) && defined(CMPLXF)

	/**
	 * An opaque type definition for a single-precision complex floating-point
	 * number.
	 */
	typedef float complex ndarray_complex64_t;

	// If we aren't going to use the native complex number type, we need to 
	// define a complex number as an "opaque" struct (here, "opaque" meaning 
	// type consumers should** not**be accessing the components directly, but 
	// only through dedicated functions) for storing the realand imaginary 
	// components...
#else

	/**
	 * An opaque type definition for a single-precision complex floating-point
	 * number.
	 *
	 * @example
	 * ndarray_complex64_t z;
	 *
	 * // Set the real component:
	 * z.re = 5.0f;
	 *
	 * // Set the imaginary component:
	 * z.im = 2.0f;
	 */
	typedef struct {
		/**
		 * Real component.
		 */
		float re;

		/**
		 * Imaginary component.
		 */
		float im;
	} ndarray_complex64_t;

#endif

	/**
	 * An opaque type definition for a union for accessing the real and imaginary
	 * parts of a single-precision complex floating-point number.
	 *
	 * @example
	 * float realf(const ndarray_complex64_t z) {
	 *     ndarray_complex64_parts_t v;
	 *
	 *     // Assign a single-precision complex floating-point number:
	 *     v.value = z;
	 *
	 *     // Extract the real component:
	 *     float re = v.parts[0];
	 *
	 *     return re;
	 * }
	 *
	 * // ...
	 *
	 * // Create a complex number:
	 * ndarray_complex64_t z = ndarray_complex64(5.0f, 2.0f);
	 *
	 * // ...
	 *
	 * // Access the real component:
	 * float re = realf(z);
	 * // returns 5.0f
	 */
	typedef union {
		// An opaque type for the output value (e.g., could be a `struct` or 
		// a C99 complex number):
		ndarray_complex64_t value;

		// Leverage the fact that C99 specifies that complex numbers have the 
		// same representationand alignment as a two - element array(see 
		// <https://en.cppreference.com/w/c/language/arithmetic_types#Complex_floating_types>), 
		// where the first element is the real componentand the second element 
		// is the imaginary component, thus allowing us to create a complex number 
		// irrespective of its native data type(e.g., `struct` vs `float complex`):
		float parts[2];
	} ndarray_complex64_parts_t;

	/**
	 * Returns a single-precision complex floating-point number.
	 */
	ndarray_complex64_t ndarray_complex64(const float real, const float imag);

	/**
	 * Converts a single-precision floating-point number to a single-precision complex
	 * floating-point number.
	 */
	ndarray_complex64_t ndarray_complex64_from_float32(const float real);

	/**
	 * Converts a double-precision floating-point number to a single-precision complex
	 * floating-point number.
	 */
	ndarray_complex64_t ndarray_complex64_from_float64(const double real);

	/**
	 * Converts (copies) a single-precision complex floating-point number to a
	 * single-precision complex floating-point number.
	 */
	ndarray_complex64_t ndarray_complex64_from_complex64(const ndarray_complex64_t z);

	/**
	 * Converts a signed 8-bit integer to a single-precision complex floating-point number.
	 */
	ndarray_complex64_t ndarray_complex64_from_int8(const int8_t real);

	/**
	 * Converts an unsigned 8-bit integer to a single-precision complex floating-point
	 * number.
	 */
	ndarray_complex64_t ndarray_complex64_from_uint8(const uint8_t real);

	/**
	 * Converts a signed 16-bit integer to a single-precision complex floating-point
	 * number.
	 */
	ndarray_complex64_t ndarray_complex64_from_int16(const int16_t real);

	/**
	 * Converts an unsigned 16-bit integer to a single-precision complex floating-point
	 * number.
	 */
	ndarray_complex64_t ndarray_complex64_from_uint16(const uint16_t real);

#ifdef __cplusplus
}
#endif

#endif // !NDARRAY_COMPLEX_FLOAT32_H
