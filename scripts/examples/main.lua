-- Minimal entry point demonstrating the scripting framework.

local NPC = require("scripts.examples.npc")
local Quest = require("scripts.examples.quest")
local Ability = require("scripts.examples.ability")
local Zone = require("scripts.examples.zone")

local guard = NPC:new(1001, "Circle Guard", 5)
local quest = Quest:new(2001, "The Broken Circle")
local strike = Ability:new("strike", "Guard Strike", 2.0)
local zone = Zone:new(3001, "Greenvale", "temperate_forest")

quest:add_objective("bandits", "Defeat the bandits", 5)
quest:activate()

zone:on_player_enter(function(event)
  if Game and Game.log then
    Game.log("player " .. tostring(event.player_id) .. " entered " .. zone.name)
  end
end)

return {
  guard = guard,
  quest = quest,
  strike = strike,
  zone = zone
}
