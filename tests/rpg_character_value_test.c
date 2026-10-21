#include <assert.h>
#include <stdio.h>

#include "../src/rpg/rpg_character_value.h"

int main(void) {
  rpg_character character;
  double pve;
  double pvp;

  rpg_character_init(&character, 9001);
  pve = rpg_character_rating(&character, RPG_CONTEXT_PVE);
  pvp = rpg_character_rating(&character, RPG_CONTEXT_PVP);

  assert(pve > 0.0);
  assert(pvp > 0.0);
  assert(pve != pvp);

  assert(rpg_character_set_level(&character, 50) == 1);
  assert(rpg_character_rating(&character, RPG_CONTEXT_PVE) > pve);

  printf("rpg character valuation tests passed\n");
  return 0;
}
