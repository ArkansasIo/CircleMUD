#include "rpg_equipment.h"

void rpg_equipment_init(rpg_equipment *equipment) {
  int i;
  if (!equipment) return;
  for (i = 0; i < RPG_EQUIPMENT_SLOTS; ++i) equipment->slots[i] = 0;
  equipment->attack_power = 0.0;
  equipment->spell_power = 0.0;
  equipment->armor = 0.0;
  equipment->critical = 0.0;
  equipment->haste = 0.0;
  equipment->movement = 1.0;
  equipment->pve_rating = 0.0;
  equipment->pvp_rating = 0.0;
}

int rpg_equipment_equip(rpg_equipment *equipment, rpg_equipment_slot slot, rpg_item *item) {
  if (!equipment || !item || slot < 0 || slot >= RPG_EQUIPMENT_SLOTS || !rpg_item_is_equipment(item)) return 0;
  equipment->slots[slot] = item;
  rpg_equipment_recalculate(equipment);
  return 1;
}

rpg_item *rpg_equipment_unequip(rpg_equipment *equipment, rpg_equipment_slot slot) {
  rpg_item *old;
  if (!equipment || slot < 0 || slot >= RPG_EQUIPMENT_SLOTS) return 0;
  old = equipment->slots[slot];
  equipment->slots[slot] = 0;
  rpg_equipment_recalculate(equipment);
  return old;
}

void rpg_equipment_recalculate(rpg_equipment *equipment) {
  int i;
  if (!equipment) return;
  equipment->attack_power = 0.0;
  equipment->spell_power = 0.0;
  equipment->armor = 0.0;
  equipment->critical = 0.0;
  equipment->haste = 0.0;
  equipment->movement = 1.0;
  equipment->pve_rating = 0.0;
  equipment->pvp_rating = 0.0;

  for (i = 0; i < RPG_EQUIPMENT_SLOTS; ++i) {
    rpg_item *item = equipment->slots[i];
    if (!item) continue;
    if (item->catalog.category == RPG_CATALOG_WEAPON) {
      equipment->attack_power += item->catalog.power;
      equipment->critical += item->catalog.power * 0.002;
      equipment->haste += item->catalog.power * 0.001;
    } else if (item->catalog.category == RPG_CATALOG_ARMOR) {
      equipment->armor += item->catalog.defense;
    }
    equipment->pve_rating += rpg_item_rating(item, RPG_CONTEXT_PVE);
    equipment->pvp_rating += rpg_item_rating(item, RPG_CONTEXT_PVP);
  }
}

double rpg_equipment_rating(const rpg_equipment *equipment, rpg_value_context context) {
  if (!equipment) return 0.0;
  return context == RPG_CONTEXT_PVP ? equipment->pvp_rating : equipment->pve_rating;
}
