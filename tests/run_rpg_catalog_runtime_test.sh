#!/bin/sh
set -eu

CC=${CC:-cc}
CFLAGS="-std=c99 -Wall -Wextra -pedantic -Isrc/rpg"
OUT=${TMPDIR:-/tmp}/circlemud-rpg-catalog-tests
RNG_OBJ=${TMPDIR:-/tmp}/circlemud-rpg-rng.o

$CC $CFLAGS -c src/rpg/rpg_rng.c -o "$RNG_OBJ"
$CC $CFLAGS \
  tests/rpg_catalog_runtime_test.c \
  src/rpg/rpg_catalog.c src/rpg/rpg_level.c src/rpg/rpg_loot.c \
  src/rpg/rpg_item.c src/rpg/rpg_equipment.c \
  src/rpg/rpg_character.c src/rpg/rpg_world_difficulty.c \
  src/rpg/rpg_encounter.c src/rpg/rpg_monster.c src/rpg/rpg_combat.c \
  src/rpg/rpg_crafting.c src/rpg/rpg_value.c "$RNG_OBJ" \
  -lm -o "$OUT"

"$OUT"
rm -f "$OUT" "$RNG_OBJ"
