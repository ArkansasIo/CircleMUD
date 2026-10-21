local WorldDifficulty = {}

WorldDifficulty.tiers = {
  [1] = { id = "frontier", name = "Frontier", hazard = 0 },
  [2] = { id = "dangerous", name = "Dangerous", hazard = 1 },
  [3] = { id = "hostile", name = "Hostile", hazard = 1 },
  [4] = { id = "brutal", name = "Brutal", hazard = 2 },
  [5] = { id = "deadly", name = "Deadly", hazard = 3 },
  [6] = { id = "cataclysmic", name = "Cataclysmic", hazard = 4 },
  [7] = { id = "mythic_world", name = "Mythic World", hazard = 5 },
  [8] = { id = "apocalypse", name = "Apocalypse", hazard = 6 },
  [9] = { id = "eternal_nightmare", name = "Eternal Nightmare", hazard = 8 }
}

function WorldDifficulty.get(tier)
  tier = math.max(1, math.min(9, tonumber(tier) or 1))
  return WorldDifficulty.tiers[tier]
end

function WorldDifficulty.scale(tier, value, stat)
  local multipliers = {
    health = {1.00,1.10,1.22,1.38,1.58,1.85,2.20,2.65,3.20},
    damage = {1.00,1.08,1.18,1.30,1.48,1.72,2.05,2.48,3.05},
    armor = {1.00,1.05,1.12,1.20,1.32,1.48,1.68,1.92,2.25},
    loot = {1.00,1.05,1.10,1.18,1.28,1.42,1.62,1.88,2.20}
  }
  local table_for_stat = multipliers[stat] or multipliers.health
  tier = math.max(1, math.min(9, tonumber(tier) or 1))
  return value * table_for_stat[tier]
end

return WorldDifficulty
