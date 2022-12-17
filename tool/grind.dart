import 'dart:io';

import 'package:grinder/grinder.dart';

import 'helpers.dart';

main(args) => grind(args);

@DefaultTask('Initialize stuff.')
void init() {
  log('Initializing stuff...');
  final ffigen = PubApp.global('ffigen');
  if (!ffigen.isActivated) {
    log('Activating ffigen...');
    ffigen.activate();
  }
}

@Task('Regenerate/Update bindings to native code.')
@Depends(init)
void genBindings([String configFile = 'ffigen.yaml']) {
  PubApp.global('ffigen').run(['--config', configFile]);
}

@Task('Compile stuff.')
@Depends(genBindings)
void compile() {
  log('Compiling stuff...');
}

@Task('Deploy stuff.')
@Depends(compile)
void deploy() {
  log('Deploying stuff...');
}

@Task('List contributors in lexicographic order.')
void listContributors() {
  log('Extracting contributors...');
  final contributors = getContributors();
  log('\nContributors founded:');
  contributors.forEach((contributor) => log(' * $contributor'));
}

@Task('Generate/Update a CONTRIBUTORS file.')
void updateContributors() {
  log('Extracting contributors...');
  var contributorList = getContributors().join('\n');
  log('Generating/Updating the CONTRIBUTORS file...');
  var contributors = File('CONTRIBUTORS');
  var thisScript = relativePath(Platform.script);
  contributors.writeAsString("""
# This file is generated by $thisScript.
#
# Contributors listed in alphabetical order.

$contributorList
""");
}
