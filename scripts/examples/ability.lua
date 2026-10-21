-- Example ability object. Keep authoritative combat resolution in C.

local OOP = require("scripts.core.oop")
local Ability = OOP.class("Ability")

function Ability:initialize(id, name, cooldown)
  self.id = id
  self.name = name or "Ability"
  self.cooldown = cooldown or 0
  self.enabled = true
end

function Ability:can_use(context)
  return self.enabled and context ~= nil
end

function Ability:execute(context)
  if not self:can_use(context) then
    return false, "ability unavailable"
  end

  -- Request authoritative resolution from the C engine.
  if Game and Game.request_ability then
    return Game.request_ability(self.id, context)
  end

  return false, "engine binding unavailable"
end

return Ability
