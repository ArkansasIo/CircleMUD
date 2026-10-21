# C Runtime: Procedural MUD Instances

## Scope

The implementation in `src/aetherworld_instance.c` is a standalone C99 runtime module for seeded logical maps. It deliberately does not allocate or mutate CircleMUD `room_data`, `char_data`, descriptors, object lists, or VNUM indexes. That integration belongs in a later adapter that can validate mappings and clean them up safely.

## Public interface

See `src/aetherworld_instance.h`.

- `aw_instance_generate`: validates parameters and builds a deterministic connected logical room graph.
- `aw_instance_validate`: checks unique room IDs, bidirectional exits, reachable objective, room roles, and encounter/hazard budgets.
- `aw_instance_transition`: enforces allowed instance state changes.
- `aw_tower_run_init`, `aw_tower_complete_floor`, `aw_tower_fail`, `aw_tower_abandon`: provide a 1–100 floor progression state model and reject duplicate consecutive reward transaction IDs.

Supported logical types are dungeon, raid, trail/trial, and tower. Party matching, production reward persistence, room/VNUM mapping, live boss spawning, and save-file integration are not yet implemented by this module.

## Determinism and limits

All random choices use a local 32-bit PRNG state derived from the request. The graph is bounded by `AW_INSTANCE_MAX_ROOMS` and generation never edits live world state. A future generator version may intentionally change seeded layouts; persist that version with each run.

## Build and test

```sh
sh tests/run_aetherworld_instance_tests.sh
```

The test runner uses C99 with `-Wall -Wextra -Werror -pedantic`. It checks same-input determinism, input bounds, budget validation, broken exits, duplicate IDs, lifecycle transitions, and tower reward idempotency.

## Room/VNUM adapter contract

A future CircleMUD adapter should:
1. Reserve a unique instance ID and bounded range of runtime room identifiers.
2. Translate the logical graph into room descriptions and exits in a temporary allocation.
3. Validate all references and reverse exits before publishing.
4. Attach per-instance mobs, objects, encounters and reset rules.
5. Publish the instance atomically only after validation succeeds.
6. Track ownership for each allocated resource and tear down only instance-owned data.
7. Prevent stale exits, active combat, descriptors, and saved player references from outliving the instance.
8. Use server-authoritative progression and an idempotent persistent reward ledger.

Do not map dynamic instance rooms directly onto permanent world VNUMs without an explicit collision and persistence design.
