local OOP = require("scripts.core.oop")
local RoomGenerator = require("scripts.world.generator")

local World = OOP.class("World")

function World:initialize()
  self.difficulty = 1
  self.time = 0
  self.entities = {}
  self.events = {}
  self.instances = {}
end

function World:generate_instance(id, config)
  if id == nil or self.instances[id] then
    return nil, "instance id is missing or already in use"
  end
  local instance, err = RoomGenerator.generate(config)
  if not instance then return nil, err end
  instance.id = id
  self.instances[id] = instance
  self:emit("instance_generated", { instance_id = id, kind = instance.kind,
    layout = instance.layout, generator_version = instance.generator_version,
    room_count = #instance.rooms, floor_count = #instance.floors, seed = instance.seed })
  return instance
end

function World:remove_instance(id)
  if not self.instances[id] then return false end
  self.instances[id] = nil
  return true
end

function World:set_difficulty(tier)
  tier = math.max(1, math.min(9, tonumber(tier) or 1))
  self.difficulty = tier
  self:emit("world_difficulty_changed", { tier = tier })
end

function World:add_entity(entity)
  self.entities[entity.id] = entity
end

function World:remove_entity(id)
  self.entities[id] = nil
end

function World:on(event_name, callback)
  self.events[event_name] = self.events[event_name] or {}
  table.insert(self.events[event_name], callback)
end

function World:emit(event_name, payload)
  for _, callback in ipairs(self.events[event_name] or {}) do
    callback(payload)
  end
end

function World:update(dt)
  self.time = self.time + dt
  self:emit("world_tick", { time = self.time, difficulty = self.difficulty })
end

return World
