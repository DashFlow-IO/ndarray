#!/usr/bin/env bash
# Copyright (c) 2023, the ndarray project authors. Please see
# the CONTRIBUTORS file for details. All rights reserved. Use
# of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.

TOOLS_ROOT="$(readlink -f "$(dirname "$(dirname "$(dirname "${BASH_SOURCE[0]}")")")")/.tools"
if [ ! -d "$TOOLS_ROOT" ]; then
  mkdir $TOOLS_ROOT
fi
if [ ! -d "$TOOLS_ROOT/depot_tools" ]; then
  git clone --depth 1 "https://chromium.googlesource.com/chromium/tools/depot_tools.git" "$TOOLS_ROOT/depot_tools"
fi
