#ifndef CIRCLEMUD_RPG_COMBAT_H
#define CIRCLEMUD_RPG_COMBAT_H

#include "rpg_character.h"
#include "rpg_monster.h"

typedef enum {
  RPG_DAMAGE_PHYSICAL = 0,
  RPG_DAMAGE_ARCANE,
  RPG_DAMAGE_FROST,
  RPG_DAMAGE_FIRE,
  RPG_DAMAGE_SHADOW,
  RPG_DAMAGE_NATURE
} rpg_damage_type;

typedef struct {
  double raw_damage;
  double mitigated_damage;
  double critical_multiplier;
  int critical;
} rpg_attack_result;

rpg_attack_result rpg_calculate_attack(const rpg_character *attacker, const rpg_monster *target, rpg_damage_type type, rpg_rng *rng);
int rpg_apply_attack(const rpg_character *attacker, rpg_monster *target, rpg_damage_type type, rpg_rng *rng, rpg_attack_result *result);

#endif
