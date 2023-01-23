#!/usr/bin/env bash
# Copyright (c) 2023, the ndarray project authors. Please see
# the CONTRIBUTORS file for details. All rights reserved. Use
# of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.

TOOLS_ROOT="$(readlink -f "$(dirname "$(dirname "$(dirname "${BASH_SOURCE[0]}")")")")/.tools"
if [ ! -d "$TOOLS_ROOT/flutter" ]; then
  git clone "https://github.com/flutter/flutter.git" "$TOOLS_ROOT/flutter"
  if [[ ! "$PATH" == *"flutter"* ]]; then
    echo "export DART_SDK=\"$TOOLS_ROOT/flutter/bin/cache/dart-sdk\"" >> "$HOME/.zprofile"
    echo "export PATH=\"\$PATH:$TOOLS_ROOT/flutter/bin\"" >> "$HOME/.zprofile"
    source "$HOME/.zprofile"
  fi
  if [[ ! "$PATH" == *"pub-cache"* ]]; then
    echo "export PATH=\"\$PATH:\$HOME/.pub-cache/bin\"" >> "$HOME/.zprofile"
    source "$HOME/.zprofile"
  fi
  flutter doctor
  if [[ ! "$LOCAL_ENGINE" ]]; then
    echo "export LOCAL_ENGINE=\"$1\"" >> "$HOME/.zprofile"
  fi
  if [[ ! "$FLUTTER_ENGINE" ]]; then
    echo "export FLUTTER_ENGINE=\"$TOOLS_ROOT/engine/src/\"" >> "$HOME/.zprofile"
  fi
  source "$HOME/.zprofile"
  flutter upgrade --force "--local-engine=$LOCAL_ENGINE"
  flutter "--local-engine=$LOCAL_ENGINE" pub global activate grinder
fi