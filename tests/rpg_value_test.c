#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../src/rpg/rpg_value.h"

static int near(double a, double b) { return fabs(a - b) < 0.000001; }

int main(void) {
  rpg_value_proc proc = {300.0, 0.25, 1.2};
  rpg_value_stats stats = {0};
  double crit = 0.90, cdr = 0.80, cc = 0.90, dr = 0.90, heal = 0.80;

  assert(near(rpg_stat_weight("str", RPG_CONTEXT_PVE), 1.0));
  assert(near(rpg_stat_weight("damage", RPG_CONTEXT_PVP), 0.70));
  assert(near(rpg_proc_value(&proc), 90.0));
  assert(near(rpg_weapon_power_pve(1200.0, 1.1, 50.0, 2.5, 120.0, 1.3, 1.0), 1486.0));
  assert(near(rpg_weapon_power_pvp(650.0, 50.0, 2.5, 0.65, 0.80), 572.5));
  assert(near(rpg_party_multiplier(1), 1.0));
  assert(near(rpg_party_multiplier(5), 3.6));
  assert(near(rpg_boss_hp(1000.0, 2.0, 5, 1.5), 6264.0));
  assert(near(rpg_boss_damage(1000.0, 2.0, 1.5), 1860.0));

  stats.str = 50.0;
  stats.crit_chance = 5.0;
  stats.damage = 120.0;
  assert(rpg_item_power(&stats, RPG_CONTEXT_PVE, &proc, 1, 0.0) > 0.0);

  assert(rpg_value_clamp_caps(&crit, &cdr, &cc, &dr, &heal, RPG_CONTEXT_PVP));
  assert(near(crit, 0.40));
  assert(near(cdr, 0.35));
  assert(near(cc, 0.50));
  assert(near(dr, 0.70));
  assert(near(heal, 0.50));

  puts("rpg_value_test: PASS");
  return 0;
}
