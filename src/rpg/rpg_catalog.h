#ifndef CIRCLEMUD_RPG_CATALOG_H
#define CIRCLEMUD_RPG_CATALOG_H

#include "rpg_types.h"

#define RPG_CATALOG_TOTAL 900
#define RPG_CATALOG_WEAPONS 150
#define RPG_CATALOG_ARMOR_COUNT 150
#define RPG_CATALOG_ITEMS 200
#define RPG_CATALOG_CONSUMABLES 150
#define RPG_CATALOG_MATERIALS 100
#define RPG_CATALOG_MONSTERS 100
#define RPG_CATALOG_NPCS 50

typedef enum {
  RPG_CATALOG_WEAPON = 0,
  RPG_CATALOG_ARMOR,
  RPG_CATALOG_ITEM,
  RPG_CATALOG_CONSUMABLE,
  RPG_CATALOG_MATERIAL,
  RPG_CATALOG_MONSTER,
  RPG_CATALOG_NPC
} rpg_catalog_category;

typedef struct {
  int index;
  rpg_catalog_category category;
  int level;
  rpg_rarity rarity;
  int subtype;
  double power;
  double defense;
  double value;
} rpg_catalog_entry;

int rpg_catalog_valid_index(int index);
int rpg_catalog_category_count(rpg_catalog_category category);
rpg_catalog_entry rpg_catalog_get(int index);
const char *rpg_catalog_category_name(rpg_catalog_category category);

#endif
