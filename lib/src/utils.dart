// Copyright (c) 2023, the ndarray project authors. Please see
// the CONTRIBUTORS file for details. All rights reserved. Use
// of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

import 'dart:io';
import 'package:path/path.dart' as path;

/// Checks if [File]/[Link] exists for an [uri].
bool doesFileExist(Uri uri) {
  return File.fromUri(uri).existsSync() || Link.fromUri(uri).existsSync();
}

String dirname(String pathString) {
  return path.dirname(pathString);
}
