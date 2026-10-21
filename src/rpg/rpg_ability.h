#ifndef CIRCLEMUD_RPG_ABILITY_H
#define CIRCLEMUD_RPG_ABILITY_H

typedef enum { RPG_ABILITY_ACTIVE = 0, RPG_ABILITY_PASSIVE, RPG_ABILITY_TOGGLE, RPG_ABILITY_ULTIMATE } rpg_ability_type;

typedef struct {
  int id;
  int class_id;
  int specialization_id;
  int required_level;
  rpg_ability_type type;
  double power_scale;
  double resource_cost;
  double cooldown;
  int enabled;
} rpg_ability;

int rpg_ability_init(rpg_ability *ability, int id, int class_id, int specialization_id, int required_level, rpg_ability_type type);
int rpg_ability_can_use(const rpg_ability *ability, int character_level, double resource, double elapsed_cooldown);

#endif
