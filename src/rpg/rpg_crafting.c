#include "rpg_crafting.h"

int rpg_recipe_init(rpg_recipe *recipe, int recipe_id, int output_catalog_index, int required_level, int profession) {
  int i;
  if (!recipe || !rpg_catalog_valid_index(output_catalog_index)) return 0;
  recipe->recipe_id = recipe_id;
  recipe->output_catalog_index = output_catalog_index;
  recipe->required_level = required_level;
  recipe->profession = profession;
  recipe->material_count = 0;
  for (i = 0; i < 4; ++i) { recipe->material_catalog_indices[i] = -1; recipe->material_counts[i] = 0; }
  return 1;
}

int rpg_recipe_add_material(rpg_recipe *recipe, int catalog_index, int count) {
  if (!recipe || !rpg_catalog_valid_index(catalog_index) || count <= 0 || recipe->material_count >= 4) return 0;
  recipe->material_catalog_indices[recipe->material_count] = catalog_index;
  recipe->material_counts[recipe->material_count] = count;
  ++recipe->material_count;
  return 1;
}

int rpg_recipe_can_craft(const rpg_recipe *recipe, const rpg_item *materials, int material_slots) {
  int r, i, found;
  if (!recipe || !materials || material_slots <= 0) return 0;
  for (r = 0; r < recipe->material_count; ++r) {
    found = 0;
    for (i = 0; i < material_slots; ++i) {
      if (materials[i].catalog.index == recipe->material_catalog_indices[r] && materials[i].stack >= recipe->material_counts[r]) {
        found = 1;
        break;
      }
    }
    if (!found) return 0;
  }
  return 1;
}
