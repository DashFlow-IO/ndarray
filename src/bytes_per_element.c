#include "ndarray/bytes_per_element.h"
#include <stdint.h>
#include "ndarray/dtypes.h"

/**
 * Returns the number of bytes per element for a given data type.
 *
 * ## Notes
 *
 * -   If unable to resolve a provided data type, the function returns `0`.
 *
 * @param dtype  data type (number)
 * @return       number of bytes per element
 *
 * @example
 * #include "ndarray/base/bytes_per_element.h"
 * #include "ndarray/dtypes.h"
 * #include <stdint.h>
 *
 * int64_t nbytes = ndarray_bytes_per_element(NDARRAY_FLOAT64);
 * // returns 8
 */
int64_t ndarray_bytes_per_element(enum NDARRAY_DTYPE dtype) {
  switch (dtype) {
    case NDARRAY_FLOAT64:
      return NDARRAY_FLOAT64_BYTES_PER_ELEMENT;
    case NDARRAY_FLOAT32:
      return NDARRAY_FLOAT32_BYTES_PER_ELEMENT;

    case NDARRAY_INT8:
      return NDARRAY_INT8_BYTES_PER_ELEMENT;
    case NDARRAY_UINT8:
      return NDARRAY_UINT8_BYTES_PER_ELEMENT;
    case NDARRAY_UINT8C:
      return NDARRAY_UINT8C_BYTES_PER_ELEMENT;

    case NDARRAY_INT16:
      return NDARRAY_INT16_BYTES_PER_ELEMENT;
    case NDARRAY_UINT16:
      return NDARRAY_UINT16_BYTES_PER_ELEMENT;

    case NDARRAY_INT32:
      return NDARRAY_INT32_BYTES_PER_ELEMENT;
    case NDARRAY_UINT32:
      return NDARRAY_UINT32_BYTES_PER_ELEMENT;

    case NDARRAY_INT64:
      return NDARRAY_INT64_BYTES_PER_ELEMENT;
    case NDARRAY_UINT64:
      return NDARRAY_UINT64_BYTES_PER_ELEMENT;

    case NDARRAY_BOOL:
      return NDARRAY_BOOL_BYTES_PER_ELEMENT;

    case NDARRAY_BINARY:
      return NDARRAY_BINARY_BYTES_PER_ELEMENT;

    case NDARRAY_COMPLEX64:
      return NDARRAY_COMPLEX64_BYTES_PER_ELEMENT;
    case NDARRAY_COMPLEX128:
      return NDARRAY_COMPLEX128_BYTES_PER_ELEMENT;

    default:
      return 0;  // data type is not currently supported
  }
}
