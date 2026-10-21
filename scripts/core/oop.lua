-- Generic object-oriented programming helpers for AetherWorld Lua.
-- Uses tables + metatables and supports single inheritance.

local OOP = {}

function OOP.class(name, base)
  local cls = {}
  cls.__name = name or "Class"
  cls.__index = cls
  cls.__base = base

  if base then
    setmetatable(cls, { __index = base })
  end

  function cls:new(...)
    local object = setmetatable({}, cls)
    if object.initialize then
      object:initialize(...)
    end
    return object
  end

  function cls:is_a(target)
    local current = self
    if rawget(self, "__index") ~= self then
      current = getmetatable(self)
    end
    while current do
      if current == target then
        return true
      end
      current = current.__base
    end
    return false
  end

  return cls
end

function OOP.super(object, class_name, method_name, ...)
  local class = object
  while class and class.__name ~= class_name do
    class = class.__base
  end

  if not class or not class.__base then
    return nil, "superclass not found"
  end

  local method = class.__base[method_name]
  if not method then
    return nil, "super method not found"
  end

  return method(object, ...)
end

return OOP
