import 'dart:io';
import 'package:path/path.dart' as path;
import 'package:grinder/grinder.dart';

List<String> getContributors() {
  //  - Generate a log summary with commit counts.
  //  - Keep everything on the line starting from the second column.
  //  - Sort in lexicographic order.
  var summary =
      run('git', arguments: ['shortlog', 'HEAD', '-sne'], quiet: true);
  List<String> contributors = [];
  summary.split(lineSeparator()).forEach((line) {
    if (line.isNotEmpty) {
      contributors.add(line.split('\t').last);
    }
  });
  contributors.sort();
  return contributors;
}

String lineSeparator() {
  return Platform.isWindows ? '\r\n' : '\n';
}

String relativePath(Uri uri) {
  return path.relative(uri.toFilePath());
}
