#include "rpg_encounter.h"
#include "rpg_level.h"
#include "rpg_world_difficulty.h"

static double tier_scale(rpg_mob_tier tier) {
  switch (tier) {
    case RPG_MOB_ELITE: return 1.20;
    case RPG_MOB_RARE: return 1.45;
    case RPG_MOB_CHAMPION: return 1.80;
    case RPG_MOB_WORLD_BOSS: return 2.50;
    default: return 1.00;
  }
}

double rpg_encounter_player_scale(int participants, double average_level, int content_level) {
  double party = 1.0 + (participants > 1 ? (participants - 1) * 0.10 : 0.0);
  double level = content_level > 0 ? average_level / (double)content_level : 1.0;
  if (level < 0.75) level = 0.75;
  if (level > 2.00) level = 2.00;
  return party * level;
}

rpg_encounter_scaling rpg_encounter_scale(int level, rpg_mob_tier tier, int world_difficulty, int participants, double average_level) {
  rpg_encounter_scaling s;
  double player = rpg_encounter_player_scale(participants, average_level, level);
  double tier_mult = tier_scale(tier);
  double world = rpg_world_difficulty_get(world_difficulty)->health;
  s.level = level;
  s.tier = tier;
  s.world_difficulty = world_difficulty;
  s.health = rpg_level_power_scale(level) * tier_mult * world * player;
  s.damage = s.health * (rpg_world_difficulty_get(world_difficulty)->damage / world);
  s.armor = (1.0 + level * 0.015) * rpg_world_difficulty_get(world_difficulty)->armor * tier_mult;
  s.reward_scale = rpg_world_difficulty_get(world_difficulty)->loot * tier_mult;
  return s;
}
