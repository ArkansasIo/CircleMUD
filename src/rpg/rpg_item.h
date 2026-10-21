#ifndef CIRCLEMUD_RPG_ITEM_H
#define CIRCLEMUD_RPG_ITEM_H

#include "rpg_catalog.h"
#include "rpg_value.h"

typedef struct {
  int id;
  rpg_catalog_entry catalog;
  int stack;
  int max_stack;
  int bound;
  double durability;
} rpg_item;

int rpg_item_init(rpg_item *item, int catalog_index, int instance_id);
int rpg_item_add(rpg_item *item, int amount);
int rpg_item_remove(rpg_item *item, int amount);
int rpg_item_is_equipment(const rpg_item *item);
double rpg_item_rating(const rpg_item *item, rpg_value_context context);

#endif
