-- Example quest object using the Lua OOP framework.

local OOP = require("scripts.core.oop")
local Quest = OOP.class("Quest")

function Quest:initialize(id, name)
  self.id = id
  self.name = name or "Unnamed Quest"
  self.state = "inactive"
  self.objectives = {}
  self.rewards = {}
end

function Quest:add_objective(id, description, required)
  self.objectives[#self.objectives + 1] = {
    id = id,
    description = description,
    required = required or 1,
    progress = 0,
    complete = false
  }
end

function Quest:activate()
  self.state = "active"
end

function Quest:advance(objective_id, amount)
  for _, objective in ipairs(self.objectives) do
    if objective.id == objective_id and not objective.complete then
      objective.progress = math.min(objective.required, objective.progress + (amount or 1))
      objective.complete = objective.progress >= objective.required
      return objective.complete
    end
  end
  return false
end

function Quest:is_complete()
  for _, objective in ipairs(self.objectives) do
    if not objective.complete then
      return false
    end
  end
  return #self.objectives > 0
end

function Quest:complete()
  if self:is_complete() then
    self.state = "complete"
    return true
  end
  return false
end

return Quest
