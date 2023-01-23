// Copyright (c) 2023, the ndarray project authors. Please see
// the CONTRIBUTORS file for details. All rights reserved. Use
// of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

import 'dart:ffi';

import 'package:ffi/ffi.dart';
import 'package:cli_util/cli_logging.dart';

import 'bindings.dart';
import 'config.dart';
import 'dylib_handler.dart';

NDArray loadAndInitBindings() {
  final ndarray = NDArray(loadDynamicLibrary());
  if (ndarrayVersion != ndarray.VersionString().cast<Utf8>().toDartString()) {
    final logger = Logger.standard();
    final ansi = Ansi(Ansi.terminalSupportsAnsi);
    logger.stderr('${ansi.red}$packageName is outdated.${ansi.none}');
    logger.stdout('Update $packageName by running:\n'
        '${ansi.yellow}flutter pub run $packageName:setup clean\n'
        'flutter pub run $packageName:setup${ansi.none}');
    throw Error();
  }
  // Initialize Dart Native API dynamically.
  ndarray.InitDartApiDL(NativeApi.initializeApiDLData);
  return ndarray;
}

late final _ndarray = loadAndInitBindings();

NDArray get ndarray => _ndarray;
