#ifndef CIRCLEMUD_RPG_WORLD_H
#define CIRCLEMUD_RPG_WORLD_H

#include "rpg_types.h"

#define RPG_MAX_WORLD_ENTITIES 2048

typedef struct {
  int id;
  int level;
  rpg_mob_tier tier;
  int world_difficulty;
  int zone_id;
  int active;
  double base_health;
  double base_damage;
  double base_armor;
  double health;
  double damage;
  double armor;
} rpg_world_entity;

void rpg_world_init(void);
int rpg_world_spawn(int id, int level, rpg_mob_tier tier, int difficulty, int zone_id, double health, double damage, double armor);
int rpg_world_despawn(int id);
rpg_world_entity *rpg_world_find(int id);
void rpg_world_update_entity(rpg_world_entity *entity, int participants, double average_level);
int rpg_world_active_count(void);

#endif
