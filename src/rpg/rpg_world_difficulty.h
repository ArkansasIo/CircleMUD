#ifndef CIRCLEMUD_RPG_WORLD_DIFFICULTY_H
#define CIRCLEMUD_RPG_WORLD_DIFFICULTY_H

#include "rpg_types.h"

const rpg_world_difficulty *rpg_world_difficulty_get(int tier);
int rpg_world_difficulty_valid(int tier);
double rpg_world_health_multiplier(int tier);
double rpg_world_damage_multiplier(int tier);
double rpg_world_loot_multiplier(int tier);
double rpg_world_experience_multiplier(int tier);
double rpg_world_mechanics_multiplier(int tier);
double rpg_world_scale(double base, int tier, double player_scale, double encounter_scale);

#endif
