# ndarray

N-Dimensional Arrays in Dart.

## Contributing

### Project stucture

This project uses the following structure:

* `src`: Contains the native source code, and a CMakeLists.txt file for building
  that source code into a dynamic library.

* `lib`: Contains the Dart code that defines the API for the `ndarray` project, 
  and which calls into the native code using `dart:ffi`.

* `bin`: Contains the Dart code that downloads/builds/veryfies the `ndarray`
   binaries according to the current platform.

* `tool`: Contains the Dart code for running the `grinder` tasks.

* platform folders (`android`, `ios`, `windows`, etc.): Contains the build files
  for building and bundling the native code library with the platform application.

### Running Tasks

First you need to activate `grinder`:

    flutter pub global activate grinder

then use it to run desired tasks:

    grind init
    grind bindings
    ...

or to run a default task (see `@DefaultTask` in [tool/gring.dart](tool/grind.dart)):

    grind

or to display a list of available tasks and their dependencies:

    grind -h

> **_NOTE:_** On vscode, `grind` is automatically globally activated.

### Binding to native code

To use the native code, bindings in Dart are needed.
To avoid writing these by hand, they are generated from the headers files (see the `headers` section in [ffigen.yaml](ffigen.yaml))
(`src/ndarray.h`) by `package:ffigen`.
Regenerate the bindings by running `grind bindings` or `flutter pub run ffigen  --config ffigen.yaml`.

## Usage

1. Add package as a dependency in your `pubspec.yaml`.

2. Run this from the `root` of your project.

   ```bash
   flutter pub get
   flutter pub run ndarray:setup # Downloads the ndarray binaries.
   ```
3. To see `ndarray:setup` available commands, run:

   ```bash
   flutter pub run ndarray:setup --help 
   ```

4. Import

   ```dart
   import 'package:ndarray/ndarray.dart';
   ```

> **_NOTE:_** Internet connection is required to download ndarray binaries.

