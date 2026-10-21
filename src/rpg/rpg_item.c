#include "rpg_item.h"

int rpg_item_init(rpg_item *item, int catalog_index, int instance_id) {
  if (!item || !rpg_catalog_valid_index(catalog_index)) return 0;
  item->id = instance_id;
  item->catalog = rpg_catalog_get(catalog_index);
  item->stack = 1;
  item->max_stack = (item->catalog.category == RPG_CATALOG_MATERIAL) ? 999 : 20;
  item->bound = item->catalog.rarity >= RPG_RARITY_LEGENDARY;
  item->durability = 100.0;
  return 1;
}

int rpg_item_add(rpg_item *item, int amount) {
  int accepted;
  if (!item || amount <= 0) return 0;
  accepted = item->max_stack - item->stack;
  if (accepted <= 0) return 0;
  if (amount > accepted) amount = accepted;
  item->stack += amount;
  return amount;
}

int rpg_item_remove(rpg_item *item, int amount) {
  if (!item || amount <= 0) return 0;
  if (amount > item->stack) amount = item->stack;
  item->stack -= amount;
  return amount;
}

int rpg_item_is_equipment(const rpg_item *item) {
  if (!item) return 0;
  return item->catalog.category == RPG_CATALOG_WEAPON || item->catalog.category == RPG_CATALOG_ARMOR;
}

double rpg_item_rating(const rpg_item *item, rpg_value_context context) {
  rpg_value_stats stats;
  double rarity_scale;
  if (!item) return 0.0;

  stats.str = stats.dex = stats.con = stats.intelligence = 0.0;
  stats.wis = stats.vit = stats.spi = stats.luck = stats.wil = stats.cha = 0.0;
  stats.crit_chance = stats.crit_damage = stats.attack_speed = stats.cooldown_reduction = 0.0;
  stats.armor = stats.resistance = stats.resilience = stats.health = 0.0;
  stats.damage = stats.healing = stats.cc_duration = 0.0;

  rarity_scale = 1.0 + ((double)item->catalog.rarity * 0.10);
  if (item->catalog.category == RPG_CATALOG_WEAPON) {
    stats.damage = item->catalog.power * rarity_scale;
    return rpg_item_power(context, &stats, NULL, 0.0);
  }
  if (item->catalog.category == RPG_CATALOG_ARMOR) {
    stats.armor = item->catalog.defense * rarity_scale;
    stats.health = item->catalog.power * 0.5 * rarity_scale;
    return rpg_item_power(context, &stats, NULL, 0.0);
  }
  stats.health = item->catalog.power * rarity_scale;
  return rpg_item_power(context, &stats, NULL, item->catalog.value * 0.01);
}
