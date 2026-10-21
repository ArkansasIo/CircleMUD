#ifndef CIRCLEMUD_RPG_LOOT_DROP_H
#define CIRCLEMUD_RPG_LOOT_DROP_H

#include "rpg_item.h"
#include "rpg_rng.h"

typedef struct {
  int item_catalog_index;
  rpg_rarity rarity;
  int quantity;
  double drop_chance;
} rpg_loot_drop;

int rpg_roll_monster_loot(int monster_catalog_index, int world_difficulty, double loot_scale, int boss, rpg_rng *rng, rpg_loot_drop *drops, int max_drops);

#endif
