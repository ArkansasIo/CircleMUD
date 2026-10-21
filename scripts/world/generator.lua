-- Seeded, data-only room graph generator for instanced content.
-- The host can map these room records to native room vnums at runtime.
local Generator = {}

local TYPES = { dungeon = true, raid = true, trail = true, tower = true }
local MAX_ROOMS = 80

local function clamp(value, low, high)
  value = tonumber(value) or low
  return math.max(low, math.min(high, math.floor(value)))
end

local function rng(seed)
  local state = math.floor(math.abs(tonumber(seed) or 1)) % 2147483647
  if state == 0 then state = 1 end
  return function(limit)
    state = (state * 48271) % 2147483647
    return (state % limit) + 1
  end
end

local function connect(a, b)
  for _, id in ipairs(a.links) do
    if id == b.id then return end
  end
  table.insert(a.links, b.id)
  table.insert(b.links, a.id)
end

local function set_role(room, role)
  room.role = role
  room.name = role:gsub("_", " "):gsub("^%l", string.upper)
end

local function generate_floor(kind, floor_number, count, random, id_start, difficulty, biome)
  local rooms = {}
  local templates = {
    dungeon = {"corridor", "combat", "treasure", "hazard", "rest"},
    raid = {"approach", "combat", "mechanic", "checkpoint", "boss"},
    trail = {"trail", "scenic", "encounter", "camp", "crossing"},
    tower = {"stairwell", "combat", "trial", "treasure", "rest"}
  }
  for i = 1, count do
    local role = "combat"
    if i == 1 then role = "entrance"
    elseif i == count then
      role = (kind == "raid" or kind == "tower") and "boss" or "exit"
    elseif i == math.floor((count + 1) / 2) and kind == "raid" then role = "checkpoint"
    else role = templates[kind][random(#templates[kind])] end
    rooms[i] = {
      id = id_start + i - 1,
      floor = floor_number,
      index = i,
      x = i - 1,
      y = 0,
      role = role,
      name = role:gsub("_", " "):gsub("^%l", string.upper),
      biome = biome,
      difficulty = difficulty,
      links = {}
    }
  end

  -- Backbone is always traversable from entrance to exit/boss.
  for i = 1, count - 1 do connect(rooms[i], rooms[i + 1]) end

  -- Add side rooms and occasional loops without disconnecting the main route.
  local next_id = id_start + count
  for i = 2, count - 1 do
    if random(100) <= 28 and #rooms < MAX_ROOMS then
      local side = {
        id = next_id, floor = floor_number, index = #rooms + 1,
        x = i - 1, y = (random(2) == 1 and -1 or 1),
        role = templates[kind][random(#templates[kind])],
        biome = biome, difficulty = difficulty, links = {}
      }
      side.name = "Side " .. side.role:gsub("_", " ")
      next_id = next_id + 1
      table.insert(rooms, side)
      connect(rooms[i], side)
      if i > 2 and random(100) <= 35 then connect(rooms[i - 1], side) end
    end
  end
  return rooms, next_id, rooms[1], rooms[count]
end

function Generator.generate(config)
  config = config or {}
  local kind = config.kind or "dungeon"
  if not TYPES[kind] then return nil, "kind must be dungeon, raid, trail, or tower" end
  local random = rng(config.seed)
  local difficulty = clamp(config.difficulty, 1, 9)
  local biome = type(config.biome) == "string" and config.biome or "temperate_forest"
  local floors = kind == "tower" and clamp(config.floors or 5, 1, 20) or 1
  local rooms_per_floor = clamp(config.rooms_per_floor or config.rooms or 8, 3, MAX_ROOMS)
  local instance = {
    kind = kind, seed = tonumber(config.seed) or 1, difficulty = difficulty,
    biome = biome,
    floors = {}, rooms = {}, entrance_id = 1, exit_id = nil
  }
  local next_id = 1
  for floor_number = 1, floors do
    local count = rooms_per_floor
    if kind == "tower" then count = clamp(rooms_per_floor + random(3) - 2, 3, 12) end
    local floor_rooms, entrance, exit
    floor_rooms, next_id, entrance, exit = generate_floor(kind, floor_number, count, random, next_id, difficulty, biome)
    instance.floors[floor_number] = {
      number = floor_number, rooms = floor_rooms, entrance = entrance, exit = exit
    }
    for _, room in ipairs(floor_rooms) do table.insert(instance.rooms, room) end
    if floor_number > 1 then
      local previous = instance.floors[floor_number - 1]
      connect(previous.exit, entrance)
      set_role(previous.exit, "stairs_up")
      set_role(entrance, "stairs_down")
    end
  end
  instance.exit_id = instance.floors[floors].exit.id
  return instance
end

return Generator
