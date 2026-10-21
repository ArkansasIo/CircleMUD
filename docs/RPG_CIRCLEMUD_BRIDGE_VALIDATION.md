# CircleMUD RPG Bridge Validation

This checklist protects the boundary between the legacy CircleMUD server and the AetherWorld RPG runtime.

## Current heartbeat sequence

1. Initialize the RPG runtime once.
2. Synchronize live PCs into the RPG adapter.
3. Reconcile live NPC instances into RPG-owned monster slots.
4. Copy the current room/zone snapshot and RPG-owned item/equipment snapshot.
5. Advance the RPG runtime by the fixed heartbeat delta.
6. Run CircleMUD's original pending-character extraction.

The adapter must never retain a pointer to `struct char_data` or `struct obj_data`. It stores stable identifiers and copies values into RPG-owned structures.

## Required invariants

- NPCs are never registered as RPG players.
- A player identifier less than zero is rejected.
- Repeated synchronization of the same player does not allocate duplicate slots.
- If a player disappears from `character_list`, its RPG registration is removed on the next bridge synchronization.
- The explicit `rpg_circlemud_on_character_extracted()` lifecycle hook is safe to call more than once.
- A full player pool causes registration to fail safely; it must not overwrite an active player's state.
- Room and zone identifiers are copied as scalar values only; `NOWHERE` is represented as `-1`.
- Inventory and equipment are copied into fixed RPG-owned `rpg_item` storage; no CircleMUD object pointer is retained.
- CircleMUD object VNUMs map through the explicit VNUM table first, with the conservative identity mapping retained for generated catalog ranges.
- Unsupported/high VNUMs remain CircleMUD-only until an explicit content mapping is supplied.
- Two legacy ring, neck, or wrist positions may collapse into the single corresponding RPG slot; the first mapped item wins.
- NPC source keys are scalar reconciliation keys; no NPC pointer is retained by RPG state.
- NPC source keys are marked during each authoritative reconciliation epoch.
- NPC instances missing from a completed reconciliation epoch are removed from the RPG live monster pool.
- Re-seeing the same NPC source key updates the existing RPG monster rather than allocating a duplicate.
- RPG monster health, damage, armor, level, catalog index, and zone are copied as scalar values from CircleMUD.
- The generated monster catalog remains a deterministic fallback for prototype-to-catalog translation.
- Combat events are queued as RPG-owned value objects; the queue never mutates legacy CircleMUD state.
- Every combat event has a monotonically assigned non-zero sequence and must pass the explicit authorization gate before a future host adapter can apply it.
- Damage, death, XP, and loot events reject invalid player/monster identifiers and invalid numeric payloads.
- Loot events must reference a valid RPG catalog entry.
- Clearing the event queue does not mutate player, NPC, object, or save-file state.
- The legacy player-file structures remain unchanged. In particular, do not alter the layout of `char_file_u` or `player_special_data_saved` without an explicit compatibility migration.
- Synchronization copies the current legacy sheet; RPG simulation does not write combat results back to CircleMUD state until a controlled gameplay adapter and persistence regression tests are added.

## Regression test commands

Run the focused runtime suites from the repository root:

```sh
sh tests/run_rpg_tests.sh
sh tests/run_rpg_catalog_runtime_test.sh
```

The RPG suite includes the live player, item, monster-source, and combat-event bridge tests.

Run the legacy server build in a configured POSIX environment:

```sh
./configure
make -C src
```

## Item bridge policy

The item adapter uses a data-driven explicit VNUM table before falling back to identity mapping for generated RPG catalog ranges. This keeps unrelated legacy content playable while allowing imported worlds to map specific VNUMs to authoritative RPG definitions.

Extend `src/rpg/rpg_circlemud_item_map.c` with the actual world-file VNUM mappings for weapons, armor, consumables, materials, and unique quest objects.

## NPC bridge policy

CircleMUD's current `char_data` model does not provide a safe persistent per-instance NPC identifier for the adapter to retain. The live bridge therefore derives a scalar reconciliation key from the mob prototype RNUM/VNUM, room, and deterministic occurrence ordinal. This prevents duplicate RPG instances during normal heartbeat passes without retaining a `char_data` pointer.

The derived key is deliberately an adapter boundary, not a persistence identity. A future host integration can replace it with an explicit stable NPC instance ID without changing the RPG monster API.

## Combat event policy

`rpg_circlemud_combat_bridge` is deliberately one-way and transactional at this stage. RPG combat can produce value events, but those events are not automatically applied to `char_data`, `obj_data`, player files, or CircleMUD combat state.

The intended future flow is:

```text
RPG Combat
    |
    v
RPG Event Queue
    |
    v
Host Authorization
    |
    +---- rejected --> discard/log
    |
    +---- accepted --> CircleMUD mutation adapter
                              |
                              +--> damage/death
                              +--> XP
                              +--> loot
                              +--> persistence
```

This prevents a partially integrated RPG calculation from corrupting the legacy simulation. A later mutation adapter must validate that the player and NPC still exist, that the source identity is current, and that the event sequence has not already been consumed.

## Integration boundary still to implement

The explicit extraction hook is available through `rpg_circlemud_on_character_extracted()`. It should be called from the server's authoritative extraction path when the host build is wired for the RPG bridge; the heartbeat sweep remains a defensive cleanup fallback.

The next stage is an explicit **combat event consumer** with replay protection, target validation, death-state arbitration, loot materialization, XP accounting, and save scheduling. Do not write RPG combat results directly into legacy character state until those contracts have dedicated multiplayer and save-file regression tests.
