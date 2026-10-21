#include "rpg_encounter.h"
#include "rpg_level.h"
#include "rpg_value.h"
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
  const rpg_world_difficulty *difficulty = rpg_world_difficulty_get(world_difficulty);
  double player = rpg_encounter_player_scale(participants, average_level, level);
  double tier_mult = tier_scale(tier);
  double world = difficulty->health;
  s.level = level;
  s.tier = tier;
  s.world_difficulty = world_difficulty;
  s.health = rpg_level_power_scale(level) * tier_mult * world * player;
  s.damage = s.health * (difficulty->damage / world);
  s.armor = (1.0 + level * 0.015) * difficulty->armor * tier_mult;
  s.reward_scale = difficulty->loot * tier_mult;
  return s;
}

double rpg_encounter_power_budget(double player_effective_power_sum,
                                  int participants,
                                  int world_difficulty) {
  const rpg_world_difficulty *difficulty = rpg_world_difficulty_get(world_difficulty);
  double party_multiplier = rpg_party_multiplier(participants);
  double difficulty_factor = difficulty->mechanics;
  if (player_effective_power_sum < 0.0) player_effective_power_sum = 0.0;
  return rpg_encounter_power_budget_value(player_effective_power_sum * party_multiplier,
                                          difficulty_factor);
}

double rpg_encounter_boss_health(double base_hp, double level_diff,
                                  int participants, int world_difficulty) {
  const rpg_world_difficulty *difficulty = rpg_world_difficulty_get(world_difficulty);
  return rpg_boss_hp(base_hp, level_diff, participants, difficulty->health);
}

double rpg_encounter_boss_damage(double base_damage, double level_diff,
                                  int world_difficulty) {
  const rpg_world_difficulty *difficulty = rpg_world_difficulty_get(world_difficulty);
  return rpg_boss_damage(base_damage, level_diff, difficulty->damage);
}
