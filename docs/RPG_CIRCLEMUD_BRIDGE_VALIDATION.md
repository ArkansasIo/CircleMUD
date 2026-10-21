# CircleMUD RPG Bridge Validation

This checklist protects the boundary between the legacy CircleMUD server and the AetherWorld RPG runtime.

## Current heartbeat sequence

1. Initialize the RPG runtime once.
2. Synchronize live PCs into the RPG adapter.
3. Copy CircleMUD room/zone and inventory/equipment summary state into the RPG player snapshot.
4. Advance the RPG runtime by the fixed heartbeat delta.
5. Run CircleMUD's original pending-character extraction.

The adapter must never retain a pointer to `struct char_data` or `struct obj_data`. It stores the stable player ID and copies values into RPG-owned structures.

## Required invariants

- NPCs are never registered as RPG players.
- A player identifier less than zero is rejected.
- Repeated synchronization of the same player does not allocate duplicate slots.
- If a player disappears from `character_list`, its RPG registration is removed on the next bridge synchronization.
- Explicit extraction/logout paths can call `rpg_circlemud_on_character_extracted()`; the heartbeat sweep remains a defensive cleanup fallback.
- A full player pool causes registration to fail safely; it must not overwrite an active player's state.
- Room and zone IDs are copied as a snapshot; `NOWHERE` is represented as `-1`.
- Inventory count, equipped-object count, and carried weight are snapshots only. RPG code does not retain legacy object pointers or take ownership of CircleMUD objects.
- The legacy player-file structures remain unchanged. In particular, do not alter the layout of `char_file_u` or `player_special_data_saved` without an explicit compatibility migration.
- Synchronization copies the current legacy sheet; RPG simulation does not write combat results back into CircleMUD state until a controlled gameplay adapter and persistence regression tests are added.

## Regression test commands

Run the focused runtime suites from the repository root:

```sh
sh tests/run_rpg_tests.sh
sh tests/run_rpg_catalog_runtime_test.sh
```

The RPG suite now includes `tests/rpg_live_bridge_state_test.c`, covering idempotent player registration, world-state snapshots, validation of negative counts, and explicit unregister cleanup.

Run the legacy server build in a configured POSIX environment:

```sh
./configure
make -C src
```

## Integration boundary still to implement

The next bridge stage is object-level item translation. The current adapter intentionally stops at safe inventory/equipment summaries because `rpg_item` instances are RPG-owned and CircleMUD `obj_data` instances have their own lifecycle and persistence rules. A future adapter should use stable object identifiers/catalog mapping and explicit ownership rules before placing RPG item pointers into a character inventory.

NPC instances and RPG monster instances should likewise be synchronized through an isolated mapping layer rather than storing CircleMUD mob pointers.
