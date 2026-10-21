local Class = {}
Class.__index = Class

function Class:new(id, name)
  return setmetatable({ id = id, name = name, abilities = {}, tags = {} }, self)
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

return Class
