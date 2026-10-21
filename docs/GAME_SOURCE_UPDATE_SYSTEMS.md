# Game Source Update Systems

This document is the implementation checklist for continuing the new game-engine source layer.

## System 01 — Engine lifecycle

```text
GameEngine_Init()
GameEngine_Update(delta)
GameEngine_Shutdown()
```

Responsibilities:

- initialize registries
- initialize scheduler
- initialize event bus
- initialize world state
- initialize optional scripting
- release resources in reverse order

## System 02 — Scheduler

The scheduler should support:

- one-shot events
- repeating events
- delayed callbacks
- priority ordering
- cancellation
- tick ownership

The scheduler must not assume that one CircleMUD tick equals one real-world second.

## System 03 — Event bus

Recommended events:

```text
GAME_START
GAME_TICK
ENTITY_CREATE
ENTITY_DESTROY
PLAYER_LOGIN
PLAYER_LOGOUT
ENTER_ZONE
LEAVE_ZONE
COMBAT_START
COMBAT_END
ABILITY_CAST
DAMAGE
HEAL
QUEST_ACCEPT
QUEST_COMPLETE
LEVEL_UP
WEATHER_CHANGE
DAY_PHASE_CHANGE
```

## System 04 — Entity/component layer

The engine should progressively move from inheritance-heavy gameplay code toward composable components.

```text
Entity
 + Transform
 + Stats
 + Combat
 + Inventory
 + Equipment
 + QuestState
 + AI
 + Audio
 + Script
```

C remains responsible for authoritative state and lifecycle.

## System 05 — World runtime

Runtime world objects should reference data definitions rather than duplicate static definitions.

```text
WorldDefinition
  -> RegionDefinition
    -> BiomeDefinition
      -> ZoneDefinition
        -> RuntimeZone
```

## System 06 — Combat

Combat should be a pipeline:

```text
Input
 -> Intent
 -> Ability
 -> Targeting
 -> Attack/Save
 -> Damage
 -> Resistances
 -> Conditions
 -> Threat
 -> Combat Event
 -> Audio Event
```

The D&D rules layer provides the resolution primitives while MMORPG-specific systems provide cooldowns, resources, threat, encounter scripting, and scaling.

## System 07 — Quest runtime

Quest execution should consume events instead of polling whenever possible.

```text
Event
  -> Objective matcher
  -> Quest state
  -> Reward resolver
  -> Story state
```

## System 08 — Audio event system

Gameplay systems emit semantic events:

```text
AUDIO_UI_CONFIRM
AUDIO_UI_ERROR
AUDIO_ZONE_ENTER
AUDIO_COMBAT_START
AUDIO_COMBAT_END
AUDIO_ABILITY_CAST
AUDIO_CRITICAL_HIT
AUDIO_LEVEL_UP
AUDIO_QUEST_ACCEPT
AUDIO_QUEST_COMPLETE
AUDIO_BOSS_INTRO
```

The audio layer resolves these IDs into music, ambient sound, and sound effects.

## System 09 — Lua scripting

Lua scripts should interact with a controlled engine API rather than arbitrary C memory.

```text
Lua
 |
 +-- game
 +-- world
 +-- entity
 +-- character
 +-- quest
 +-- combat
 +-- ability
 +-- audio
 `-- events
```

Scripts should never receive raw pointers or unrestricted filesystem/network access.

## System 10 — Persistence

The runtime should eventually separate:

```text
Static definitions
    !=
Player state
    !=
Runtime transient state
```

Static definitions belong in registries/data files. Player state belongs in persistence. Runtime objects can be rebuilt after a restart.

## System 11 — Diagnostics

Add engine diagnostics for:

- subsystem initialization failures
- script errors
- invalid entity IDs
- invalid zone IDs
- missing definitions
- event-handler failures
- scheduler overflow
- unexpected state transitions
- resource leaks

## System 12 — Test strategy

The engine should be tested in layers:

```text
Unit tests
   -> subsystem tests
      -> integration tests
         -> server tests
            -> gameplay regression tests
```

## Source-file policy

Every new `.c` file should have a matching public header when the API is externally consumed. Internal-only implementation helpers should remain private to their module.

Use explicit ownership rules for allocated objects and document who creates, updates, and destroys every runtime object.

## Current status

The repository currently contains the initial engine/object/world/script source layer and documentation. The next implementation increment is the scheduler + event bus + authoritative engine tick, followed by controlled Lua bindings.
