# CircleMUD RPG Bridge Validation

This checklist protects the boundary between the legacy CircleMUD server and the AetherWorld RPG runtime.

## Current heartbeat sequence

1. Initialize the RPG runtime once.
2. Synchronize linked non-player characters out of scope and live PCs into the RPG adapter.
3. Advance the RPG runtime by the fixed heartbeat delta.
4. Run CircleMUD's original pending-character extraction.

The adapter must never retain a pointer to `struct char_data`. It stores the stable player ID and copies values into RPG-owned structures.

## Required invariants

- NPCs are never registered as RPG players.
- A player identifier less than zero is rejected.
- Repeated synchronization of the same player does not allocate duplicate slots.
- If a player disappears from `character_list`, its RPG registration is removed on the next bridge synchronization.
- A full player pool causes registration to fail safely; it must not overwrite an active player's state.
- The legacy player-file structures remain unchanged. In particular, do not alter the layout of `char_file_u` or `player_special_data_saved` without an explicit compatibility migration.
- Synchronization copies the current legacy sheet; RPG simulation does not write combat results back into CircleMUD state until a controlled gameplay adapter and regression tests are added.

## Regression test commands

Run the focused runtime suites from the repository root:

```sh
sh tests/run_rpg_tests.sh
sh tests/run_rpg_catalog_runtime_test.sh
```

Run the legacy server build in a configured POSIX environment:

```sh
./configure
make -C src
```

## Integration boundary still to implement

Logout/extraction should call `rpg_circlemud_unsync_character()` as part of the player lifecycle. The heartbeat sweep is a defensive cleanup fallback, not a replacement for explicit lifecycle wiring.

Inventory, equipped objects, room/zone state, and NPC instances must be adapted in isolated functions. Avoid changing legacy save-file layouts or applying RPG combat, loot, or XP directly to a player until persistence and multiplayer consistency have dedicated tests.
