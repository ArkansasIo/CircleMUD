#include <assert.h>
#include <stdio.h>
#include "rpg_catalog.h"
#include "rpg_item.h"
#include "rpg_character.h"
#include "rpg_monster.h"
#include "rpg_combat.h"
#include "rpg_crafting.h"

int main(void) {
  int i;
  rpg_item weapon, material;
  rpg_character character;
  rpg_monster monster;
  rpg_rng rng;
  rpg_attack_result attack;
  rpg_recipe recipe;

  assert(rpg_catalog_category_count(RPG_CATALOG_WEAPON) == 150);
  assert(rpg_catalog_category_count(RPG_CATALOG_ARMOR) == 150);
  assert(rpg_catalog_category_count(RPG_CATALOG_ITEM) == 200);
  assert(rpg_catalog_category_count(RPG_CATALOG_CONSUMABLE) == 150);
  assert(rpg_catalog_category_count(RPG_CATALOG_MATERIAL) == 100);
  assert(rpg_catalog_category_count(RPG_CATALOG_MONSTER) == 100);
  assert(rpg_catalog_category_count(RPG_CATALOG_NPC) == 50);
  for (i = 0; i < RPG_CATALOG_TOTAL; ++i) assert(rpg_catalog_get(i).index == i);

  assert(rpg_item_init(&weapon, 0, 1));
  assert(rpg_item_is_equipment(&weapon));
  assert(rpg_item_init(&material, 650, 2));
  assert(!rpg_item_is_equipment(&material));
  assert(rpg_item_add(&material, 10) == 10);

  rpg_character_init(&character, 100);
  assert(rpg_character_set_level(&character, 50));
  assert(character.stats.max_health > 0.0);
  assert(character.stats.attack_power > 0.0);
  assert(rpg_equipment_equip(&character.equipment, RPG_SLOT_CHEST, &weapon) == 1);

  assert(rpg_monster_init(&monster, 500, 799, 1, 5));
  rpg_rng_seed(&rng, 12345);
  assert(rpg_apply_attack(&character, &monster, RPG_DAMAGE_PHYSICAL, &rng, &attack));
  assert(attack.mitigated_damage >= 0.0);

  assert(rpg_recipe_init(&recipe, 1, 0, 10, 1));
  assert(rpg_recipe_add_material(&recipe, 650, 5));
  assert(rpg_recipe_can_craft(&recipe, &material, 1));

  puts("rpg_catalog_runtime: 900 entries and runtime systems validated");
  return 0;
}
