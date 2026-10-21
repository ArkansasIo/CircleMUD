#ifndef CIRCLEMUD_RPG_LEVEL_H
#define CIRCLEMUD_RPG_LEVEL_H

#include "rpg_types.h"

int rpg_level_valid(int level);
double rpg_level_xp_required(int level);
double rpg_level_power_scale(int level);
int rpg_level_band(int level);
int rpg_level_milestone(int level);

#endif
