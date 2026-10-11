#!/usr/bin/env bash

set -euo pipefail

rm -rf build && mkdir build

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

cmake -S "${SCRIPT_DIR}" -B "${SCRIPT_DIR}/build" -G Ninja
cmake --build "${SCRIPT_DIR}/build"

build/cedis