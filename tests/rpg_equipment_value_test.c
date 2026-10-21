#include <assert.h>
#include <stdio.h>
#include "../src/rpg/rpg_equipment.h"
#include "../src/rpg/rpg_catalog.h"

int main(void) {
  rpg_item weapon;
  rpg_item armor;
  rpg_equipment equipment;

  assert(rpg_item_init(&weapon, 0, 1001) == 1);
  assert(rpg_item_init(&armor, RPG_CATALOG_WEAPONS, 1002) == 1);
  assert(rpg_item_is_equipment(&weapon) == 1);
  assert(rpg_item_is_equipment(&armor) == 1);

  rpg_equipment_init(&equipment);
  assert(rpg_equipment_equip(&equipment, RPG_SLOT_CHEST, &armor) == 1);
  assert(rpg_equipment_equip(&equipment, RPG_SLOT_HANDS, &weapon) == 1);
  assert(equipment.attack_power > 0.0);
  assert(equipment.armor > 0.0);
  assert(equipment.pve_rating > 0.0);
  assert(equipment.pvp_rating > 0.0);
  assert(rpg_equipment_rating(&equipment, RPG_CONTEXT_PVE) == equipment.pve_rating);
  assert(rpg_equipment_rating(&equipment, RPG_CONTEXT_PVP) == equipment.pvp_rating);
  assert(equipment.pve_rating != equipment.pvp_rating);

  assert(rpg_equipment_unequip(&equipment, RPG_SLOT_HANDS) == &weapon);
  assert(equipment.attack_power == 0.0);
  assert(equipment.armor > 0.0);
  assert(equipment.pve_rating > 0.0);

  printf("rpg equipment valuation tests passed\n");
  return 0;
}
