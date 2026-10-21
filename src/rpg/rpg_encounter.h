#ifndef CIRCLEMUD_RPG_ENCOUNTER_H
#define CIRCLEMUD_RPG_ENCOUNTER_H

#include "rpg_types.h"

rpg_encounter_scaling rpg_encounter_scale(int level, rpg_mob_tier tier, int world_difficulty, int participants, double average_level);
double rpg_encounter_player_scale(int participants, double average_level, int content_level);

#endif
