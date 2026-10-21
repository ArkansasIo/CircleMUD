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
  src/rpg/rpg_value.c \
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
  tests/rpg_character_value_test.c \
  src/rpg/rpg_character.c \
  src/rpg/rpg_character_value.c \
  src/rpg/rpg_equipment.c \
  src/rpg/rpg_item.c \
  src/rpg/rpg_catalog.c \
  src/rpg/rpg_level.c \
  src/rpg/rpg_value.c \
  -lm -o /tmp/circlemud-rpg-character-value-tests
/tmp/circlemud-rpg-character-value-tests
rm -f /tmp/circlemud-rpg-character-value-tests

$CC $CFLAGS \
  tests/rpg_combat_value_test.c \
  src/rpg/rpg_character.c \
  src/rpg/rpg_character_value.c \
  src/rpg/rpg_level.c \
  src/rpg/rpg_item.c \
  src/rpg/rpg_catalog.c \
  src/rpg/rpg_equipment.c \
  src/rpg/rpg_value.c \
  src/rpg/rpg_monster.c \
  src/rpg/rpg_encounter.c \
  src/rpg/rpg_world_difficulty.c \
  src/rpg/rpg_rng.c \
  src/rpg/rpg_combat.c \
  -lm -o /tmp/circlemud-rpg-combat-value-tests
/tmp/circlemud-rpg-combat-value-tests
rm -f /tmp/circlemud-rpg-combat-value-tests

$CC $CFLAGS \
  tests/rpg_encounter_budget_test.c \
  src/rpg/rpg_encounter.c \
  src/rpg/rpg_level.c \
  src/rpg/rpg_world_difficulty.c \
  src/rpg/rpg_value.c \
  -lm -o /tmp/circlemud-rpg-encounter-budget-tests
/tmp/circlemud-rpg-encounter-budget-tests
rm -f /tmp/circlemud-rpg-encounter-budget-tests

$CC $CFLAGS \
  tests/rpg_runtime_tick_test.c \
  src/rpg/rpg_runtime_tick.c \
  src/rpg/rpg_live.c \
  src/rpg/rpg_system.c \
  src/rpg/rpg_world.c \
  src/rpg/rpg_character.c \
  src/rpg/rpg_level.c \
  src/rpg/rpg_equipment.c \
  src/rpg/rpg_item.c \
  src/rpg/rpg_catalog.c \
  src/rpg/rpg_character_value.c \
  src/rpg/rpg_value.c \
  src/rpg/rpg_monster.c \
  src/rpg/rpg_encounter.c \
  src/rpg/rpg_world_difficulty.c \
  src/rpg/rpg_rng.c \
  src/rpg/rpg_combat.c \
  src/rpg/rpg_loot_drop.c \
  src/rpg/rpg_loot.c \
  -lm -o /tmp/circlemud-rpg-runtime-tick-tests
/tmp/circlemud-rpg-runtime-tick-tests
rm -f /tmp/circlemud-rpg-runtime-tick-tests

$CC $CFLAGS \
  tests/rpg_heartbeat_hook_test.c \
  src/rpg/rpg_heartbeat_hook.c \
  src/rpg/rpg_runtime_tick.c \
  src/rpg/rpg_live.c \
  src/rpg/rpg_system.c \
  src/rpg/rpg_world.c \
  src/rpg/rpg_character.c \
  src/rpg/rpg_level.c \
  src/rpg/rpg_equipment.c \
  src/rpg/rpg_item.c \
  src/rpg/rpg_catalog.c \
  src/rpg/rpg_character_value.c \
  src/rpg/rpg_value.c \
  src/rpg/rpg_monster.c \
  src/rpg/rpg_encounter.c \
  src/rpg/rpg_world_difficulty.c \
  src/rpg/rpg_rng.c \
  src/rpg/rpg_combat.c \
  src/rpg/rpg_loot_drop.c \
  src/rpg/rpg_loot.c \
  -lm -o /tmp/circlemud-rpg-heartbeat-hook-tests
/tmp/circlemud-rpg-heartbeat-hook-tests
rm -f /tmp/circlemud-rpg-heartbeat-hook-tests

$CC $CFLAGS \
  tests/rpg_live_test.c \
  src/rpg/rpg_live.c \
  src/rpg/rpg_character.c \
  src/rpg/rpg_level.c \
  src/rpg/rpg_equipment.c \
  src/rpg/rpg_item.c \
  src/rpg/rpg_catalog.c \
  src/rpg/rpg_character_value.c \
  src/rpg/rpg_value.c \
  src/rpg/rpg_monster.c \
  src/rpg/rpg_encounter.c \
  src/rpg/rpg_world_difficulty.c \
  src/rpg/rpg_rng.c \
  src/rpg/rpg_combat.c \
  src/rpg/rpg_loot_drop.c \
  src/rpg/rpg_loot.c \
  -lm -o /tmp/circlemud-rpg-live-tests
/tmp/circlemud-rpg-live-tests
rm -f /tmp/circlemud-rpg-live-tests

$CC $CFLAGS \
  tests/aetherworld_runtime_test.c \
  src/aetherworld.c \
  src/aetherworld_story.c \
  -o /tmp/aetherworld-tests
/tmp/aetherworld-tests
rm -f /tmp/aetherworld-tests
