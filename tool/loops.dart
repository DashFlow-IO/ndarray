// Copyright (c) 2022, the ndarray project authors. Please see
// the CONTRIBUTORS file for details. All rights reserved. Use
// of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

import 'package:ndarray/ndarray.dart';

/// Generates a list of loop signatures from a list of [dtypes] names.
List<String> signatures(List<String> dtypes) {
  // Generate the list of signatures:
  final out = <String>[];
  for (final dtype in dtypes) {
    // Resolve single-letter dtype abbreviation:
    final inputChar = DType.fromString(dtype).char;

    // Generate the input/output array signature:
    out.add('${inputChar}_$inputChar'); // e.g., d_d
    // TODO: finalize implementation (tiago)
  }

  out.sort();
  return out;
}
