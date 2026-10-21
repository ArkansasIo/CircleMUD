-- Example zone controller using OOP + event-driven scripting.

local OOP = require("scripts.core.oop")
local EventBus = require("scripts.core.event")

local Zone = OOP.class("Zone")

function Zone:initialize(id, name, biome)
  self.id = id
  self.name = name or "Zone"
  self.biome = biome or "temperate_forest"
  self.active = true
  self.events = EventBus:new()
end

function Zone:on_player_enter(callback)
  return self.events:on("player_enter", callback)
end

function Zone:on_player_leave(callback)
  return self.events:on("player_leave", callback)
end

function Zone:player_enter(player_id)
  self.events:emit("player_enter", {
    zone_id = self.id,
    player_id = player_id
  })
end

function Zone:player_leave(player_id)
  self.events:emit("player_leave", {
    zone_id = self.id,
    player_id = player_id
  })
end

function Zone:update(delta_seconds)
  if not self.active then
    return
  end
  self.last_delta = delta_seconds
end

return Zone
