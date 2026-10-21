#ifndef CIRCLEMUD_RPG_CIRCLEMUD_HOST_RESOLVER_H
#define CIRCLEMUD_RPG_CIRCLEMUD_HOST_RESOLVER_H

#include "rpg_circlemud_combat_bridge.h"

/* Install the production CircleMUD callbacks after the legacy world exists. */
void rpg_circlemud_install_host_adapter(void);

/* Resolve a player and NPC source identity without retaining pointers. */
int rpg_circlemud_host_player_exists(int player_id);
int rpg_circlemud_host_monster_exists(int monster_id, int source_key);

#endif
