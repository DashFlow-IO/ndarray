// Copyright (c) 2023, the ndarray project authors. Please see
// the CONTRIBUTORS file for details. All rights reserved. Use
// of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

import 'dart:ffi';
import 'dart:io';

import 'package:cli_util/cli_logging.dart' show Ansi, Logger;
import 'package:ndarray/src/utils.dart';

import 'config.dart';

/// Resolves the absolute path of a resource (usually a dylib).
///
/// Checks if a dynamic library is located in -
///   1. Present Working Directory and it's .dart_tool.
///   2. Current script's/executable's directory and it's .dart_tool.
///   3. Current script's/executable's directory's parent and it's .dart_tool.
/// and returns the absolute path or [null] if can't be resolved.
String? _resolveLibUri(String name) {
  var libUri = Directory.current.uri.resolve(name);
  var dartTool = '.dart_tool/$packageName/';

  // If lib is in Present Working Directory.
  if (doesFileExist(libUri)) {
    return libUri.toFilePath(windows: Platform.isWindows);
  }

  // If lib is in Present Working Directory's .dart_tool folder.
  if (Platform.isWindows) {
    dartTool += 'windows';
  } else if (Platform.isMacOS) {
    dartTool += 'macos';
  } else {
    dartTool += 'linux';
  }

  libUri = Directory.current.uri.resolve('$dartTool/$name');
  if (doesFileExist(libUri)) {
    return libUri.toFilePath(windows: Platform.isWindows);
  }

  // If lib is in script's directory.
  libUri = Uri.directory(dirname(Platform.script.path)).resolve(name);

  if (doesFileExist(libUri)) {
    return libUri.toFilePath(windows: Platform.isWindows);
  }

  // If lib is in script's .dart_tool directory.
  libUri =
      Uri.directory(dirname(Platform.script.path)).resolve('$dartTool/$name');
  if (doesFileExist(libUri)) {
    return libUri.toFilePath(windows: Platform.isWindows);
  }

  // If lib is in executable's directory.
  libUri = Uri.directory(dirname(Platform.resolvedExecutable)).resolve(name);

  if (doesFileExist(libUri)) {
    return libUri.toFilePath(windows: Platform.isWindows);
  }

  // If lib is in executable's .dart_tool directory.
  libUri = Uri.directory(dirname(Platform.resolvedExecutable))
      .resolve('$dartTool/$name');

  if (doesFileExist(libUri)) {
    return libUri.toFilePath(windows: Platform.isWindows);
  }

  // If lib is in script's directory's parent.

  libUri = Uri.directory(dirname(Platform.script.path)).resolve('../$name');

  if (doesFileExist(libUri)) {
    return libUri.toFilePath(windows: Platform.isWindows);
  }

  // If lib is in script's directory's parent's .dart_tool.

  libUri = Uri.directory(dirname(Platform.script.path))
      .resolve('../$dartTool/$name');

  if (doesFileExist(libUri)) {
    return libUri.toFilePath(windows: Platform.isWindows);
  }

  // If lib is in executable's directory's parent.

  libUri =
      Uri.directory(dirname(Platform.resolvedExecutable)).resolve('../$name');

  if (doesFileExist(libUri)) {
    return libUri.toFilePath(windows: Platform.isWindows);
  }

  // If lib is in executable's directory's parent's .dart_tool.
  libUri = Uri.directory(dirname(Platform.resolvedExecutable))
      .resolve('..../$dartTool/$name');

  if (doesFileExist(libUri)) {
    return libUri.toFilePath(windows: Platform.isWindows);
  }

  return null;
}

/// Loads dynamic library depending on the platform.
///
/// Throws an [ArgumentError] if library can't be loaded.
DynamicLibrary loadDylib(String name) {
  // _resolveLibUri() will try to resolve ndarray's absolute path.
  // If it can't find it, try looking at search paths provided by the system.
  name = _resolveLibUri(name) ?? name;

  try {
    return DynamicLibrary.open(name);
  } catch (exception) {
    final logger = Logger.standard();
    final ansi = Ansi(Ansi.terminalSupportsAnsi);

    logger
        .stderr('${ansi.red}Failed to open the library. Make sure that required'
            ' binaries are in place.${ansi.none}');
    logger.stdout(
        'To download the binaries, please run the following from the root of'
        ' your project:');
    logger.stdout('${ansi.yellow}dart run $packageName:setup${ansi.none}');
    logger.stdout('${ansi.green}Valid platforms are:');
    for (final platform in validPlatforms) {
      logger.stdout(platform);
    }
    logger.stdout(ansi.none);
    rethrow;
  }
}

/// Loads `ndarray` dynamic library depending on the platform.
DynamicLibrary loadDynamicLibrary() {
  return loadDylib(getLibName());
}
