#ifndef CIRCLEMUD_RPG_TYPES_H
#define CIRCLEMUD_RPG_TYPES_H

#include <stdint.h>

#define RPG_MAX_LEVEL 720
#define RPG_WORLD_DIFFICULTY_MIN 1
#define RPG_WORLD_DIFFICULTY_MAX 9
#define RPG_MAX_RARITY 9

typedef enum {
  RPG_MOB_NORMAL = 0,
  RPG_MOB_ELITE,
  RPG_MOB_RARE,
  RPG_MOB_CHAMPION,
  RPG_MOB_WORLD_BOSS
} rpg_mob_tier;

typedef enum {
  RPG_RARITY_COMMON = 1,
  RPG_RARITY_UNCOMMON,
  RPG_RARITY_RARE,
  RPG_RARITY_EPIC,
  RPG_RARITY_LEGENDARY,
  RPG_RARITY_MYTHIC,
  RPG_RARITY_ANCIENT,
  RPG_RARITY_TRANSCENDENT,
  RPG_RARITY_ETERNAL
} rpg_rarity;

typedef struct {
  int level;
  double xp_required;
  double power_scale;
} rpg_level_info;

typedef struct {
  int tier;
  const char *id;
  const char *name;
  double health;
  double damage;
  double armor;
  double loot;
  double mechanics;
  double experience;
  double spawn_rate;
  int elite_bonus;
  int boss_bonus;
  int hazard_level;
} rpg_world_difficulty;

typedef struct {
  uint32_t seed;
  uint32_t state;
} rpg_rng;

typedef struct {
  int level;
  rpg_mob_tier tier;
  int world_difficulty;
  double health;
  double damage;
  double armor;
  double reward_scale;
} rpg_encounter_scaling;

#endif
