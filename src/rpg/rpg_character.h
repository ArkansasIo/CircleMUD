#ifndef CIRCLEMUD_RPG_CHARACTER_H
#define CIRCLEMUD_RPG_CHARACTER_H

#include "rpg_equipment.h"

typedef struct {
  double str, dex, con, intel, wis, vit, spi, lck, wil, cha;
  double attack_power, spell_power, armor, crit, haste, dodge;
  double max_health, health, max_mana, mana, max_stamina, stamina;
} rpg_character_stats;

typedef struct {
  int id;
  int level;
  double xp;
  int race_id;
  int class_id;
  int specialization_id;
  rpg_character_stats stats;
  rpg_equipment equipment;
} rpg_character;

void rpg_character_init(rpg_character *character, int id);
int rpg_character_set_level(rpg_character *character, int level);
void rpg_character_recalculate(rpg_character *character);
int rpg_character_gain_xp(rpg_character *character, double amount);

#endif
