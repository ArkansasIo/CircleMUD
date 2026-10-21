#ifndef CIRCLEMUD_RPG_ENCOUNTER_H
#define CIRCLEMUD_RPG_ENCOUNTER_H

#include "rpg_types.h"

rpg_encounter_scaling rpg_encounter_scale(int level, rpg_mob_tier tier, int world_difficulty, int participants, double average_level);
double rpg_encounter_player_scale(int participants, double average_level, int content_level);

/* Effective-power encounter budgeting. */
double rpg_encounter_budget_for_party(double player_effective_power_sum,
                                      int participants,
                                      int world_difficulty);
double rpg_encounter_boss_health(double base_hp, double level_diff,
                                  int participants, int world_difficulty);
double rpg_encounter_boss_damage(double base_damage, double level_diff,
                                  int world_difficulty);

#endif
