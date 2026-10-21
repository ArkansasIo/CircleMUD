#include "rpg_world_difficulty.h"

static const rpg_world_difficulty WORLD_DIFFICULTIES[10] = {
  {0, "none", "None", 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0, 0, 0},
  {1, "frontier", "Frontier", 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1, 1, 0},
  {2, "dangerous", "Dangerous", 1.10, 1.08, 1.05, 1.05, 1.05, 1.05, 1, 1, 1},
  {3, "hostile", "Hostile", 1.22, 1.18, 1.12, 1.10, 1.10, 1.10, 1, 1, 1},
  {4, "brutal", "Brutal", 1.38, 1.30, 1.20, 1.18, 1.18, 1.16, 2, 1, 2},
  {5, "deadly", "Deadly", 1.58, 1.48, 1.32, 1.28, 1.30, 1.24, 2, 2, 3},
  {6, "cataclysmic", "Cataclysmic", 1.85, 1.72, 1.48, 1.42, 1.48, 1.34, 3, 2, 4},
  {7, "mythic_world", "Mythic World", 2.20, 2.05, 1.68, 1.62, 1.72, 1.46, 3, 3, 5},
  {8, "apocalypse", "Apocalypse", 2.65, 2.48, 1.92, 1.88, 2.00, 1.62, 4, 3, 6},
  {9, "eternal_nightmare", "Eternal Nightmare", 3.20, 3.05, 2.25, 2.20, 2.40, 1.85, 5, 4, 8}
};

const rpg_world_difficulty *rpg_world_difficulty_get(int tier) {
  if (!rpg_world_difficulty_valid(tier)) return &WORLD_DIFFICULTIES[1];
  return &WORLD_DIFFICULTIES[tier];
}

int rpg_world_difficulty_valid(int tier) {
  return tier >= RPG_WORLD_DIFFICULTY_MIN && tier <= RPG_WORLD_DIFFICULTY_MAX;
}

double rpg_world_health_multiplier(int tier) { return rpg_world_difficulty_get(tier)->health; }
double rpg_world_damage_multiplier(int tier) { return rpg_world_difficulty_get(tier)->damage; }
double rpg_world_loot_multiplier(int tier) { return rpg_world_difficulty_get(tier)->loot; }
double rpg_world_experience_multiplier(int tier) { return rpg_world_difficulty_get(tier)->experience; }
double rpg_world_mechanics_multiplier(int tier) { return rpg_world_difficulty_get(tier)->mechanics; }

double rpg_world_scale(double base, int tier, double player_scale, double encounter_scale) {
  const rpg_world_difficulty *d = rpg_world_difficulty_get(tier);
  return base * d->health * player_scale * encounter_scale;
}
