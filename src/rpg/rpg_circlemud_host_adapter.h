#ifndef CIRCLEMUD_RPG_CIRCLEMUD_HOST_ADAPTER_H
#define CIRCLEMUD_RPG_CIRCLEMUD_HOST_ADAPTER_H

#include "rpg_circlemud_combat_bridge.h"

typedef struct {
  int (*apply_damage)(int player_id, int monster_id, int source_key, int amount);
  int (*apply_death)(int player_id, int monster_id, int source_key);
  int (*grant_xp)(int player_id, int monster_id, int amount);
  int (*materialize_loot)(int player_id, int monster_id, int catalog_index);
} rpg_circlemud_host_callbacks;

void rpg_circlemud_host_adapter_init(void);
int rpg_circlemud_host_adapter_register(const rpg_circlemud_host_callbacks *callbacks);
int rpg_circlemud_host_adapter_available(void);
int rpg_circlemud_host_adapter_apply(const rpg_circlemud_combat_event *event);

#endif
