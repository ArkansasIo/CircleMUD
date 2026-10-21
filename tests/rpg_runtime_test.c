#include <assert.h>
#include <stdio.h>
#include "../src/rpg/rpg_level.h"
#include "../src/rpg/rpg_world_difficulty.h"
#include "../src/rpg/rpg_rng.h"
#include "../src/rpg/rpg_loot.h"
#include "../src/rpg/rpg_encounter.h"

int main(void) {
  rpg_rng rng;
  rpg_encounter_scaling s;
  assert(rpg_level_valid(1));
  assert(rpg_level_valid(720));
  assert(!rpg_level_valid(721));
  assert(rpg_level_xp_required(10) > rpg_level_xp_required(1));
  assert(rpg_level_milestone(100));
  assert(rpg_level_milestone(720));
  assert(rpg_world_difficulty_valid(9));
  assert(!rpg_world_difficulty_valid(10));
  assert(rpg_world_health_multiplier(9) > rpg_world_health_multiplier(1));
  rpg_rng_seed(&rng, 12345u);
  s = rpg_encounter_scale(720, RPG_MOB_WORLD_BOSS, 9, 20, 720.0);
  assert(s.health > 0.0 && s.damage > 0.0 && s.reward_scale > 0.0);
  assert(rpg_roll_rarity(&rng, 2.0, 9, 1) >= RPG_RARITY_COMMON);
  printf("RPG runtime tests passed\n");
  return 0;
}
