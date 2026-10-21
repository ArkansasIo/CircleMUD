-- Example scripted NPC using the OOP framework.

local OOP = require("scripts.core.oop")
local Entity = require("scripts.core.entity")

local NPC = OOP.class("NPC", Entity)

function NPC:initialize(id, name, level)
  self.id = id
  self.name = name or "NPC"
  self.level = level or 1
  self.active = true
  self.hostile = false
  self.components = {}
end

function NPC:on_spawn()
  self.active = true
end

function NPC:on_damage(amount, source_id)
  if amount > 0 then
    self.hostile = true
    self.last_attacker = source_id
  end
end

function NPC:update(delta_seconds)
  if not self.active then
    return
  end

  Entity.update(self, delta_seconds)
end

return NPC
