-- Base gameplay class template.
-- Prefer scripts.core.oop for new classes that need inheritance.

local OOP = require("scripts.core.oop")

local Class = OOP.class("GameClass")

function Class:initialize(id, name)
  self.id = id
  self.name = name or "Class"
  self.abilities = {}
  self.tags = {}
end

function Class:add_ability(ability)
  self.abilities[#self.abilities + 1] = ability
end

function Class:has_tag(tag)
  return self.tags[tag] == true
end

function Class:add_tag(tag)
  self.tags[tag] = true
end

function Class:get_ability(id)
  for _, ability in ipairs(self.abilities) do
    if ability.id == id then
      return ability
    end
  end
  return nil
end

return Class
