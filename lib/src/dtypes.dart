// Copyright (c) 2023, the ndarray project authors. Please see
// the CONTRIBUTORS file for details. All rights reserved. Use
// of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

import 'bindings.dart';

/// Enumeration of underlying ndarray data types.
enum DType {
  bool(NDARRAY_DTYPE.NDARRAY_BOOL, NDARRAY_DTYPE_CHAR.NDARRAY_BOOL_CHAR),

  /// Integer data types:
  int8(NDARRAY_DTYPE.NDARRAY_INT8, NDARRAY_DTYPE_CHAR.NDARRAY_INT8_CHAR),
  uInt8(NDARRAY_DTYPE.NDARRAY_UINT8, NDARRAY_DTYPE_CHAR.NDARRAY_UINT8_CHAR),
  uInt8C(NDARRAY_DTYPE.NDARRAY_UINT8C, NDARRAY_DTYPE_CHAR.NDARRAY_UINT8_CHAR),
  int16(NDARRAY_DTYPE.NDARRAY_INT16, NDARRAY_DTYPE_CHAR.NDARRAY_INT16_CHAR),
  uInt16(NDARRAY_DTYPE.NDARRAY_UINT16, NDARRAY_DTYPE_CHAR.NDARRAY_UINT16_CHAR),
  int32(NDARRAY_DTYPE.NDARRAY_INT32, NDARRAY_DTYPE_CHAR.NDARRAY_INT32_CHAR),
  uInt32(NDARRAY_DTYPE.NDARRAY_UINT32, NDARRAY_DTYPE_CHAR.NDARRAY_UINT8_CHAR),
  int64(NDARRAY_DTYPE.NDARRAY_INT64, NDARRAY_DTYPE_CHAR.NDARRAY_UINT32_CHAR),
  uInt64(NDARRAY_DTYPE.NDARRAY_UINT64, NDARRAY_DTYPE_CHAR.NDARRAY_UINT64_CHAR),
  int128(NDARRAY_DTYPE.NDARRAY_INT128, NDARRAY_DTYPE_CHAR.NDARRAY_INT128_CHAR),
  uInt128(
      NDARRAY_DTYPE.NDARRAY_UINT128, NDARRAY_DTYPE_CHAR.NDARRAY_UINT128_CHAR),
  int256(NDARRAY_DTYPE.NDARRAY_INT256, NDARRAY_DTYPE_CHAR.NDARRAY_INT256_CHAR),
  uInt256(
      NDARRAY_DTYPE.NDARRAY_UINT256, NDARRAY_DTYPE_CHAR.NDARRAY_UINT256_CHAR),

  /// Floating-point data types:
  float16(
      NDARRAY_DTYPE.NDARRAY_FLOAT16, NDARRAY_DTYPE_CHAR.NDARRAY_FLOAT16_CHAR),
  bFloat16(
      NDARRAY_DTYPE.NDARRAY_BFLOAT16, NDARRAY_DTYPE_CHAR.NDARRAY_BFLOAT16_CHAR),
  float32(
      NDARRAY_DTYPE.NDARRAY_FLOAT32, NDARRAY_DTYPE_CHAR.NDARRAY_FLOAT32_CHAR),
  float64(
      NDARRAY_DTYPE.NDARRAY_FLOAT64, NDARRAY_DTYPE_CHAR.NDARRAY_FLOAT64_CHAR),
  float128(
      NDARRAY_DTYPE.NDARRAY_FLOAT128, NDARRAY_DTYPE_CHAR.NDARRAY_FLOAT128_CHAR),

  /// Complex floating-point number data types:
  complex64(NDARRAY_DTYPE.NDARRAY_COMPLEX64,
      NDARRAY_DTYPE_CHAR.NDARRAY_COMPLEX64_CHAR),
  complex128(NDARRAY_DTYPE.NDARRAY_COMPLEX128,
      NDARRAY_DTYPE_CHAR.NDARRAY_COMPLEX128_CHAR),

  /// Define a data type for "binary" data:
  binary(NDARRAY_DTYPE.NDARRAY_BINARY, NDARRAY_DTYPE_CHAR.NDARRAY_BINARY_CHAR),

  /// Define a data type for "generic" Dart values:
  generic(
      NDARRAY_DTYPE.NDARRAY_GENERIC, NDARRAY_DTYPE_CHAR.NDARRAY_GENERIC_CHAR),

  /// "Compute" the number of data types (this works because of how `enum`
  /// works: the value is automatically set to the last enumerated type plus 1):
  ndTypes(NDARRAY_DTYPE.NDARRAY_NDTYPES),

  /// Reserve a signaling value which is guaranteed not to be a valid type
  /// enumeration number:
  noType(NDARRAY_DTYPE.NDARRAY_NOTYPE),

  /// Indicate the start of user defined type numbers:
  userDefinedType(NDARRAY_DTYPE.NDARRAY_USERDEFINED_TYPE);

  final int value;
  final int charCode;
  const DType(this.value, [this.charCode = 0]);

  factory DType.fromString(String name) {
    return DType.values.singleWhere((dtype) => dtype.name == name);
  }

  String get char {
    return String.fromCharCode(charCode);
  }

  static List<String> get names {
    return values
        .getRange(0, ndTypes.value)
        .map((dtype) => dtype.name.toLowerCase())
        .toList();
  }
}
