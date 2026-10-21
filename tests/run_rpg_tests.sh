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
  tests/rpg_quest_runtime_test.c \
  src/rpg/rpg_quest.c \
  src/rpg/rpg_quest_runtime.c \
  -o /tmp/circlemud-rpg-quest-tests
/tmp/circlemud-rpg-quest-tests
rm -f /tmp/circlemud-rpg-quest-tests

$CC $CFLAGS \
  tests/rpg_item_value_test.c \
  src/rpg/rpg_item.c \
  src/rpg/rpg_catalog.c \
  src/rpg/rpg_value.c \
  -lm -o /tmp/circlemud-rpg-item-value-tests
/tmp/circlemud-rpg-item-value-tests
rm -f /tmp/circlemud-rpg-item-value-tests

$CC $CFLAGS \
  tests/rpg_equipment_value_test.c \
  src/rpg/rpg_item.c \
  src/rpg/rpg_catalog.c \
  src/rpg/rpg_equipment.c \
  src/rpg/rpg_value.c \
  -lm -o /tmp/circlemud-rpg-equipment-value-tests
/tmp/circlemud-rpg-equipment-value-tests
rm -f /tmp/circlemud-rpg-equipment-value-tests

$CC $CFLAGS \
  tests/aetherworld_runtime_test.c \
  src/aetherworld.c \
  src/aetherworld_story.c \
  -o /tmp/aetherworld-tests
/tmp/aetherworld-tests
rm -f /tmp/aetherworld-tests
