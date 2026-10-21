#include "rpg_loot_drop.h"

int rpg_roll_monster_loot(int monster_catalog_index, int world_difficulty, double loot_scale, int boss, rpg_rng *rng, rpg_loot_drop *drops, int max_drops) {
  int count = 0;
  int rolls, i;
  if (!rng || !drops || max_drops <= 0 || monster_catalog_index < 750 || monster_catalog_index >= 850) return 0;
  rolls = boss ? 5 : 2 + (world_difficulty >= 5 ? 1 : 0);
  for (i = 0; i < rolls && count < max_drops; ++i) {
    double chance = 0.35 + loot_scale * 0.08;
    if (rpg_rng_unit(rng) > chance) continue;
    {
      int item_index = 0 + rpg_rng_range(rng, 0, 649);
      rpg_loot_drop *drop = &drops[count++];
      drop->item_catalog_index = item_index;
      drop->rarity = rpg_roll_rarity(rng, loot_scale, world_difficulty, boss);
      drop->quantity = 1 + rpg_rng_range(rng, 0, boss ? 3 : 1);
      drop->drop_chance = chance;
    }
  }
  return count;
}
