#include <assert.h>
#include <stdio.h>
#include "rpg_item.h"

int main(void) {
  rpg_item weapon;
  rpg_item armor;

  assert(rpg_item_init(&weapon, 0, 1001));
  assert(rpg_item_init(&armor, RPG_CATALOG_WEAPONS, 1002));
  assert(rpg_item_is_equipment(&weapon));
  assert(rpg_item_is_equipment(&armor));
  assert(rpg_item_rating(&weapon, RPG_CONTEXT_PVE) > 0.0);
  assert(rpg_item_rating(&weapon, RPG_CONTEXT_PVP) > 0.0);
  assert(rpg_item_rating(&armor, RPG_CONTEXT_PVE) > 0.0);

  puts("rpg item value tests passed");
  return 0;
}
