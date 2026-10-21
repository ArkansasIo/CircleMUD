#include "rpg_ability.h"

int rpg_ability_init(rpg_ability *ability, int id, int class_id, int specialization_id, int required_level, rpg_ability_type type) {
  if (!ability || required_level < 1 || required_level > 720) return 0;
  ability->id = id;
  ability->class_id = class_id;
  ability->specialization_id = specialization_id;
  ability->required_level = required_level;
  ability->type = type;
  ability->power_scale = 1.0 + required_level * 0.02;
  ability->resource_cost = 5.0 + required_level * 0.15;
  ability->cooldown = type == RPG_ABILITY_ULTIMATE ? 120.0 : type == RPG_ABILITY_TOGGLE ? 0.0 : 2.0 + required_level * 0.01;
  ability->enabled = 1;
  return 1;
}

int rpg_ability_can_use(const rpg_ability *ability, int character_level, double resource, double elapsed_cooldown) {
  if (!ability || !ability->enabled) return 0;
  return character_level >= ability->required_level && resource >= ability->resource_cost && elapsed_cooldown >= ability->cooldown;
}
