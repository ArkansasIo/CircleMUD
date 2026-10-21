#include "rpg_character.h"
#include "rpg_level.h"

void rpg_character_init(rpg_character *character, int id) {
  if (!character) return;
  character->id = id;
  character->level = 1;
  character->xp = 0.0;
  character->race_id = 0;
  character->class_id = 0;
  character->specialization_id = 0;
  character->stats.str = 10.0; character->stats.dex = 10.0;
  character->stats.con = 10.0; character->stats.intel = 10.0;
  character->stats.wis = 10.0; character->stats.vit = 10.0;
  character->stats.spi = 10.0; character->stats.lck = 10.0;
  character->stats.wil = 10.0; character->stats.cha = 10.0;
  rpg_equipment_init(&character->equipment);
  rpg_character_recalculate(character);
}

int rpg_character_set_level(rpg_character *character, int level) {
  if (!character || !rpg_level_valid(level)) return 0;
  character->level = level;
  rpg_character_recalculate(character);
  return 1;
}

void rpg_character_recalculate(rpg_character *character) {
  double power;
  if (!character) return;
  rpg_equipment_recalculate(&character->equipment);
  power = rpg_level_power_scale(character->level);
  character->stats.max_health = character->stats.vit * 15.0 + character->stats.con * 10.0 + power * 20.0;
  character->stats.max_mana = character->stats.intel * 12.0 + character->stats.spi * 8.0 + power * 10.0;
  character->stats.max_stamina = character->stats.con * 8.0 + character->stats.vit * 6.0 + power * 8.0;
  character->stats.attack_power = power + character->stats.str * 2.0 + character->equipment.attack_power;
  character->stats.spell_power = power + character->stats.intel * 2.0 + character->equipment.spell_power;
  character->stats.armor = character->stats.con + character->stats.vit + character->equipment.armor;
  character->stats.crit = character->stats.dex * 0.002 + character->equipment.critical;
  character->stats.haste = character->stats.dex * 0.001 + character->equipment.haste;
  character->stats.dodge = character->stats.dex * 0.0015;
  if (character->stats.health <= 0.0) character->stats.health = character->stats.max_health;
  if (character->stats.mana <= 0.0) character->stats.mana = character->stats.max_mana;
  if (character->stats.stamina <= 0.0) character->stats.stamina = character->stats.max_stamina;
}

int rpg_character_gain_xp(rpg_character *character, double amount) {
  int levels = 0;
  if (!character || amount <= 0.0) return 0;
  character->xp += amount;
  while (character->level < RPG_MAX_LEVEL && character->xp >= rpg_level_xp_required(character->level)) {
    character->xp -= rpg_level_xp_required(character->level);
    ++character->level;
    ++levels;
  }
  if (levels) rpg_character_recalculate(character);
  return levels;
}
