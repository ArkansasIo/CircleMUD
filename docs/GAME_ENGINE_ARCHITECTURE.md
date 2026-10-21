# CircleMUD Game Engine Architecture

The project now has an engine layer on top of the legacy MUD loop. CircleMUD remains the authoritative server/runtime, while `src/gameengine/` provides reusable engine services and `scripts/` provides optional Lua gameplay scripting.

## Architecture

```text
CircleMUD server
  |
  +-- Game Engine Core
  |    +-- fixed/update loop
  |    +-- event bus
  |    +-- system registry
  |    +-- object/component model
  |    +-- world/zone registry
  |
  +-- Gameplay Systems
  |    +-- RPG
  |    +-- quests
  |    +-- combat
  |    +-- D&D rules
  |    +-- AI
  |    +-- audio/event metadata
  |
  +-- Lua Runtime
       +-- game scripts
       +-- entities
       +-- classes
       +-- abilities
       +-- biome/zone data
       +-- quest scripts
```

## C API

`game_engine.h/.c` implements:

- engine initialization/shutdown
- fixed-step updates
- system registration
- event registration and dispatch
- tick/time tracking

`game_object.h/.c` implements an object-oriented C interface using objects, callbacks, userdata, and event handlers. This allows gameplay objects to expose polymorphic behavior without requiring a C++ rewrite of CircleMUD.

`game_world.h/.c` implements biome/zone/subzone runtime registries.

## Lua

Lua 5.4 is an optional embedded scripting layer. The engine is still capable of compiling without Lua when `GAMEENGINE_WITH_LUA` is not defined. When enabled, the host creates a Lua state, opens the standard libraries, loads scripts, and exposes engine events to Lua.

Lua uses tables/metatables for object-oriented programming. The repository includes reusable `Class` and `Entity` examples.

## Script boundary

C remains authoritative for:

- network state
- persistence
- memory ownership
- security-sensitive validation
- deterministic combat calculations
- database operations

Lua is intended for:

- quests
- NPC behavior
- scripted encounters
- abilities and effects
- world events
- dialogue
- zone events
- prototypes and rapid gameplay iteration

Scripts must not directly mutate raw C memory. Future bindings should expose validated handles and API functions.

## Recommended next engine modules

```text
src/gameengine/
  game_engine.c/.h
  game_types.h
  game_object.c/.h
  game_world.c/.h
  game_script.c/.h
  game_entity.c/.h
  game_component.c/.h
  game_event.c/.h
  game_scheduler.c/.h
  game_save.c/.h
  game_rng.c/.h
  game_combat.c/.h
  game_ability.c/.h
  game_quest.c/.h
  game_ai.c/.h
```

The current implementation intentionally starts with the smallest stable core so the legacy CircleMUD server can adopt the engine incrementally instead of replacing the proven network/game loop in one step.
