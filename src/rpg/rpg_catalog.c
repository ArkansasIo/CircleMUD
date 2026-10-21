#include "rpg_catalog.h"
#include "rpg_level.h"
#include "rpg_loot.h"

static int category_for_index(int index) {
  if (index < 150) return RPG_CATALOG_WEAPON;
  if (index < 300) return RPG_CATALOG_ARMOR;
  if (index < 500) return RPG_CATALOG_ITEM;
  if (index < 650) return RPG_CATALOG_CONSUMABLE;
  if (index < 750) return RPG_CATALOG_MATERIAL;
  if (index < 850) return RPG_CATALOG_MONSTER;
  return RPG_CATALOG_NPC;
}

static int local_index(int index, rpg_catalog_category category) {
  static const int base[] = {0,150,300,500,650,750,850};
  return index - base[category];
}

int rpg_catalog_valid_index(int index) { return index >= 0 && index < RPG_CATALOG_TOTAL; }

int rpg_catalog_category_count(rpg_catalog_category category) {
  static const int counts[] = {150,150,200,150,100,100,50};
  if (category < RPG_CATALOG_WEAPON || category > RPG_CATALOG_NPC) return 0;
  return counts[category];
}

const char *rpg_catalog_category_name(rpg_catalog_category category) {
  static const char *names[] = {"weapon","armor","item","consumable","material","monster","npc"};
  if (category < RPG_CATALOG_WEAPON || category > RPG_CATALOG_NPC) return "unknown";
  return names[category];
}

rpg_catalog_entry rpg_catalog_get(int index) {
  rpg_catalog_entry e;
  int local;
  e.index = index;
  e.category = RPG_CATALOG_ITEM;
  e.level = 1;
  e.rarity = RPG_RARITY_COMMON;
  e.subtype = 0;
  e.power = 1.0;
  e.defense = 0.0;
  e.value = 1.0;
  if (!rpg_catalog_valid_index(index)) return e;
  e.category = (rpg_catalog_category)category_for_index(index);
  local = local_index(index, e.category);
  e.level = ((local * 7) + ((index + 1) * 3)) % RPG_MAX_LEVEL + 1;
  e.rarity = (rpg_rarity)((local * 9) / rpg_catalog_category_count(e.category) + 1);
  if (e.rarity > RPG_RARITY_ETERNAL) e.rarity = RPG_RARITY_ETERNAL;
  e.subtype = local % 16;
  e.power = rpg_level_power_scale(e.level) * rpg_rarity_power_multiplier(e.rarity);
  e.defense = e.category == RPG_CATALOG_ARMOR ? e.power * 0.65 : 0.0;
  e.value = e.power * (10.0 + (double)(local % 25));
  if (e.category == RPG_CATALOG_MONSTER) e.defense = e.power * (1.0 + (local % 5) * 0.2);
  return e;
}
