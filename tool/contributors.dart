import 'package:grinder/grinder.dart';

List<String> contributors() {
  // - Generate a log summary with commit counts.
  // - Keep everything on the line starting from the second column.
  // - Sort in lexicographic order.
  var summary =
      run('git', arguments: ['shortlog', 'HEAD', '-sne'], quiet: true);
  List<String> contributors = [];
  summary.split('\n').forEach((line) {
    if (line.isNotEmpty && !line.contains('github')) {
      contributors.add(line.split('\t').last);
    }
  });
  contributors.sort();
  return contributors;
}
