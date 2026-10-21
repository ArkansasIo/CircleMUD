-- Lightweight event bus for Lua gameplay scripts.

local OOP = require("scripts.core.oop")

local EventBus = OOP.class("EventBus")

function EventBus:initialize()
  self.listeners = {}
end

function EventBus:on(event_name, callback)
  assert(type(event_name) == "string", "event name must be a string")
  assert(type(callback) == "function", "event callback must be a function")

  self.listeners[event_name] = self.listeners[event_name] or {}
  table.insert(self.listeners[event_name], callback)
  return callback
end

function EventBus:emit(event_name, payload)
  local listeners = self.listeners[event_name]
  if not listeners then
    return
  end

  for _, callback in ipairs(listeners) do
    callback(payload or {})
  end
end

function EventBus:clear(event_name)
  if event_name then
    self.listeners[event_name] = nil
  else
    self.listeners = {}
  end
end

return EventBus
