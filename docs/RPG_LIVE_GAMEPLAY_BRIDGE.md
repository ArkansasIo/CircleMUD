# RPG Live Gameplay Bridge

The live RPG bridge is the runtime boundary between CircleMUD's authoritative heartbeat and the data-driven RPG simulation.

## Heartbeat path

```text
CircleMUD heartbeat
      |
      v
circlemud_rpg_extract_pending_chars()
      |
      +--> rpg_runtime_tick(0.1)
      |       |
      |       +--> rpg_system_update()
      |       +--> rpg_live_update()
      |
      +--> original extract_pending_chars()
```

The legacy `src/comm.c` implementation remains unchanged. The build redirects the existing `extract_pending_chars()` call only for `comm.o`, so RPG simulation time advances on the same authoritative heartbeat path.

## Live state

`rpg_live_state` currently provides fixed-capacity runtime storage for:

- 128 active RPG player adapters.
- 256 active RPG monsters.
- Player combat cooldowns, kill counters, and loot-event counters.
- Character state backed by the existing level, equipment, and valuation systems.
- Monster state backed by catalog, encounter, difficulty, and loot systems.

## Player lifecycle

Use `rpg_live_register_player(player_id, level)` when a CircleMUD character becomes an RPG participant. The bridge creates a `rpg_character` and initializes its derived stats and equipment state.

Use `rpg_live_get_character(player_id)` for the authoritative RPG character object and `rpg_live_unregister_player(player_id)` when the participant leaves the RPG runtime.

## Monster lifecycle

`rpg_live_spawn_monster(catalog_index, zone_id, difficulty)` allocates an active monster from the shared 256-slot pool. Monster initialization uses the existing encounter and world-difficulty scaling rules.

## Combat and loot

`rpg_live_attack()` routes player attacks through `rpg_apply_attack()`. The existing combat pipeline therefore remains responsible for damage, critical strikes, mitigation, and monster death.

When a monster dies, the player receives its configured experience reward. `rpg_live_collect_loot()` then routes the defeated monster through the existing deterministic loot-drop implementation.

## Scope boundary

This bridge is intentionally not a replacement for CircleMUD's `char_data`, object lists, room graph, or persistence format. It is the RPG runtime adapter. The next integration layer should map CircleMUD player/object/room lifecycle events onto these APIs rather than duplicating the legacy world model.

## Testing

`tests/rpg_live_test.c` verifies:

1. RPG runtime initialization.
2. Player registration and character creation.
3. Monster spawning.
4. Combat until monster defeat.
5. Experience/kill accounting.
6. Loot collection.
7. Player unregistering.

The same live runtime is linked into the heartbeat and runtime-tick regression tests so the heartbeat path cannot silently regress to an unlinked RPG runtime.
