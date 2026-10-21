#include "rpg_monster.h"
#include "rpg_encounter.h"
#include "rpg_world_difficulty.h"
#include "rpg_level.h"

static rpg_mob_tier tier_from_catalog(int local) {
  switch (local % 5) {
    case 1: return RPG_MOB_ELITE;
    case 2: return RPG_MOB_RARE;
    case 3: return RPG_MOB_CHAMPION;
    case 4: return RPG_MOB_WORLD_BOSS;
    default: return RPG_MOB_NORMAL;
  }
}

int rpg_monster_init(rpg_monster *monster, int id, int catalog_index, int zone_id, int difficulty) {
  rpg_catalog_entry e;
  rpg_encounter_scaling s;
  if (!monster || catalog_index < 750 || catalog_index >= 850) return 0;
  e = rpg_catalog_get(catalog_index);
  monster->id = id;
  monster->catalog_index = catalog_index;
  monster->zone_id = zone_id;
  monster->tier = tier_from_catalog(catalog_index - 750);
  monster->level = e.level;
  monster->world_difficulty = difficulty;
  s = rpg_encounter_scale(monster->level, monster->tier, difficulty, 1, (double)monster->level);
  monster->health = 100.0 * e.power * s.health;
  monster->max_health = monster->health;
  monster->damage = 10.0 * e.power * s.damage;
  monster->armor = 5.0 * e.defense * s.armor;
  monster->experience_reward = rpg_level_xp_required(monster->level) * 0.25 * s.reward_scale;
  monster->loot_scale = s.reward_scale;
  monster->alive = 1;
  return 1;
}

void rpg_monster_scale(rpg_monster *monster, int participants, double average_level) {
  rpg_encounter_scaling s;
  rpg_catalog_entry e;
  if (!monster || !monster->alive) return;
  e = rpg_catalog_get(monster->catalog_index);
  s = rpg_encounter_scale(monster->level, monster->tier, monster->world_difficulty, participants, average_level);
  monster->health = 100.0 * e.power * s.health;
  monster->max_health = monster->health;
  monster->damage = 10.0 * e.power * s.damage;
  monster->armor = 5.0 * e.defense * s.armor;
  monster->experience_reward = rpg_level_xp_required(monster->level) * 0.25 * s.reward_scale;
  monster->loot_scale = s.reward_scale;
}

int rpg_monster_damage(rpg_monster *monster, double amount) {
  if (!monster || !monster->alive || amount <= 0.0) return 0;
  if (amount > monster->health) amount = monster->health;
  monster->health -= amount;
  if (monster->health <= 0.0) { monster->health = 0.0; monster->alive = 0; }
  return 1;
}

int rpg_monster_is_boss(const rpg_monster *monster) {
  return monster && monster->tier == RPG_MOB_WORLD_BOSS;
}
