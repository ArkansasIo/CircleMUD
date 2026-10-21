#ifndef CIRCLEMUD_RPG_LOOT_H
#define CIRCLEMUD_RPG_LOOT_H

#include "rpg_types.h"
#include "rpg_rng.h"

rpg_rarity rpg_roll_rarity(rpg_rng *rng, double loot_scale, int world_difficulty, int boss);
int rpg_rarity_affix_count(rpg_rarity rarity);
double rpg_rarity_power_multiplier(rpg_rarity rarity);

#endif
