# Game Engine Source Update Notes

## Update 2026-10-21

The CircleMUD repository now contains a separate game-engine layer intended to grow around the existing C server rather than replacing the legacy server architecture.

## Source layout

```text
src/gameengine/
├── game_types.h
├── game_engine.h
├── game_engine.c
├── game_object.h
├── game_object.c
├── game_world.h
├── game_world.c
├── game_script.h
└── game_script.c

scripts/
├── core/
│   ├── game.lua
│   ├── class.lua
│   └── entity.lua
└── world/
    └── biomes.lua
```

## Build integration

`patches/0002-game-engine-core.patch` documents the build integration for the dependency-free C engine core.

The initial build patch intentionally integrates:

- `game_engine.c`
- `game_object.c`
- `game_world.c`

The Lua bridge is kept optional because linking it requires a configured Lua development library. This prevents a normal CircleMUD build from unexpectedly acquiring a new external linker dependency.

## Engine update pipeline

```text
CircleMUD network/server loop
        |
        v
GameEngine_Update()
        |
        +--> scheduler/tick systems
        +--> world systems
        +--> entity systems
        +--> combat systems
        +--> quest systems
        +--> audio/event systems
        +--> Lua scripting (optional)
```

## Object model

The engine uses an object-oriented design implemented in C through structs, function pointers, lifecycle callbacks, and opaque user data.

```text
GameObject
  |-- identity
  |-- state
  |-- lifecycle callbacks
  |-- update callback
  |-- event callback
  `-- user data
```

Lua extends this model with tables and metatables.

## World model

World content is intended to follow:

```text
World
  -> Continent
    -> Region
      -> Biome
        -> SubBiome
          -> Zone
            -> SubZone
              -> Landmark
                -> Encounter
```

## Update-system rules

1. The legacy CircleMUD loop remains authoritative for networking.
2. Game-engine systems must not directly mutate unrelated CircleMUD globals.
3. Systems should communicate through explicit APIs/events.
4. Gameplay state must remain deterministic where practical.
5. Lua scripts are gameplay extensions, not trusted replacements for security-sensitive C code.
6. World, quest, combat, class, race, and audio definitions should remain data-driven.
7. New engine modules must be independently testable.

## Planned systems

### Core

- [x] Engine lifecycle
- [x] Game object abstraction
- [x] World abstraction
- [ ] System registry
- [ ] Event bus
- [ ] Fixed timestep scheduler
- [ ] Deterministic RNG service
- [ ] Save/load serialization

### Gameplay

- [ ] Entity/component system
- [ ] Character runtime profile
- [ ] Race/class registry integration
- [ ] Ability system
- [ ] Skill system
- [ ] Talent system
- [ ] Equipment system
- [ ] Combat system
- [ ] Quest runtime
- [ ] NPC AI
- [ ] Party/group system
- [ ] Guild system

### World

- [ ] Biome registry
- [ ] Sub-biome registry
- [ ] Zone registry
- [ ] Sub-zone registry
- [ ] Dungeon registry
- [ ] Raid registry
- [ ] Encounter director
- [ ] Weather integration
- [ ] Day/night cycle

### D&D rules

- [ ] d20 check API
- [ ] Ability modifiers
- [ ] Proficiency
- [ ] Advantage/disadvantage
- [ ] Saving throws
- [ ] Skill checks
- [ ] Armor class
- [ ] Attack rolls
- [ ] Damage rolls
- [ ] Conditions
- [ ] Rest/recovery
- [ ] Spell/ability resolution
- [ ] Encounter difficulty

### Lua

- [ ] Optional Lua runtime initialization
- [ ] Script loader
- [ ] Safe script execution boundary
- [ ] Entity bindings
- [ ] Character bindings
- [ ] Room/zone bindings
- [ ] Quest bindings
- [ ] Ability bindings
- [ ] Event bindings
- [ ] Script sandbox policy
- [ ] Script error reporting
- [ ] Hot reload for development

## Patch workflow

The patch format follows the traditional unified-diff workflow documented by CircleMUD's porting documentation. A source tree can therefore maintain both direct Git commits and portable patch files. citeturn0search12

Example:

```sh
patch -p1 < patches/0002-game-engine-core.patch
```

After applying a patch:

```sh
./configure
cd src
make clean
make
```

## Validation requirements

Every new engine system should provide:

- compile validation
- initialization/shutdown validation
- invalid-ID validation
- boundary-value tests
- deterministic behavior tests
- failure-path tests
- memory-safety testing
- regression coverage

The upstream CircleMUD source uses a generated `Makefile.in` as part of its traditional build architecture, so build changes should remain compatible with that workflow. citeturn0search0turn0search12

## Compatibility policy

The game-engine layer is additive. Existing CircleMUD commands, zones, mobiles, objects, and networking should continue to work while the new systems are introduced incrementally.

The long-term objective is:

```text
Legacy CircleMUD
      |
      +-------------------+
      |                   |
      v                   v
Legacy gameplay      New Game Engine
                          |
             +------------+------------+
             |            |            |
             v            v            v
          C Systems     Lua      Data Registries
```

## Next patch

The next patch should integrate the scheduler/event bus and expose an explicit `GameEngine_Tick()` call from the server update loop. Lua should be linked only behind a configure/build option such as `--enable-lua` after the repository has a reproducible Lua dependency check.
