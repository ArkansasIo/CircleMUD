# Procedural instance rooms

`generator.lua` builds deterministic, data-only room graphs for dungeons, raids,
trails, and multi-floor towers. A seeded pseudo-random generator makes the same
configuration produce the same layout. Every floor has a connected entrance to
exit path; optional side rooms and loops add exploration without breaking it.

```lua
local World = require("scripts.core.world")
local world = World:new()

local raid, err = world:generate_instance("raid-42", {
  kind = "raid", seed = 20261021, rooms = 12,
  biome = "arcane", difficulty = 6
})
assert(raid, err)
```

Set `kind` to `dungeon`, `raid`, `trail`, or `tower`. `seed` controls layout;
`biome` is copied to each room for the host's room descriptions and encounter
selection; `difficulty` is clamped to tiers 1 through 9. `rooms` (or
`rooms_per_floor`) sets the backbone size from 3 through 80. Towers accept
`floors` from 1 through 20 and use 3 through 12 rooms per floor. Other instance
types contain one floor.

Each room has a unique instance-local `id`, `floor`, `index`, grid coordinates
`x`/`y`, `role`, display `name`, `biome`, `difficulty`, and `links` (the ids of
connected rooms). The graph is undirected. Raid layouts include a checkpoint
and a final boss; tower floors are connected by stairs; trails use scenic,
camp, and crossing room roles. The generator returns data and does not allocate
CircleMUD room vnums or spawn entities. The host can translate room records
into native rooms and react to the `instance_generated` world event. Instances
are stored by id and can be removed with `world:remove_instance(id)`.
