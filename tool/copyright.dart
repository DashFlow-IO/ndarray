// Copyright (c) 2022, the ndarray project authors. Please see
// the CONTRIBUTORS file for details. All rights reserved. Use
// of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

import 'dart:io';
import 'package:glob/glob.dart';
import 'package:glob/list_local_fs.dart';
import 'package:yaml/yaml.dart';

import 'utils.dart';

Copyright loadCopyright([path = 'copyright.yaml']) {
  final yaml = loadYaml(File(path).readAsStringSync());
  return Copyright(yaml);
}

class CommentStyle {
  final String start;
  final String middle;
  final String end;
  CommentStyle._({this.start = '', this.middle = '// ', this.end = ''});
}

class Copyright {
  static const DEFAULT_AUTHORS = 'ndarray project authors';
  static const DEFAULT_LICENSE = 'LICENSE';
  static const DEFAULT_TYPE = 'MIT';
  static const DEFAULT_CONTRIBUTORS = 'CONTRIBUTORS';
  static const DEFAULT_INCLUDE = '*';
  static const DEFAULT_HEADER = """
Copyright (c) {{ year }}, the {{ authors }}. Please see the
{{ contributors }} file for details. All rights reserved. Use of
this source code is governed by a {{ type }}-style license that
can be found in the {{ license }} file.
""";

  final Map<String, String> arguments;
  final Glob include;
  final String header;
  final Map<String, CommentStyle> commentStyleMap;

  final _templatesRegexes = {
    'year': '(?<year>[0-9]*)',
    'authors': '(?<authors>[\\w|\\s]+)',
    'license': '(?<license>[\\w|\\s]+)',
    'type': '(?<type>[\\w|\\s]+)',
    'contributors': '(?<contributors>[\\w|\\s]+)'
  };

  final _regexHeaders = <String, RegExp>{};
  final _renderedHeders = <String, String>{};

  Copyright._(this.arguments, this.include, this.header, this.commentStyleMap);

  factory Copyright(YamlMap yaml) {
    var commentStyleMap = <String, CommentStyle>{'*': CommentStyle._()};
    if (yaml.containsKey('comment')) {
      (yaml['comment'] as Map).forEach((key, value) {
        Map commentStyle = value;
        commentStyleMap[key] = CommentStyle._(
            start: commentStyle['start'],
            middle: commentStyle['middle'],
            end: commentStyle['end']);
      });
    }
    return Copyright._(
        {
          'year': yaml['year'] ?? DateTime.now().year.toString(),
          'authors': yaml['authors'] ?? DEFAULT_AUTHORS,
          'license': yaml['license'] ?? DEFAULT_LICENSE,
          'type': yaml['type'] ?? DEFAULT_TYPE,
          'contributors': yaml['contributors'] ?? DEFAULT_CONTRIBUTORS
        },
        yaml.containsKey('include')
            ? Glob("{${yaml['include'].join(',')}}")
            : Glob(DEFAULT_INCLUDE),
        yaml['header'] ?? DEFAULT_HEADER,
        commentStyleMap);
  }
  void apply() {
    for (final entity in include.listSync()) {
      final extension = fileExtension(entity);
      final file = File(entity.path);
      var text = file.readAsStringSync();
      final match = _matchCopyright(text, extension);
      if (match == null) {
        file.writeAsStringSync(_insertCopyright(text, extension));
      } else if (_isDirty(match)) {
        file.writeAsStringSync(_updateCopyright(text, extension, match));
      }
    }
  }

  RegExpMatch? _matchCopyright(String textString, String extension) {
    if (textString.isEmpty) return null;
    final regex = _regexHeader(extension);
    return regex.firstMatch(textString);
  }

  String _insertCopyright(String text, String extension) {
    final header = _renderHeader(extension);
    return '$header\n\n$text';
  }

  String _updateCopyright(String text, String extension, RegExpMatch match) {
    var newText = text.substring(0, match.start);
    if (match.end + 1 <= text.length) newText += text.substring(match.end + 1);
    return _insertCopyright(newText, extension);
  }

  String _renderHeader(String extension, {Map<String, String>? args}) {
    args = args ?? arguments;
    if (!_renderedHeders.containsKey(extension)) {
      final comment = _commentStyleFor(extension);
      var renderedHeader = renderTemplate(header, args);
      _renderedHeders[extension] = _asCommentHeader(renderedHeader, comment);
    }
    return _renderedHeders[extension]!;
  }

  RegExp _regexHeader(String extension) {
    if (!_regexHeaders.containsKey(extension)) {
      final comment = _commentStyleFor(extension);
      var scaped = RegExp.escape(_asCommentHeader(header, comment));
      scaped = scaped.replaceAll(r'\{', '{');
      scaped = scaped.replaceAll(r'\}', '}');
      final regex = RegExp(renderTemplate(scaped, _templatesRegexes));
      _regexHeaders[extension] = regex;
    }
    return _regexHeaders[extension]!;
  }

  String _asCommentHeader(String header, CommentStyle comment) {
    final lines = [
      if (comment.start.isNotEmpty) "${comment.start}",
      ...header.split('\n').map((line) => '${comment.middle}$line'),
      if (comment.end.isNotEmpty) "${comment.end}"
    ];
    return lines.join('\n');
  }

  CommentStyle _commentStyleFor(String extension) {
    for (final entry in commentStyleMap.entries) {
      if (entry.key.contains(extension)) return entry.value;
    }
    return commentStyleMap[DEFAULT_INCLUDE]!;
  }

  bool _isDirty(RegExpMatch match) {
    if (match.start > 0) return true;
    return arguments.keys
        .any((argName) => arguments[argName] != match.namedGroup(argName));
  }
}
