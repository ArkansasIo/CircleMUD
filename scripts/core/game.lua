-- CircleMUD GameEngine Lua host API.
-- C remains authoritative; Lua provides gameplay behavior and data.

local OOP = require("scripts.core.oop")
local EventBus = require("scripts.core.event")

Game = Game or {}
Game.events = Game.events or EventBus:new()
Game.time = Game.time or 0

function Game_Update(delta_seconds)
  Game.time = Game.time + (delta_seconds or 0)
  if Game.on_update then
    Game.on_update(delta_seconds or 0)
  end
end

function Game_Event(event_type, actor_id, target_id, value, name)
  local event = {
    type = event_type,
    actor_id = actor_id,
    target_id = target_id,
    value = value,
    name = name
  }

  Game.events:emit(event_type, event)

  if Game.on_event then
    Game.on_event(event)
  end
end

function Game:on(event_name, callback)
  return self.events:on(event_name, callback)
end

function Game:emit(event_name, payload)
  self.events:emit(event_name, payload)
end

function Game.new_class(name, base)
  return OOP.class(name, base)
end

return Game
