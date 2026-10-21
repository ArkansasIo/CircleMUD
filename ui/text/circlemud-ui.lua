local OOP = require("scripts.core.oop")

local TextUI = OOP.class("TextUI")

function TextUI:initialize(system)
  self.system = system or {}
  self.ready = false
end

function TextUI:boot(stage, progress)
  io.write(string.format("[%3d%%] %s\n", progress or 0, stage or "Starting"))
end

function TextUI:show_header()
  print("+--------------------------------------------------+")
  print("| AETHERWORLD RPG ENGINE 0.1.0 PRE-ALPHA          |")
  print("| ArkansasIo | AETHERWORLD-GE-0.1.0-PA-0001       |")
  print("+--------------------------------------------------+")
end

function TextUI:show_menu()
  print("1 World     2 Character     3 Quests")
  print("4 Inventory 5 Abilities     6 Factions")
  print("7 Combat    8 Campaign     9 System")
end

function TextUI:run()
  self:show_header()
  self:show_menu()
  self.ready = true
end

return TextUI
