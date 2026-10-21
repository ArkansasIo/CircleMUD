#ifndef CIRCLEMUD_RPG_EQUIPMENT_H
#define CIRCLEMUD_RPG_EQUIPMENT_H

#include "rpg_item.h"
#include "rpg_value.h"

#define RPG_EQUIPMENT_SLOTS 12

typedef enum {
  RPG_SLOT_HEAD = 0, RPG_SLOT_CHEST, RPG_SLOT_SHOULDERS, RPG_SLOT_HANDS,
  RPG_SLOT_LEGS, RPG_SLOT_FEET, RPG_SLOT_CLOAK, RPG_SLOT_BRACERS,
  RPG_SLOT_BELT, RPG_SLOT_RING, RPG_SLOT_AMULET, RPG_SLOT_TRINKET
} rpg_equipment_slot;

typedef struct {
  rpg_item *slots[RPG_EQUIPMENT_SLOTS];
  double attack_power;
  double spell_power;
  double armor;
  double critical;
  double haste;
  double movement;
  double pve_rating;
  double pvp_rating;
} rpg_equipment;

void rpg_equipment_init(rpg_equipment *equipment);
int rpg_equipment_equip(rpg_equipment *equipment, rpg_equipment_slot slot, rpg_item *item);
rpg_item *rpg_equipment_unequip(rpg_equipment *equipment, rpg_equipment_slot slot);
void rpg_equipment_recalculate(rpg_equipment *equipment);
double rpg_equipment_rating(const rpg_equipment *equipment, rpg_value_context context);

#endif
