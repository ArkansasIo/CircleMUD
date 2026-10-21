#ifndef CIRCLEMUD_RPG_SYSTEM_H
#define CIRCLEMUD_RPG_SYSTEM_H

#include "rpg_types.h"

void rpg_system_init(void);
void rpg_system_update(double delta_seconds);
void rpg_system_set_world_difficulty(int tier);
int rpg_system_get_world_difficulty(void);
rpg_world_difficulty rpg_system_get_difficulty_info(void);

#endif
