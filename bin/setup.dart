// Copyright (c) 2022, the ndarray project authors. Please see
// the CONTRIBUTORS file for details. All rights reserved. Use
// of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

import 'dart:convert';
import 'dart:io';

import 'package:args/command_runner.dart';
import 'package:ndarray/ndarray.dart';
import 'package:cli_util/cli_logging.dart' show Ansi, Logger;
import 'package:path/path.dart' as path;

late final packageName = () {
  final dirname = path.dirname(Platform.script.path);
  return path.split(dirname).last;
}();

late final binaryStorageDir = '.dart_tool/$packageName/';

/// Download `ndarray` library from Github Releases.
Future<void> downloadBinaries(String platform) async {
  // TODO: implement (tiago)
}

/// Builds `ndarray` from source for the current platform.
void build() {
  final logger = Logger.standard();
  final ansi = Ansi(Ansi.terminalSupportsAnsi);
  final src = sourcePath();
  final pwd = Directory.current;
  final sdkPath = getSdkPath();
  Directory.current = Directory(src);
  logger.stdout('Building $packageName...');
  try {
    final result = Process.runSync('cmake', [
      'CMakeLists.txt',
      '-B',
      path.join(pwd.path, 'build', Platform.operatingSystem),
      '-DCMAKE_BUILD_TYPE=Release',
      '-DDART_SDK=$sdkPath'
    ]);
    print(result.stdout);
    print(result.stderr);
  } catch (error) {
    Directory.current = pwd;
    logger.stdout("${ansi.red}Build failed.${ansi.none}");
    if (Platform.isWindows) {
      logger
          .stdout('Open ${ansi.yellow} Native Tools Command Prompt for VS 2019.'
              '${ansi.none} Then run:\n'
              'cd ${pwd.path}\ndart run $packageName:setup build');
    }
    return;
  }
  var result = Process.runSync('cmake', [
    '--build',
    path.join(pwd.path, 'build', Platform.operatingSystem),
    '--config',
    'Release'
  ]);
  print(result.stdout);
  print(result.stderr);
  if (result.exitCode != 0) exit(result.exitCode);
  Directory.current = pwd;
  final moveLocation = '$binaryStorageDir${Platform.operatingSystem}';
  Directory(moveLocation).createSync(recursive: true);
  final buildOutputPath = _makeBuildPath(pwd.path, getLibName(packageName));
  File(buildOutputPath)
      .copySync(path.join(moveLocation, getLibName(packageName)));
  logger.stdout(
      '${ansi.green}$packageName moved to $moveLocation. Success!${ansi.none}');
  return;
}

String _makeBuildPath(String buildFolderPath, String fileName,
    {bool isDebug = false}) {
  if (Platform.isWindows) {
    return path.join(buildFolderPath, 'build', Platform.operatingSystem,
        isDebug ? 'Debug' : 'Release', fileName);
  } else if (Platform.isMacOS || Platform.isLinux) {
    return path.join(
        buildFolderPath, 'build', Platform.operatingSystem, fileName);
  } else {
    throw Exception('Unsupported Platform.');
  }
}

/// Verify if `ndarray` binary is working correctly.
void verifyBinary() {
  // TODO: implement mobile and web platforms (tiago)
}

/// Finds the root [Uri] of our package.
Uri findPackageRoot() {
  var root = Directory.current.uri;
  do {
    // Traverse up till .dart_tool/package_config.json is found.
    final file = File.fromUri(root.resolve('.dart_tool/package_config.json'));
    if (file.existsSync()) {
      // get package path from package_config.json.
      try {
        final packageMap =
            jsonDecode(file.readAsStringSync()) as Map<String, dynamic>;
        if (packageMap['configVersion'] == 2) {
          var packageRootUriString = (packageMap['packages'] as List<dynamic>)
                  .cast<Map<String, dynamic>>()
                  .firstWhere(
                      (element) => element['name'] == packageName)['rootUri']
              as String;
          packageRootUriString = packageRootUriString.endsWith('/')
              ? packageRootUriString
              : '$packageRootUriString/';
          return file.parent.uri.resolve(packageRootUriString);
        }
      } catch (e, s) {
        print(s);
        throw Exception("Cannot resolve package:$packageName's rootUri");
      }
    }
  } while (root != (root = root.resolve('..')));
  print('Unable to fetch package location.'
      "Make sure you've added package:$packageName as a dependency");
  throw Exception("Cannot resolve package:$packageName's rootUri");
}

/// Gets the `ndarray`'s source code's path, throws [Exception] if not found.
String sourcePath() {
  final packagePath = findPackageRoot();
  final src = packagePath.resolve('src');
  if (!Directory.fromUri(src).existsSync()) {
    throw Exception('Cannot find $packageName source!');
  }
  return src.toFilePath(windows: Platform.isWindows);
}

/// Checks if `ndarray` binaries are already available in the project.
bool isAvailable(String platform) {
  // TODO: implement (tiago)
  return false;
}

/// Return the path to the current Dart SDK.
String getSdkPath() => path.dirname(path.dirname(Platform.resolvedExecutable));

// Available Commands.

class BuildCommand extends Command<void> {
  @override
  String get description => 'Builds the $packageName binaries. Requires cmake.';

  @override
  String get name => 'build';

  @override
  void run() {
    build();
  }
}

class CleanCommand extends Command<void> {
  @override
  String get description => 'Cleans downloaded or built binaries.';

  @override
  String get name => 'clean';

  @override
  void run() {
    print('cleaning...');
    Directory(binaryStorageDir).deleteSync(recursive: true);
  }
}

class VerifyCommand extends Command<void> {
  @override
  String get description => 'Verifies the $packageName binary.';

  @override
  String get name => 'verify';

  @override
  void run() {
    verifyBinary();
  }
}

Future<void> main(List<String> args) async {
  final runner = CommandRunner<void>(
      'setup', 'Downloads/Builds the $packageName binaries.');
  runner
    ..addCommand(BuildCommand())
    ..addCommand(CleanCommand())
    ..addCommand(VerifyCommand());
  if (args.isEmpty) {
    // Targeting only 64bit OS. (At least for the time being.)
    if (validPlatforms.contains('${Platform.operatingSystem}')) {
      await downloadBinaries('${Platform.operatingSystem}');
    }
    // TODO: implement mobile and web platforms (tiago)
  } else {
    await runner.run(args);
  }
}
