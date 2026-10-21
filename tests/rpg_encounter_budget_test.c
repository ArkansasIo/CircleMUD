#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../src/rpg/rpg_encounter.h"

static int close_enough(double a, double b) {
  return fabs(a - b) < 0.0001;
}

int main(void) {
  rpg_encounter_scaling normal;
  rpg_encounter_scaling boss;
  double solo_budget;
  double party_budget;
  double normal_boss_hp;
  double nightmare_boss_hp;
  double normal_boss_damage;
  double nightmare_boss_damage;

  normal = rpg_encounter_scale(50, RPG_MOB_NORMAL, 1, 1, 50.0);
  boss = rpg_encounter_scale(50, RPG_MOB_WORLD_BOSS, 9, 5, 50.0);

  assert(normal.health > 0.0);
  assert(normal.damage > 0.0);
  assert(boss.health > normal.health);
  assert(boss.damage > normal.damage);
  assert(boss.reward_scale > normal.reward_scale);

  solo_budget = rpg_encounter_budget_for_party(1000.0, 1, 1);
  party_budget = rpg_encounter_budget_for_party(1000.0, 5, 9);
  assert(solo_budget > 0.0);
  assert(party_budget > solo_budget);

  normal_boss_hp = rpg_encounter_boss_health(1000.0, 5.0, 1, 1);
  nightmare_boss_hp = rpg_encounter_boss_health(1000.0, 5.0, 5, 9);
  assert(nightmare_boss_hp > normal_boss_hp);

  normal_boss_damage = rpg_encounter_boss_damage(100.0, 5.0, 1);
  nightmare_boss_damage = rpg_encounter_boss_damage(100.0, 5.0, 9);
  assert(nightmare_boss_damage > normal_boss_damage);

  assert(close_enough(normal_boss_hp, 1000.0 * (1.0 + 5.0 * 0.18)));
  printf("rpg encounter budget tests passed\n");
  return 0;
}
