#!/usr/bin/env bash
# Copyright (c) 2023, the ndarray project authors. Please see
# the CONTRIBUTORS file for details. All rights reserved. Use
# of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.

TOOLS_ROOT="$(readlink -f "$(dirname "$(dirname "$(dirname "${BASH_SOURCE[0]}")")")")/.tools"
if [ -d "$TOOLS_ROOT/../.dart_tool" ]; then
  rm -rf  "$TOOLS_ROOT/../.dart_tool"
fi
if [ ! -d "$TOOLS_ROOT/engine" ]; then
  if [[ ! "$PATH" == *"buildtools"* ]]; then
    echo "export PATH=\"\$PATH:$TOOLS_ROOT/engine/src/buildtools/mac-x64/clang/bin/\"" >> "$HOME/.zprofile"
    source "$HOME/.zprofile"
  fi

  mkdir "$TOOLS_ROOT/engine"
  DOT_GCLIENT="$TOOLS_ROOT/engine/.gclient"
  ENGINE_REPO="https://github.com/flutter/engine.git"
  echo "solutions = [
  {
    \"managed\": False,
    \"name\": \"src/flutter\",
    \"url\": \"$ENGINE_REPO\",
    \"custom_deps\": {},
    \"deps_file\": \"DEPS\",
    \"safesync_url\": \"\",
    \"custom_vars\": {
      \"download_emsdk\": True,
    },
  },
]" > "$DOT_GCLIENT"
  update_depot_tools_toggle.py --disable
  cd "$TOOLS_ROOT/engine" && gclient sync --shallow
  while [ -d _bad_scm ];
  do
    rm -rf _bad_scm
    gclient sync --shallow
  done
  cd src && git checkout master &&  git pull
  cd flutter && git checkout main && git pull && cd ..
  ./flutter/tools/gn \
    --unoptimized \
    --no-prebuilt-dart-sdk \
    --no-goma \
    --full-dart-debug \
    --runtime-mode debug
  ninja -C out/$1
fi