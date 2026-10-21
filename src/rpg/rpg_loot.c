#include "rpg_loot.h"

static const double POWER[10] = {0.0,1.00,1.15,1.35,1.65,2.05,2.55,3.20,4.10,5.25};
static const int AFFIXES[10] = {0,0,1,2,3,4,5,6,7,8};

int rpg_rarity_affix_count(rpg_rarity rarity) {
  if (rarity < 1 || rarity > RPG_RARITY_ETERNAL) return 0;
  return AFFIXES[rarity];
}

double rpg_rarity_power_multiplier(rpg_rarity rarity) {
  if (rarity < 1 || rarity > RPG_RARITY_ETERNAL) return 1.0;
  return POWER[rarity];
}

rpg_rarity rpg_roll_rarity(rpg_rng *rng, double loot_scale, int world_difficulty, int boss) {
  double roll = rpg_rng_unit(rng);
  double bonus = (loot_scale - 1.0) * 0.08 + (world_difficulty - 1) * 0.006 + (boss ? 0.08 : 0.0);
  double p[9] = {0.52,0.24,0.13,0.065,0.025,0.010,0.004,0.0015,0.0005};
  double cumulative = 0.0;
  int i;
  for (i = 0; i < 9; ++i) {
    double adjusted = p[i];
    if (i > 3) adjusted *= (1.0 + bonus * (double)(i - 2));
    cumulative += adjusted;
    if (roll <= cumulative) return (rpg_rarity)(i + 1);
  }
  return RPG_RARITY_ETERNAL;
}
