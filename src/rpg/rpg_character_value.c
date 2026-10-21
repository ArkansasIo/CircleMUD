#include "rpg_character_value.h"

#include <math.h>

static double character_stat_power(const rpg_character *character,
                                   rpg_value_context context) {
  rpg_value_stats stats;
  double level_scale;

  if (!character) return 0.0;

  stats.str = character->stats.str;
  stats.dex = character->stats.dex;
  stats.con = character->stats.con;
  stats.intelligence = character->stats.intel;
  stats.wis = character->stats.wis;
  stats.vit = character->stats.vit;
  stats.spi = character->stats.spi;
  stats.luck = character->stats.lck;
  stats.wil = character->stats.wil;
  stats.cha = character->stats.cha;
  stats.crit_chance = character->stats.crit;
  stats.crit_damage = 0.0;
  stats.attack_speed = character->stats.haste;
  stats.cooldown_reduction = 0.0;
  stats.armor = character->stats.armor;
  stats.resistance = character->stats.armor * 0.50;
  stats.resilience = context == RPG_CONTEXT_PVP ? character->stats.armor * 0.25 : 0.0;
  stats.health = character->stats.max_health;
  stats.damage = character->stats.attack_power + character->stats.spell_power;
  stats.healing = character->stats.spell_power * 0.50;
  stats.cc_duration = 0.0;

  level_scale = 1.0 + ((double)(character->level > 0 ? character->level : 1) * 0.02);
  return rpg_item_power(&stats, context, NULL, 0, 0.0) * level_scale;
}

double rpg_character_rating(const rpg_character *character,
                            rpg_value_context context) {
  double base;
  double equipment;
  double survival;

  if (!character) return 0.0;

  base = character_stat_power(character, context);
  equipment = rpg_equipment_rating(&character->equipment, context);
  survival = rpg_defensive_power(character->stats.max_health,
                                 character->stats.armor,
                                 character->stats.armor * 0.50,
                                 0.010, 0.75, 0.90,
                                 context == RPG_CONTEXT_PVP ? 1.10 : 1.0);

  return fmax(0.0, base + equipment + survival);
}
