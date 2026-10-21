#include <assert.h>
#include <stdio.h>
#include "../src/rpg/rpg_combat.h"
#include "../src/rpg/rpg_catalog.h"

int main(void) {
  rpg_character attacker;
  rpg_monster target;
  rpg_rng rng;
  rpg_attack_result result;

  rpg_catalog_init();
  rpg_character_init(&attacker, 1);
  assert(rpg_character_set_level(&attacker, 50) == 1);
  assert(rpg_monster_init(&target, 100, 750, 1, 3) == 1);
  rpg_rng_seed(&rng, 12345u);

  result = rpg_calculate_attack(&attacker, &target, RPG_DAMAGE_PHYSICAL, &rng);
  assert(result.raw_damage > 0.0);
  assert(result.mitigated_damage > 0.0);
  assert(result.mitigated_damage <= result.raw_damage);

  assert(rpg_apply_attack(&attacker, &target, RPG_DAMAGE_FIRE, &rng, &result) == 1);
  assert(target.health < target.max_health);

  printf("rpg combat valuation tests passed\n");
  return 0;
}
