local WorldEvents = {}

WorldEvents.event_types = {
  "invasion", "boss_spawn", "corruption", "resource_surge",
  "faction_war", "weather_storm", "elite_hunt", "world_defense"
}

function WorldEvents.difficulty_weight(tier)
  return 1.0 + ((math.max(1, math.min(9, tier)) - 1) * 0.25)
end

function WorldEvents.spawn_profile(tier)
  tier = math.max(1, math.min(9, tonumber(tier) or 1))
  return {
    difficulty = tier,
    elite_count = math.max(0, tier - 1),
    champion_count = tier >= 4 and tier - 3 or 0,
    boss_phase_bonus = tier >= 7 and tier - 6 or 0,
    hazard_level = tier >= 5 and tier - 4 or 0
  }
end

return WorldEvents
