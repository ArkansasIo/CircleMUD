-- Deterministic, data-only BSP dungeon generator.
-- The host maps generated rooms to native room ids and spawns gameplay content.
local Generator = {VERSION = 2}

local VALID_KINDS = { dungeon = true, raid = true, trail = true, tower = true }
local MAX_ROOMS = 80
local MAX_MAP_SIZE = 160

local function clamp(value, low, high)
  value = tonumber(value) or low
  return math.max(low, math.min(high, math.floor(value)))
end

local function rng(seed)
  local state = math.floor(math.abs(tonumber(seed) or 1)) % 2147483647
  if state == 0 then state = 1 end
  return function(low, high)
    state = (state * 48271) % 2147483647
    if high == nil then return (state % low) + 1 end
    return low + (state % (high - low + 1))
  end
end

local function chance(random, value, fallback)
  local p = tonumber(value)
  if p == nil then p = fallback end
  if p > 1 then p = p / 100 end
  p = math.max(0, math.min(1, p))
  return random(1, 10000) <= math.floor(p * 10000)
end

local function difficulty_room_range(tier)
  if tier <= 5 then
    return math.floor(10 + (tier - 1) * 2.5), math.floor(20 + (tier - 1) * 5)
  end
  return math.floor(20 + (tier - 5) * 5), math.floor(40 + (tier - 5) * 10)
end

local function split_region(region, min_width, min_height, random)
  local can_x = region.width >= min_width * 2
  local can_y = region.height >= min_height * 2
  if not can_x and not can_y then return nil end
  local split_x
  if can_x and can_y then
    if region.width > region.height * 1.2 then split_x = true
    elseif region.height > region.width * 1.2 then split_x = false
    else split_x = random(1, 2) == 1 end
  else
    split_x = can_x
  end
  if split_x then
    local at = random(min_width, region.width - min_width)
    return {
      {x = region.x, y = region.y, width = at, height = region.height},
      {x = region.x + at, y = region.y, width = region.width - at, height = region.height}
    }
  end
  local at = random(min_height, region.height - min_height)
  return {
    {x = region.x, y = region.y, width = region.width, height = at},
    {x = region.x, y = region.y + at, width = region.width, height = region.height - at}
  }
end

local function make_regions(width, height, target, min_width, min_height, random)
  local leaves = {{x = 1, y = 1, width = width, height = height}}
  while #leaves < target do
    local eligible = {}
    for i, leaf in ipairs(leaves) do
      if leaf.width >= min_width * 2 or leaf.height >= min_height * 2 then
        table.insert(eligible, i)
      end
    end
    if #eligible == 0 then return nil, "map is too small for the requested room count" end
    local leaf_index = eligible[random(1, #eligible)]
    local children = split_region(leaves[leaf_index], min_width, min_height, random)
    if not children then return nil, "could not split a BSP region" end
    table.remove(leaves, leaf_index)
    table.insert(leaves, children[1])
    table.insert(leaves, children[2])
  end
  return leaves
end

local function room_center(room)
  return room.x + math.floor((room.width - 1) / 2), room.y + math.floor((room.height - 1) / 2)
end

local function make_rooms(regions, params, random, id_start, floor_number)
  local rooms = {}
  for i, region in ipairs(regions) do
    local max_width = math.min(params.max_room_width, region.width - 2)
    local max_height = math.min(params.max_room_height, region.height - 2)
    if max_width < params.min_room_width or max_height < params.min_room_height then
      return nil, "BSP region cannot fit the configured minimum room size"
    end
    local width = random(params.min_room_width, max_width)
    local height = random(params.min_room_height, max_height)
    local room = {
      id = id_start + i - 1,
      floor = floor_number,
      index = i,
      x = region.x + random(1, region.width - width - 1),
      y = region.y + random(1, region.height - height - 1),
      width = width,
      height = height,
      role = "normal",
      required = true,
      secret = false,
      links = {},
      connections = {},
      biome = params.biome,
      theme = params.theme,
      difficulty = params.difficulty
    }
    room.position = {x = room.x, y = room.y}
    room.size = {width = width, height = height}
    local center_x, center_y = room_center(room)
    room.center = {x = center_x, y = center_y}
    table.insert(rooms, room)
  end
  return rooms
end

local function distance(a, b)
  local ax, ay = room_center(a)
  local bx, by = room_center(b)
  return math.abs(ax - bx) + math.abs(ay - by)
end

local function find_root(parent, value)
  while parent[value] ~= value do
    parent[value] = parent[parent[value]]
    value = parent[value]
  end
  return value
end

local function connect(instance, a, b, properties)
  for _, linked_id in ipairs(a.links) do
    if linked_id == b.id then return nil end
  end
  table.insert(a.links, b.id)
  table.insert(b.links, a.id)
  table.insert(a.connections, b.id)
  table.insert(b.connections, a.id)
  local edge = {from = a.id, to = b.id, secret = false, transition = false}
  for key, value in pairs(properties or {}) do edge[key] = value end
  table.insert(instance.edges, edge)
  return edge
end

local function build_spanning_tree(instance, rooms)
  local candidates = {}
  for i = 1, #rooms - 1 do
    for j = i + 1, #rooms do
      table.insert(candidates, {a = rooms[i], b = rooms[j], weight = distance(rooms[i], rooms[j])})
    end
  end
  table.sort(candidates, function(a, b)
    if a.weight ~= b.weight then return a.weight < b.weight end
    if a.a.id ~= b.a.id then return a.a.id < b.a.id end
    return a.b.id < b.b.id
  end)
  local parent = {}
  for _, room in ipairs(rooms) do parent[room.id] = room.id end
  local chosen = {}
  for _, candidate in ipairs(candidates) do
    local left = find_root(parent, candidate.a.id)
    local right = find_root(parent, candidate.b.id)
    if left ~= right then
      parent[left] = right
      table.insert(chosen, connect(instance, candidate.a, candidate.b, {tree = true}))
      if #chosen == #rooms - 1 then break end
    end
  end
  return chosen
end

local function make_grid(params, rooms)
  local blocked = {}
  for y = 1, params.map_height do blocked[y] = {} end
  for _, room in ipairs(rooms) do
    for y = room.y, room.y + room.height - 1 do
      for x = room.x, room.x + room.width - 1 do blocked[y][x] = room.id end
    end
  end
  return blocked
end

local DIRECTIONS = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}}

local function make_cave_terrain(params, rooms, random)
  local grid = {}
  for y = 1, params.map_height do
    grid[y] = {}
    for x = 1, params.map_width do
      if x == 1 or y == 1 or x == params.map_width or y == params.map_height then
        grid[y][x] = 0
      else
        grid[y][x] = random(1, 100) <= 48 and 0 or 1
      end
    end
  end
  for _ = 1, 5 do
    local next_grid = {}
    for y = 1, params.map_height do
      next_grid[y] = {}
      for x = 1, params.map_width do
        if x == 1 or y == 1 or x == params.map_width or y == params.map_height then
          next_grid[y][x] = 0
        else
          local walls = 0
          for dy = -1, 1 do
            for dx = -1, 1 do
              if (dx ~= 0 or dy ~= 0) and grid[y + dy][x + dx] == 0 then walls = walls + 1 end
            end
          end
          next_grid[y][x] = walls >= 5 and 0 or 1
        end
      end
    end
    grid = next_grid
  end

  local visited, largest = {}, {}
  for y = 2, params.map_height - 1 do
    for x = 2, params.map_width - 1 do
      local key = (y - 1) * params.map_width + x
      if grid[y][x] == 1 and not visited[key] then
        local component, queue, head = {}, {{x = x, y = y}}, 1
        visited[key] = true
        while head <= #queue do
          local point = queue[head]
          head = head + 1
          table.insert(component, point)
          for _, direction in ipairs(DIRECTIONS) do
            local nx, ny = point.x + direction[1], point.y + direction[2]
            local next_key = (ny - 1) * params.map_width + nx
            if nx > 1 and nx < params.map_width and ny > 1 and ny < params.map_height and
                grid[ny][nx] == 1 and not visited[next_key] then
              visited[next_key] = true
              table.insert(queue, {x = nx, y = ny})
            end
          end
        end
        if #component > #largest then largest = component end
      end
    end
  end
  for y = 1, params.map_height do
    for x = 1, params.map_width do grid[y][x] = 0 end
  end
  for _, point in ipairs(largest) do grid[point.y][point.x] = 1 end
  for _, room in ipairs(rooms) do
    for y = room.y, room.y + room.height - 1 do
      for x = room.x, room.x + room.width - 1 do grid[y][x] = 1 end
    end
  end
  return grid
end

local function make_corridor(edge, a, b, rooms, params, terrain)
  local ax, ay = room_center(a)
  local bx, by = room_center(b)
  local blocked = make_grid(params, rooms)
  local start = (ay - 1) * params.map_width + ax
  local goal = (by - 1) * params.map_width + bx
  local function search(cave_only)
    local queue, previous, head = {start}, {[start] = false}, 1
    while head <= #queue and previous[goal] == nil do
      local current = queue[head]
      head = head + 1
      local x = ((current - 1) % params.map_width) + 1
      local y = math.floor((current - 1) / params.map_width) + 1
      for _, direction in ipairs(DIRECTIONS) do
        local nx, ny = x + direction[1], y + direction[2]
        if nx >= 1 and nx <= params.map_width and ny >= 1 and ny <= params.map_height then
          local next_id = (ny - 1) * params.map_width + nx
          local occupant = blocked[ny][nx]
          local walkable = occupant == nil or occupant == a.id or occupant == b.id
          if walkable and (not cave_only or not terrain or terrain[ny][nx] == 1 or occupant == a.id or occupant == b.id)
              and previous[next_id] == nil then
            previous[next_id] = current
            table.insert(queue, next_id)
          end
        end
      end
    end
    return previous
  end
  local previous = search(terrain ~= nil)
  if previous[goal] == nil and terrain then previous = search(false) end
  if previous[goal] == nil then return nil, "no navigable corridor could be routed" end
  local reverse = {}
  local cursor = goal
  while cursor do
    table.insert(reverse, {x = ((cursor - 1) % params.map_width) + 1,
      y = math.floor((cursor - 1) / params.map_width) + 1})
    cursor = previous[cursor]
  end
  local path = {}
  for i = #reverse, 1, -1 do table.insert(path, reverse[i]) end
  edge.corridor = path
  if terrain then
    for _, point in ipairs(path) do terrain[point.y][point.x] = 1 end
  end
  edge.corridor_style = #path <= math.max(math.abs(ax - bx), math.abs(ay - by)) + 1 and "straight" or "winding"
  return edge
end

local function add_loops(instance, rooms, params, random)
  local loops_added, max_loops = 0, math.max(1, math.floor(#rooms * 0.15))
  for i = 1, #rooms - 1 do
    if loops_added >= max_loops then break end
    for j = i + 1, #rooms do
      if loops_added >= max_loops then break end
      local a, b = rooms[i], rooms[j]
      local already_linked = false
      for _, id in ipairs(a.links) do if id == b.id then already_linked = true; break end end
      if not already_linked and chance(random, params.loop_chance, 0.12) then
        if connect(instance, a, b, {loop = true}) then loops_added = loops_added + 1 end
      end
    end
  end
end

local function add_secret_rooms(instance, rooms, params, random, next_id, floor_number)
  local max_secrets = math.min(5, math.floor(#rooms / 10))
  local source_room_count = #rooms
  local created = 0
  for _ = 1, source_room_count do
    if created >= max_secrets then break end
    if chance(random, params.secret_room_chance, 0.04) then
      for _ = 1, 80 do
        local width, height = random(3, 5), random(3, 5)
        local x, y = random(2, params.map_width - width - 1), random(2, params.map_height - height - 1)
        local candidate = {x = x, y = y, width = width, height = height}
        local overlaps = false
        for _, other in ipairs(rooms) do
          if not (x + width <= other.x or other.x + other.width <= x or
              y + height <= other.y or other.y + other.height <= y) then
            overlaps = true
            break
          end
        end
        if not overlaps then
          local room = {
            id = next_id, floor = floor_number, index = #rooms + 1,
            x = x, y = y, width = width, height = height,
            role = "secret", required = false, secret = true,
            links = {}, connections = {}, biome = params.biome,
            theme = params.theme, difficulty = params.difficulty
          }
          room.position = {x = x, y = y}
          room.size = {width = width, height = height}
          local center_x, center_y = room_center(room)
          room.center = {x = center_x, y = center_y}
          table.insert(rooms, room)
          local nearest, nearest_distance
          for _, other in ipairs(rooms) do
            if other.id ~= room.id then
              local d = distance(room, other)
              if not nearest_distance or d < nearest_distance then nearest, nearest_distance = other, d end
            end
          end
          connect(instance, nearest, room, {secret = true, hidden = true})
          next_id, created = next_id + 1, created + 1
          break
        end
      end
    end
  end
  return next_id
end

local function find_room(rooms, id)
  for _, room in ipairs(rooms) do if room.id == id then return room end end
end

local function graph_depths(instance, rooms, start_id, include_secret, tree_only)
  local depths, parents = {[start_id] = 0}, {}
  local queue, head = {start_id}, 1
  while head <= #queue do
    local id = queue[head]
    head = head + 1
    local room = find_room(rooms, id)
    for _, linked_id in ipairs(room and room.links or {}) do
      if find_room(rooms, linked_id) then
        local edge_is_secret, is_tree_edge = false, false
        for _, edge in ipairs(instance.edges) do
          if (edge.from == id and edge.to == linked_id) or (edge.to == id and edge.from == linked_id) then
            edge_is_secret = edge.secret
            is_tree_edge = edge.tree
            break
          end
        end
        if (include_secret or not edge_is_secret) and (not tree_only or is_tree_edge) and depths[linked_id] == nil then
          depths[linked_id] = depths[id] + 1
          parents[linked_id] = id
          table.insert(queue, linked_id)
        end
      end
    end
  end
  return depths, parents
end

local function choose_role(random, depth, max_depth, difficulty, kind)
  if depth == nil then return "secret" end
  if depth == 0 then return "start" end
  if depth == max_depth then return "exit" end
  local roll = random(1, 1000)
  if kind == "trail" then
    if roll <= 80 then return "scenic" end
    if roll <= 150 then return "camp" end
    if roll <= 220 then return "crossing" end
  elseif kind == "raid" then
    if difficulty >= 7 and roll <= 30 then return "miniboss" end
    if roll <= 75 then return "mechanic" end
    if roll <= 170 then return "elite" end
  end
  if difficulty >= 4 and roll <= 45 then return "elite" end
  if roll <= 105 then return "treasure" end
  if roll <= 145 then return "shop" end
  if roll <= 190 then return "shrine" end
  if roll <= 245 then return "puzzle" end
  if roll <= 295 then return "trap" end
  if roll <= 365 then return "event" end
  return "normal"
end

local function assign_roles(instance, rooms, random, difficulty, start_id, kind)
  local depths, parents = graph_depths(instance, rooms, start_id, false, true)
  local exit_id, max_depth = start_id, -1
  for _, room in ipairs(rooms) do
    local depth = depths[room.id]
    if depth and depth > max_depth then exit_id, max_depth = room.id, depth end
  end
  local boss_id = parents[exit_id]
  if not boss_id or boss_id == start_id then
    for _, room in ipairs(rooms) do
      if room.id ~= start_id and room.id ~= exit_id then boss_id = room.id; break end
    end
  end
  for _, room in ipairs(rooms) do
    if room.id == start_id then room.role = "start"
    elseif room.id == exit_id then room.role = "exit"
    elseif room.id == boss_id then room.role = "boss"
    else room.role = choose_role(random, depths[room.id], max_depth, difficulty, kind) end
    room.depth = depths[room.id] or -1
    room.required = not room.secret
    room.name = room.role:gsub("_", " "):gsub("^%l", string.upper)
  end
  if kind == "raid" then
    local checkpoint
    for _, room in ipairs(rooms) do
      if room.id ~= start_id and room.id ~= boss_id and room.id ~= exit_id and
          (not checkpoint or math.abs(room.depth - max_depth / 2) < math.abs(checkpoint.depth - max_depth / 2)) then
        checkpoint = room
      end
    end
    if checkpoint then checkpoint.role, checkpoint.name = "checkpoint", "Checkpoint" end
  end
  return depths, boss_id, exit_id
end

local function scale_for(difficulty)
  return 1 + ((difficulty - 1) * 0.20)
end

local function fill_gameplay(instance, rooms, params, random)
  local difficulty = params.difficulty
  local scale = scale_for(difficulty)
  local total_enemy = params.enemy_budget or (difficulty * 10000)
  local total_loot = params.loot_budget or (difficulty * 5000)
  local combat_rooms, elite_rooms = {}, {}
  for _, room in ipairs(rooms) do
    if room.role == "normal" then table.insert(combat_rooms, room)
    elseif room.role == "elite" or room.role == "miniboss" then table.insert(elite_rooms, room) end
  end
  local trash_pool = math.floor(total_enemy * 0.55)
  local elite_pool = math.floor(total_enemy * 0.25)
  local boss_pool = total_enemy - trash_pool - elite_pool
  local trash_each = #combat_rooms > 0 and math.floor(trash_pool / #combat_rooms) or 0
  local elite_each = #elite_rooms > 0 and math.floor(elite_pool / #elite_rooms) or 0

  for _, room in ipairs(rooms) do
    room.gameplay = {traps = {}, npcs = {}, quests = {}, events = {}, chests = {}}
    if room.role == "normal" then
      room.gameplay.encounter = {kind = "trash", budget = trash_each}
    elseif room.role == "elite" or room.role == "miniboss" then
      room.gameplay.encounter = {kind = room.role, budget = elite_each}
    elseif room.role == "boss" then
      room.gameplay.encounter = {kind = "boss", budget = boss_pool}
      room.gameplay.boss = {
        level = difficulty * 10 + room.floor,
        health_multiplier = scale * 2.5,
        damage_multiplier = scale * 1.5,
        abilities = {"heavy_strike", "area_attack"},
        phases = {"opening", "enraged"}
      }
    end
    if room.role == "trap" or (difficulty >= 5 and chance(random, 0.08, 0.08)) then
      table.insert(room.gameplay.traps, {kind = "theme_hazard", damage_scale = scale})
    end
    if room.role == "shop" then table.insert(room.gameplay.npcs, {kind = "merchant"}) end
    if room.role == "shrine" then table.insert(room.gameplay.events, {kind = "shrine_blessing"}) end
    if room.role == "event" then table.insert(room.gameplay.events, {kind = "biome_event"}) end
    if room.role == "checkpoint" then table.insert(room.gameplay.events, {kind = "raid_checkpoint"}) end
    if room.role == "scenic" then table.insert(room.gameplay.events, {kind = "trail_discovery"}) end
    if room.role == "camp" then table.insert(room.gameplay.events, {kind = "rest_camp"}) end
    if room.role == "crossing" then table.insert(room.gameplay.events, {kind = "trail_crossing"}) end
    if room.role == "puzzle" then table.insert(room.gameplay.quests, {kind = "room_objective", required = room.required}) end
    if room.role == "mechanic" then table.insert(room.gameplay.quests, {kind = "raid_mechanic", required = room.required}) end
    if room.role == "treasure" or room.role == "boss" or room.role == "miniboss" or room.secret then
      local share = room.role == "boss" and 0.35 or (room.role == "miniboss" and 0.15 or (room.secret and 0.12 or 0.08))
      table.insert(room.gameplay.chests, {
        loot_budget = math.floor(total_loot * share),
        tier = math.min(9, difficulty + (room.role == "boss" and 1 or 0)),
        boss_reward = room.role == "boss" or room.role == "miniboss"
      })
    end
    room.difficulty_scale = scale
  end
  local budgets = {
    encounter = {total = total_enemy, trash = trash_pool, elite = elite_pool, boss = boss_pool},
    loot = {total = total_loot, scale = scale}
  }
  instance.budgets = instance.budgets or {floors = {}}
  instance.budgets.floors[rooms[1].floor] = budgets
  return budgets
end

local function validate_floor(instance, floor_data, params)
  local errors, rooms_by_id = {}, {}
  for _, room in ipairs(floor_data.rooms) do rooms_by_id[room.id] = room end
  if not rooms_by_id[floor_data.entrance_id] then table.insert(errors, "floor has no valid start room") end
  if not rooms_by_id[floor_data.boss_id] then table.insert(errors, "floor has no valid boss room") end
  if not rooms_by_id[floor_data.exit_id] then table.insert(errors, "floor has no valid exit room") end
  if params.difficulty < 1 or params.difficulty > 9 then table.insert(errors, "difficulty is outside tiers 1 through 9") end
  for i, room in ipairs(floor_data.rooms) do
    if room.x < 1 or room.y < 1 or room.x + room.width - 1 > params.map_width or room.y + room.height - 1 > params.map_height then
      table.insert(errors, "room " .. room.id .. " is outside map bounds")
    end
    for j = i + 1, #floor_data.rooms do
      local other = floor_data.rooms[j]
      if not (room.x + room.width <= other.x or other.x + other.width <= room.x or
          room.y + room.height <= other.y or other.y + other.height <= room.y) then
        table.insert(errors, "rooms " .. room.id .. " and " .. other.id .. " overlap")
      end
    end
    for _, linked_id in ipairs(room.links) do
      local linked = find_room(instance.rooms, linked_id)
      if not linked then table.insert(errors, "room " .. room.id .. " links to a missing room")
      else
        local symmetric = false
        for _, back_id in ipairs(linked.links) do if back_id == room.id then symmetric = true; break end end
        if not symmetric then table.insert(errors, "room connection is not bidirectional") end
      end
    end
  end
  local depths = graph_depths(instance, floor_data.rooms, floor_data.entrance_id, false)
  for _, id in ipairs({floor_data.boss_id, floor_data.exit_id}) do
    if id and depths[id] == nil then table.insert(errors, "required room " .. id .. " is unreachable from start") end
  end
  for _, room in ipairs(floor_data.rooms) do
    if room.required and depths[room.id] == nil then table.insert(errors, "required room " .. room.id .. " is unreachable from start") end
  end
  for _, edge in ipairs(instance.edges) do
    if edge.floor == floor_data.number and not edge.transition then
      if type(edge.corridor) ~= "table" or #edge.corridor == 0 then
        table.insert(errors, "edge " .. edge.from .. "-" .. edge.to .. " has no corridor")
      else
        local from_room, to_room = rooms_by_id[edge.from], rooms_by_id[edge.to]
        if not from_room or not to_room then
          table.insert(errors, "edge references a room outside its floor")
        else
        local from_x, from_y = room_center(from_room)
        local to_x, to_y = room_center(to_room)
        local previous = {x = from_x, y = from_y}
        for index, point in ipairs(edge.corridor) do
          if point.x < 1 or point.x > params.map_width or point.y < 1 or point.y > params.map_height then
            table.insert(errors, "corridor leaves map bounds")
            break
          end
          if index == 1 and (point.x ~= from_x or point.y ~= from_y) then
            table.insert(errors, "corridor does not start in its source room")
            break
          end
          if math.abs(point.x - previous.x) + math.abs(point.y - previous.y) ~= 1 then
            table.insert(errors, "corridor contains a non-adjacent step")
            break
          end
          if floor_data.terrain and floor_data.terrain[point.y][point.x] ~= 1 then
            table.insert(errors, "corridor crosses an uncarved cave cell")
            break
          end
          for _, blocker in ipairs(floor_data.rooms) do
            if blocker.id ~= from_room.id and blocker.id ~= to_room.id and
                point.x >= blocker.x and point.x < blocker.x + blocker.width and
                point.y >= blocker.y and point.y < blocker.y + blocker.height then
              table.insert(errors, "corridor crosses unrelated room " .. blocker.id)
              break
            end
          end
          previous = point
        end
        local last = edge.corridor[#edge.corridor]
        if last and (last.x ~= to_x or last.y ~= to_y) then
          table.insert(errors, "corridor does not end in its destination room")
        end
        end
      end
    end
  end
  return #errors == 0, errors
end

local function parameters(config, kind, difficulty)
  local suggested_min, suggested_max = difficulty_room_range(difficulty)
  local min_rooms = clamp(config.min_rooms or config.rooms or config.rooms_per_floor or suggested_min, 3, MAX_ROOMS)
  local max_rooms = clamp(config.max_rooms or config.rooms or config.rooms_per_floor or suggested_max, min_rooms, MAX_ROOMS)
  local min_room_width = clamp(config.min_room_width or 4, 3, 20)
  local min_room_height = clamp(config.min_room_height or 4, 3, 20)
  local max_room_width = clamp(config.max_room_width or 10, min_room_width, 32)
  local max_room_height = clamp(config.max_room_height or 10, min_room_height, 32)
  local default_map = math.max(48, math.ceil(math.sqrt(max_rooms) * 10))
  local map_width = clamp(config.map_width or default_map, 24, MAX_MAP_SIZE)
  local map_height = clamp(config.map_height or default_map, 24, MAX_MAP_SIZE)
  local enemy_budget = tonumber(config.enemy_budget)
  local loot_budget = tonumber(config.loot_budget)
  if enemy_budget then enemy_budget = math.max(0, math.floor(enemy_budget)) end
  if loot_budget then loot_budget = math.max(0, math.floor(loot_budget)) end
  return {
    kind = kind,
    layout = config.layout == "cave" and "cave" or "bsp",
    difficulty = difficulty,
    biome = type(config.biome) == "string" and config.biome or "temperate_forest",
    theme = type(config.theme) == "string" and config.theme or "classic",
    min_rooms = min_rooms, max_rooms = max_rooms,
    min_room_width = min_room_width, max_room_width = max_room_width,
    min_room_height = min_room_height, max_room_height = max_room_height,
    map_width = map_width, map_height = map_height,
    loop_chance = config.loop_chance or config.loopChance,
    secret_room_chance = config.secret_room_chance or config.secretRoomChance,
    enemy_budget = enemy_budget,
    loot_budget = loot_budget
  }
end

local function generate_floor(instance, random, params, floor_number, id_start)
  local target = random(params.min_rooms, params.max_rooms)
  local regions, err = make_regions(params.map_width, params.map_height, target,
    params.min_room_width + 2, params.min_room_height + 2, random)
  if not regions then return nil, err end
  local rooms
  rooms, err = make_rooms(regions, params, random, id_start, floor_number)
  if not rooms then return nil, err end
  local terrain = params.layout == "cave" and make_cave_terrain(params, rooms, random) or nil
  local floor_data = {number = floor_number, rooms = rooms, map_width = params.map_width,
    map_height = params.map_height, layout = params.layout, terrain = terrain}
  table.insert(instance.floors, floor_data)
  for _, room in ipairs(rooms) do table.insert(instance.rooms, room) end

  build_spanning_tree(instance, rooms)
  add_loops(instance, rooms, params, random)
  local next_id = id_start + #rooms
  next_id = add_secret_rooms(instance, rooms, params, random, next_id, floor_number)
  -- Secret rooms were appended after the floor's base rooms.
  for _, room in ipairs(rooms) do
    if not find_room(instance.rooms, room.id) then table.insert(instance.rooms, room) end
  end
  if terrain then
    for _, room in ipairs(rooms) do
      for y = room.y, room.y + room.height - 1 do
        for x = room.x, room.x + room.width - 1 do terrain[y][x] = 1 end
      end
    end
  end
  for _, edge in ipairs(instance.edges) do
    if edge.floor == nil and not edge.transition then edge.floor = floor_number end
  end
  for _, edge in ipairs(instance.edges) do
    if edge.floor == floor_number and not edge.transition then
      local a, b = find_room(rooms, edge.from), find_room(rooms, edge.to)
      local corridor
      corridor, err = make_corridor(edge, a, b, rooms, params, terrain)
      if not corridor then return nil, err end
    end
  end

  local start_id = rooms[1].id
  local depths, boss_id, exit_id = assign_roles(instance, rooms, random, params.difficulty, start_id, params.kind)
  local budgets = fill_gameplay(instance, rooms, params, random)
  floor_data.entrance_id = start_id
  floor_data.boss_id = boss_id
  floor_data.exit_id = exit_id
  floor_data.budgets = budgets
  local valid, errors = validate_floor(instance, floor_data, params)
  if not valid then return nil, table.concat(errors, "; ") end
  floor_data.depths = depths
  return floor_data, next_id
end

function Generator.validate(instance)
  if type(instance) ~= "table" or type(instance.floors) ~= "table" or #instance.floors == 0 then
    return false, {"instance is missing generated floors"}
  end
  for _, floor in ipairs(instance.floors) do
    local params = floor.parameters
    if not params then return false, {"floor is missing generation parameters"} end
    local valid, errors = validate_floor(instance, floor, params)
    if not valid then return false, errors end
  end
  return true, {}
end

local function generate_once(config, seed)
  config = config or {}
  local kind = config.kind or "dungeon"
  if not VALID_KINDS[kind] then return nil, "kind must be dungeon, raid, trail, or tower" end
  seed = math.floor(math.abs(tonumber(seed) or 1)) % 2147483647
  local difficulty = clamp(config.difficulty, 1, 9)
  local floor_count = kind == "tower" and clamp(config.floors or 5, 1, 100) or 1
  local params = parameters(config, kind, difficulty)
  local random = rng(seed)
  local instance = {
    generator_version = Generator.VERSION,
    kind = kind,
    layout = params.layout,
    seed = seed,
    difficulty = difficulty,
    biome = params.biome,
    theme = params.theme,
    floors = {}, rooms = {}, edges = {},
    entrance_id = nil, boss_id = nil, exit_id = nil
  }
  local next_id = 1
  for floor_number = 1, floor_count do
    if floor_number > 1 then
      params = parameters(config, kind, difficulty)
    end
    local floor_data, updated_id_or_error = generate_floor(instance, random, params, floor_number, next_id)
    if not floor_data then return nil, updated_id_or_error end
    floor_data.parameters = params
    next_id = updated_id_or_error
    if floor_number == 1 then instance.entrance_id = floor_data.entrance_id end
    if floor_number > 1 then
      local previous = instance.floors[floor_number - 1]
      local previous_exit = find_room(previous.rooms, previous.exit_id)
      local current_entrance = find_room(floor_data.rooms, floor_data.entrance_id)
      connect(instance, previous_exit, current_entrance, {transition = true, stair_link = true})
      previous_exit.role = "stairs_up"
      current_entrance.role = "stairs_down"
      previous_exit.name = "Stairs Up"
      current_entrance.name = "Stairs Down"
    end
  end
  local last_floor = instance.floors[#instance.floors]
  instance.boss_id, instance.exit_id = last_floor.boss_id, last_floor.exit_id
  local valid, errors = Generator.validate(instance)
  if not valid then return nil, table.concat(errors, "; ") end
  instance.generator_seed = seed
  return instance
end

function Generator.generate(config)
  config = config or {}
  local seed = math.floor(math.abs(tonumber(config.seed) or 1))
  local attempts = clamp(config.max_attempts or 20, 1, 20)
  local last_error
  for attempt = 1, attempts do
    local attempt_seed = (seed + (attempt - 1) * 104729) % 2147483647
    local instance, err = generate_once(config, attempt_seed)
    if instance then
      instance.seed = seed
      instance.generation_attempt = attempt
      return instance
    end
    last_error = err
  end
  return nil, "generation failed after " .. attempts .. " attempts: " .. tostring(last_error)
end

return Generator
