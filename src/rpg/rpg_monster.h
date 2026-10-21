#ifndef CIRCLEMUD_RPG_MONSTER_H
#define CIRCLEMUD_RPG_MONSTER_H

#include "rpg_catalog.h"
#include "rpg_world.h"

typedef struct {
  int id;
  int catalog_index;
  int zone_id;
  rpg_mob_tier tier;
  int level;
  int world_difficulty;
  double health;
  double max_health;
  double damage;
  double armor;
  double experience_reward;
  double loot_scale;
  int alive;
} rpg_monster;

int rpg_monster_init(rpg_monster *monster, int id, int catalog_index, int zone_id, int difficulty);
void rpg_monster_scale(rpg_monster *monster, int participants, double average_level);
int rpg_monster_damage(rpg_monster *monster, double amount);
int rpg_monster_is_boss(const rpg_monster *monster);

#endif
