# Lua Object-Oriented Scripting Framework

CircleMUD now has a reusable Lua scripting template intended for gameplay systems while C remains authoritative for networking, persistence, security, and deterministic core simulation.

## Directory

```text
scripts/
├── core/
│   ├── oop.lua          # classes, inheritance, super calls
│   ├── event.lua        # event bus
│   ├── game.lua         # engine-facing Lua API
│   ├── class.lua        # gameplay class template
│   └── entity.lua       # entity/component template
├── examples/
│   ├── main.lua         # framework entry example
│   ├── npc.lua          # NPC class
│   ├── quest.lua        # quest class
│   ├── ability.lua      # ability class
│   └── zone.lua         # zone/event class
└── world/
    └── biomes.lua       # world data
```

## Class model

The framework uses Lua tables and metatables:

```text
Base Class
   │
   ├── Entity
   │    ├── NPC
   │    └── PlayerProxy
   │
   ├── Quest
   ├── Ability
   └── Zone
```

A class is created with:

```lua
local OOP = require("scripts.core.oop")
local Entity = OOP.class("Entity")
```

Inheritance:

```lua
local NPC = OOP.class("NPC", Entity)
```

Construction:

```lua
local guard = NPC:new(1001, "Guard", 5)
```

## Lifecycle methods

Gameplay classes should use predictable lifecycle methods:

- `initialize(...)`
- `on_spawn(...)`
- `on_load(...)`
- `on_update(delta_seconds)`
- `on_event(event)`
- `on_damage(context)`
- `on_death(context)`
- `on_destroy()`

The exact authoritative state transition remains in C.

## Event-driven programming

Use `EventBus` for script-to-script communication:

```lua
zone:on_player_enter(function(event)
  -- scripted behavior
end)

zone:player_enter(player_id)
```

Core events can be forwarded from C through `Game_Event()`.

## Gameplay templates

### NPC

NPC scripts can contain:

- personality
- dialogue selection
- patrol decisions
- encounter behavior
- faction reactions
- quest hooks
- scripted state machines

### Quest

Quest scripts can contain:

- objectives
- progress tracking
- prerequisites
- state transitions
- scripted events
- reward requests

### Ability

Ability scripts can contain:

- targeting rules
- cooldown metadata
- animation/event requests
- status-effect requests
- conditional behavior

Actual damage, resource spending, validation, and authoritative combat results should be performed by the C engine.

### Zone

Zone scripts can contain:

- player-enter events
- player-leave events
- spawn events
- environmental events
- biome behavior
- dynamic encounters
- world events

## Module pattern

Each script module should return a class or data object:

```lua
local OOP = require("scripts.core.oop")
local MySystem = OOP.class("MySystem")

function MySystem:initialize()
end

return MySystem
```

This keeps scripts testable and avoids unnecessary global variables.

## Security boundary

Lua must not receive unrestricted pointers into C memory. Future bindings should expose opaque IDs/handles and validated API calls.

Preferred:

```lua
Game.request_ability(ability_id, context)
```

Avoid exposing:

```lua
raw_c_pointer.some_internal_field = value
```

## Recommended script categories

```text
scripts/
├── core/
├── entities/
├── players/
├── npcs/
├── classes/
├── races/
├── abilities/
├── skills/
├── talents/
├── quests/
├── dialogue/
├── zones/
├── biomes/
├── encounters/
├── dungeons/
├── raids/
├── factions/
├── events/
├── audio/
└── tests/
```

## Engine integration roadmap

1. Load core Lua modules during engine initialization.
2. Create a restricted Lua state.
3. Register validated C APIs.
4. Load gameplay modules.
5. Dispatch engine events to Lua.
6. Execute script update hooks within a bounded time budget.
7. Convert Lua errors into engine diagnostics.
8. Add development-only hot reload.
9. Add Lua unit tests.
10. Add deterministic replay tests for script-driven gameplay.

## Design principle

Lua is the **gameplay extension language**. C is the **authoritative engine layer**. This separation allows rapid gameplay iteration without making security-sensitive or persistence-critical systems dependent on unrestricted scripting.
