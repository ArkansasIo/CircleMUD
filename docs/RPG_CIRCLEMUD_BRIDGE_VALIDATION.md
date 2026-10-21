# CircleMUD RPG Bridge Validation

This checklist protects the boundary between the legacy CircleMUD server and the AetherWorld RPG runtime.

## Current heartbeat sequence

1. Initialize the RPG runtime once.
2. Synchronize live PCs into the RPG adapter.
3. Copy the current room/zone snapshot and RPG-owned item/equipment snapshot.
4. Advance the RPG runtime by the fixed heartbeat delta.
5. Run CircleMUD's original pending-character extraction.

The adapter must never retain a pointer to `struct char_data` or `struct obj_data`. It stores stable player identifiers and copies values into RPG-owned structures.

## Required invariants

- NPCs are never registered as RPG players.
- A player identifier less than zero is rejected.
- Repeated synchronization of the same player does not allocate duplicate slots.
- If a player disappears from `character_list`, its RPG registration is removed on the next bridge synchronization.
- The explicit `rpg_circlemud_on_character_extracted()` lifecycle hook is safe to call more than once.
- A full player pool causes registration to fail safely; it must not overwrite an active player's state.
- Room and zone identifiers are copied as scalar values only; `NOWHERE` is represented as `-1`.
- Inventory and equipment are copied into fixed RPG-owned `rpg_item` storage; no CircleMUD object pointer is retained.
- CircleMUD object VNUMs map directly to RPG catalog indices only when the VNUM is in `[0, RPG_CATALOG_TOTAL)`.
- Unsupported/high VNUMs remain CircleMUD-only until an explicit content mapping table is introduced.
- Two legacy ring, neck, or wrist positions may collapse into the single corresponding RPG slot; the first mapped item wins.
- The legacy player-file structures remain unchanged. In particular, do not alter the layout of `char_file_u` or `player_special_data_saved` without an explicit compatibility migration.
- Synchronization copies the current legacy sheet; RPG simulation does not write combat results back into CircleMUD state until a controlled gameplay adapter and persistence regression tests are added.

## Regression test commands

Run the focused runtime suites from the repository root:

```sh
sh tests/run_rpg_tests.sh
sh tests/run_rpg_catalog_runtime_test.sh
```

The RPG suite now includes `tests/rpg_live_bridge_state_test.c` and `tests/rpg_live_item_bridge_test.c`, covering world snapshots, item ownership boundaries, equipment-slot state, validation, and unregister cleanup.

Run the legacy server build in a configured POSIX environment:

```sh
./configure
make -C src
```

## Item bridge policy

The item adapter intentionally uses a conservative identity mapping instead of guessing how arbitrary CircleMUD VNUM ranges correspond to the generated RPG catalog. This keeps unrelated legacy content playable and prevents silently assigning incorrect RPG stats to objects.

The next content-pipeline step is a data-driven VNUM mapping table that can explicitly map legacy object VNUMs to RPG catalog entries, including weapons, armor, consumables, materials, and unique quest objects.

## Integration boundary still to implement

The explicit extraction hook is available through `rpg_circlemud_on_character_extracted()`. It should be called from the server's authoritative extraction path when the host build is wired for the RPG bridge; the heartbeat sweep remains a defensive cleanup fallback.

NPC instances still need an isolated mob-to-RPG-monster adapter. Avoid changing legacy save-file layouts or applying RPG combat, loot, or XP directly to a player until persistence and multiplayer consistency have dedicated tests.
