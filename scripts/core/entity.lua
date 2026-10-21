local Entity = {}
Entity.__index = Entity

function Entity:new(id, name)
  return setmetatable({
    id = id,
    name = name or "entity",
    active = true,
    x = 0,
    y = 0,
    z = 0,
    components = {}
  }, self)
end

function Entity:add_component(name, component)
  self.components[name] = component
end

function Entity:get_component(name)
  return self.components[name]
end

function Entity:update(delta_seconds)
  for _, component in pairs(self.components) do
    if component.update then
      component:update(delta_seconds)
    end
  end
end

return Entity
