// Copyright (c) 2023, the ndarray project authors. Please see
// the CONTRIBUTORS file for details. All rights reserved. Use
// of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

import 'dart:io';

const validPlatforms = <String>['linux', 'windows', 'macos'];

const packageName = 'ndarray';

const ndarrayVersion = "0.0.1";

String getDylibName(String name, [String platform = '']) {
  var ext = '.so';
  var prefix = 'lib';

  if (Platform.isWindows || platform.startsWith('windows')) {
    prefix = '';
    ext = '.dll';
  } else if (Platform.isMacOS || platform.startsWith('macos')) {
    ext = '.dylib';
  } else if (!(Platform.isLinux || platform.startsWith('linux'))) {
    // If NOT even linux, then unsupported.
    throw Exception('Unsupported Platform.');
  }
  return prefix + name + ext;
}

String getLibName({String name = packageName, String platform = ''}) {
  return getDylibName(name, platform);
}
