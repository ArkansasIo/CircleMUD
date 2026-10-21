#ifndef CIRCLEMUD_RPG_CRAFTING_H
#define CIRCLEMUD_RPG_CRAFTING_H

#include "rpg_item.h"

typedef struct {
  int recipe_id;
  int output_catalog_index;
  int material_catalog_indices[4];
  int material_counts[4];
  int required_level;
  int profession;
  int material_count;
} rpg_recipe;

int rpg_recipe_init(rpg_recipe *recipe, int recipe_id, int output_catalog_index, int required_level, int profession);
int rpg_recipe_add_material(rpg_recipe *recipe, int catalog_index, int count);
int rpg_recipe_can_craft(const rpg_recipe *recipe, const rpg_item *materials, int material_slots);

#endif
