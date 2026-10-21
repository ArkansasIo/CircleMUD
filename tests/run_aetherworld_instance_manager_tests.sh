#!/bin/sh
set -eu
CC=${CC:-cc}
CFLAGS="${CFLAGS:--std=c99 -Wall -Wextra -Werror -pedantic -Isrc}"
OUT="${TMPDIR:-/tmp}/aetherworld-instance-manager-tests"
$CC $CFLAGS tests/aetherworld_instance_manager_test.c \
  src/aetherworld_instance_manager.c src/aetherworld_instance.c -o "$OUT"
"$OUT"
rm -f "$OUT"
