# Procedural dungeon and instance generation

`generator.lua` returns deterministic, data-only room layouts for dungeons,
raids, trails, and towers. It uses a seeded random stream and Binary Space
Partitioning (BSP) to place non-overlapping rectangular rooms. A minimum
spanning tree connects the rooms, then configurable extra edges and optional
secret rooms add alternate routes. A grid breadth-first search routes corridors
around unrelated room interiors. Choose `layout = "cave"` to create an organic
cellular-automata floor map and carve corridors through it; the default `bsp`
layout leaves the space outside rooms as empty map.

```lua
local World = require("scripts.core.world")
local world = World:new()

local dungeon, err = world:generate_instance("crypt-847291552", {
  kind = "dungeon",
  seed = 847291552,
  biome = "crypt",
  theme = "ancient_crypt",
  layout = "cave",
  difficulty = 7,
  map_width = 96,
  map_height = 96,
  min_rooms = 20,
  max_rooms = 40,
  loop_chance = 0.12,
  secret_room_chance = 0.04,
  enemy_budget = 250000,
  loot_budget = 50000
})
assert(dungeon, err)
```

## Parameters

- `kind`: `dungeon`, `raid`, `trail`, or `tower`.
- `seed`: deterministic seed. The same seed and parameters produce the same layout.
- `floors`: tower floor count, clamped to 1 through 100.
- `layout`: `bsp` for room-and-corridor floors or `cave` for cellular-automata terrain.
- `max_attempts`: regeneration limit after validation failures, clamped to 1 through 20 (default 20).
- `difficulty`: tier 1 through 9. Default room ranges scale from 10–20 rooms at tier 1 to 40–80 at tier 9.
- `map_width`, `map_height`: map dimensions, clamped to 24 through 160.
- `min_rooms`, `max_rooms`: BSP room count bounds. `rooms` and `rooms_per_floor` are accepted as fixed-count shortcuts.
- `min_room_width`, `max_room_width`, `min_room_height`, `max_room_height`: room dimensions.
- `loop_chance`: probability from 0 to 1 (or 0 to 100) for each non-tree link.
- `secret_room_chance`: probability per ordinary room of attempting an optional secret branch.
- `biome`, `theme`: labels copied to the instance and its rooms.
- `enemy_budget`, `loot_budget`: per-floor gameplay budget metadata.

## Output and validation

Each room includes an instance-local `id`, floor, position and size, role,
depth, difficulty, biome/theme labels, bidirectional `links`, and a
`gameplay` record. Roles include start, normal, elite, treasure, shop, shrine,
puzzle, trap, event, secret, miniboss, checkpoint, scenic, camp, crossing,
boss, and exit. The graph keeps secrets optional with hidden edges. Each edge
contains its corridor grid path; tower stair edges connect the exit of one
floor to the entrance of the next.

Gameplay records describe encounter budgets, boss phase/ability placeholders,
trap metadata, merchant or shrine events, room objectives, and chest reward
tiers. These are generation plans, not spawned mobs, objects, NPCs, quests, or
native room vnums. The host game must resolve the metadata against its content
catalogs and instantiate it.

`Generator.validate(instance)` checks required-room reachability, symmetric
connections, room bounds and overlap, and corridor paths. `World:generate_instance`
stores successful instances and emits `instance_generated`; use
`world:remove_instance(id)` to release an instance.

The generator version is included in each result. A version change can alter
the layout produced by a seed, so persist `generator_version` with saved
instances that must be reproducible across game updates.

The result also records `generator_seed` and `generation_attempt`. When a
validation failure causes regeneration, `seed` remains the requested dungeon
identity and `generator_seed` records the successful deterministic attempt.
