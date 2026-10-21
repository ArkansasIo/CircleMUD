#!/bin/sh
set -eu
CC=${CC:-cc}
CFLAGS="-std=c99 -Wall -Wextra -pedantic -Isrc/rpg -Isrc"

$CC $CFLAGS \
  tests/rpg_runtime_test.c \
  src/rpg/rpg_level.c \
  src/rpg/rpg_world_difficulty.c \
  src/rpg/rpg_rng.c \
  src/rpg/rpg_loot.c \
  src/rpg/rpg_encounter.c \
  -lm -o /tmp/circlemud-rpg-tests
/tmp/circlemud-rpg-tests
rm -f /tmp/circlemud-rpg-tests

$CC $CFLAGS \
  tests/aetherworld_runtime_test.c \
  src/aetherworld.c \
  src/aetherworld_story.c \
  -o /tmp/aetherworld-tests
/tmp/aetherworld-tests
rm -f /tmp/aetherworld-tests
