import 'dart:io';
import 'package:path/path.dart' as path;

/// Checks if [File]/[Link] exists for an [uri].
bool doesFileExist(Uri uri) {
  return File.fromUri(uri).existsSync() || Link.fromUri(uri).existsSync();
}

String dirname(String pathString) {
  return path.dirname(pathString);
}
