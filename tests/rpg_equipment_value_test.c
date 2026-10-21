#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/rpg/rpg_equipment.h"
#include "../src/rpg/rpg_catalog.h"

int main(void) {
  rpg_catalog_entry weapon_catalog;
  rpg_catalog_entry armor_catalog;
  rpg_item weapon;
  rpg_item armor;
  rpg_equipment equipment;

  memset(&weapon_catalog, 0, sizeof(weapon_catalog));
  weapon_catalog.index = 0;
  weapon_catalog.category = RPG_CATALOG_WEAPON;
  weapon_catalog.level = 50;
  weapon_catalog.rarity = 3;
  weapon_catalog.power = 500.0;
  weapon_catalog.value = 1000.0;

  memset(&armor_catalog, 0, sizeof(armor_catalog));
  armor_catalog.index = 1;
  armor_catalog.category = RPG_CATALOG_ARMOR;
  armor_catalog.level = 50;
  armor_catalog.rarity = 4;
  armor_catalog.power = 300.0;
  armor_catalog.defense = 800.0;
  armor_catalog.value = 900.0;

  rpg_item_init(&weapon, &weapon_catalog);
  rpg_item_init(&armor, &armor_catalog);
  rpg_equipment_init(&equipment);

  assert(rpg_equipment_equip(&equipment, RPG_SLOT_CHEST, &armor) == 1);
  assert(rpg_equipment_equip(&equipment, RPG_SLOT_HANDS, &weapon) == 1);
  assert(equipment.attack_power == 500.0);
  assert(equipment.armor == 800.0);
  assert(equipment.pve_rating > 0.0);
  assert(equipment.pvp_rating > 0.0);
  assert(rpg_equipment_rating(&equipment, RPG_CONTEXT_PVE) == equipment.pve_rating);
  assert(rpg_equipment_rating(&equipment, RPG_CONTEXT_PVP) == equipment.pvp_rating);
  assert(equipment.pve_rating != equipment.pvp_rating);

  assert(rpg_equipment_unequip(&equipment, RPG_SLOT_HANDS) == &weapon);
  assert(equipment.attack_power == 0.0);
  assert(equipment.pve_rating > 0.0);

  printf("rpg equipment valuation tests passed\n");
  return 0;
}
