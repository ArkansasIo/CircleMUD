# Endgame Content Systems

## Overview

CircleMUD now defines four reusable endgame content families:

1. **Dungeons** — instanced or world-based PvE encounters with bosses, loot, checkpoints, and difficulty tiers.
2. **Trails** — progressive challenge routes connecting encounters, hazards, traversal objectives, and optional branches.
3. **Raids** — coordinated multi-player encounters with wings, bosses, mechanics, lockouts, loot distribution, and difficulty modes.
4. **Tower** — a 100-floor escalating challenge with floor modifiers, elite encounters, bosses, checkpoints, and milestone rewards.

All four systems are data-driven and can be consumed by the application API, Lua scripting layer, HUD, map, quest system, and future MySQL persistence layer.

## Content hierarchy

```text
World
└── Region
    └── Zone
        ├── Dungeon
        │   ├── Entrance
        │   ├── Wings
        │   ├── Encounters
        │   └── Final Boss
        ├── Trail
        │   ├── Route
        │   ├── Challenges
        │   └── Trail Boss
        ├── Raid
        │   ├── Wing
        │   ├── Boss
        │   └── Final Encounter
        └── Tower
            ├── Floors 1-100
            ├── Checkpoints
            ├── Floor Bosses
            └── Tower Guardian
```

## Dungeon system

Each dungeon definition contains:

- ID and display name
- recommended level
- minimum party size
- maximum party size
- difficulty modes
- entrance location
- biome/theme
- encounter list
- boss list
- checkpoint policy
- completion state
- loot tables
- quest hooks
- achievement hooks

### Dungeon modes

- Normal
- Veteran
- Mythic
- Mythic+
- Challenge

### Core lifecycle

```text
AVAILABLE
  -> ENTERING
  -> ACTIVE
  -> CHECKPOINT
  -> BOSS
  -> COMPLETE
  -> RESET
```

## Trail system

Trails are persistent multi-stage routes rather than simple rooms. A trail can contain:

- combat encounters
- traversal sections
- survival sections
- puzzles
- resource gathering
- escort objectives
- elite encounters
- hidden branches
- optional bosses
- timed challenges

Trail completion tracks the highest stage reached and the best completion time.

### Trail grades

```text
Bronze -> Silver -> Gold -> Platinum -> Mythic
```

Grades can drive rewards without changing the underlying content definition.

## Raid system

Raids support coordinated groups and multi-boss progression.

### Raid features

- 10/20/40-player profiles
- normal/heroic/mythic difficulty
- wings
- boss progression
- encounter phases
- wipe/reset handling
- role checks
- loot rules
- weekly lockouts
- achievement tracking
- raid-wide story flags
- final-boss rewards

### Raid state

```text
FORMING
  -> ENTERING
  -> WING_ACTIVE
  -> BOSS_ACTIVE
  -> BOSS_DEFEATED
  -> WING_COMPLETE
  -> RAID_COMPLETE
```

## 100-floor tower

The tower is a single persistent challenge family containing exactly **100 floors**.

### Floor bands

| Floors | Tier | Design focus |
|---|---|---|
| 1-10 | Initiate | basic combat and teaching |
| 11-20 | Vanguard | mixed encounters |
| 21-30 | Veteran | elites and environmental hazards |
| 31-40 | Champion | coordinated mechanics |
| 41-50 | Heroic | advanced builds and resist checks |
| 51-60 | Mythic | strong modifiers |
| 61-70 | Ascendant | elite chains and mini-bosses |
| 71-80 | Paragon | complex encounter combinations |
| 81-90 | Apex | high-pressure boss mechanics |
| 91-99 | Transcendent | endgame trials |
| 100 | Tower Crown | final guardian |

### Floor cycle

Every floor can generate or select:

1. entrance state
2. encounter package
3. environmental modifier
4. optional objective
5. reward chest
6. exit/checkpoint

Every 10th floor is a milestone floor. Floors 10, 20, 30, 40, 50, 60, 70, 80, and 90 contain milestone encounters. Floor 100 is the final Tower Crown encounter.

### Tower modifiers

Examples:

- Reinforced Enemies
- Reduced Healing
- Arcane Surge
- Burning Ground
- Frozen Ground
- Vampiric Enemies
- Swarming Enemies
- Time Pressure
- No Respawn
- Elite Reinforcements

Modifiers are data-driven and can be combined subject to difficulty rules.

## Scaling

Recommended deterministic scaling:

```text
level_scale = 1.0 + (content_level - base_level) * 0.08
floor_scale = 1.0 + (floor - 1) * 0.025
boss_scale = 1.35
elite_scale = 1.20

final_health = base_health * level_scale * floor_scale * encounter_scale
final_damage = base_damage * level_scale * floor_scale * encounter_scale
```

The production implementation should clamp scaling according to the active difficulty profile.

## Shared encounter interface

```c
struct content_instance {
    int id;
    int definition_id;
    int difficulty;
    int state;
    int current_stage;
    int current_floor;
    int party_size;
    int deaths;
    int elapsed_seconds;
};
```

Shared operations:

```text
create_instance()
enter_instance()
start_encounter()
complete_objective()
defeat_boss()
advance_stage()
advance_floor()
activate_checkpoint()
complete_instance()
reset_instance()
```

## Lua integration

Content definitions should expose OOP classes such as:

```lua
local OOP = require("scripts.core.oop")

local Tower = OOP.class("Tower")

function Tower:initialize(id, max_floor)
    self.id = id
    self.max_floor = max_floor or 100
    self.current_floor = 1
end

function Tower:advance()
    if self.current_floor < self.max_floor then
        self.current_floor = self.current_floor + 1
    end
    return self.current_floor
end

return Tower
```

Lua remains a gameplay scripting layer. Authoritative state, persistence, and security-sensitive operations remain in the C engine.

## API integration

Recommended endpoints:

```text
GET  /api/v1/content/dungeons
GET  /api/v1/content/dungeons/{id}
POST /api/v1/content/dungeons/{id}/enter
GET  /api/v1/content/trails
GET  /api/v1/content/trails/{id}
POST /api/v1/content/trails/{id}/start
GET  /api/v1/content/raids
GET  /api/v1/content/raids/{id}
POST /api/v1/content/raids/{id}/enter
GET  /api/v1/content/tower
GET  /api/v1/content/tower/floors/{floor}
POST /api/v1/content/tower/enter
POST /api/v1/content/tower/advance
```

## Persistence

Persist at minimum:

- player/content instance membership
- dungeon completion
- dungeon difficulty
- trail progress
- trail best grade/time
- raid lockouts
- raid boss kills
- tower highest floor
- tower checkpoint
- tower run statistics
- rewards claimed
- achievement state

## Security and authority

The client never determines whether a floor, boss, reward, or completion is valid. Client requests are commands. The C engine validates the instance state and emits authoritative events before persistence or reward distribution.
