import 'dart:io';
import 'package:path/path.dart' as path;
import 'package:mustache_template/mustache_template.dart' as mustache;

String relativePath(Uri uri) {
  return path.relative(uri.toFilePath());
}

String fileName(String pathString) {
  return path.basename(pathString).split('.').first;
}

String fileExtension(FileSystemEntity entity) {
  return path.extension(entity.path).substring(1);
}

/// Renders the [templateString] filling it with the given [arguments] values.
String renderTemplate(String templateString, Map<String, String> arguments) {
  return mustache.Template(templateString, htmlEscapeValues: false)
      .renderString(arguments);
}
