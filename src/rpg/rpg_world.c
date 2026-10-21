#include "rpg_world.h"
#include "rpg_encounter.h"

static rpg_world_entity entities[RPG_MAX_WORLD_ENTITIES];

void rpg_world_init(void) {
  int i;
  for (i = 0; i < RPG_MAX_WORLD_ENTITIES; ++i) entities[i].active = 0;
}

rpg_world_entity *rpg_world_find(int id) {
  int i;
  for (i = 0; i < RPG_MAX_WORLD_ENTITIES; ++i)
    if (entities[i].active && entities[i].id == id) return &entities[i];
  return 0;
}

int rpg_world_spawn(int id, int level, rpg_mob_tier tier, int difficulty, int zone_id, double health, double damage, double armor) {
  int i;
  if (rpg_world_find(id)) return 0;
  for (i = 0; i < RPG_MAX_WORLD_ENTITIES; ++i) {
    if (!entities[i].active) {
      entities[i].id = id;
      entities[i].level = level;
      entities[i].tier = tier;
      entities[i].world_difficulty = difficulty;
      entities[i].zone_id = zone_id;
      entities[i].active = 1;
      entities[i].base_health = health;
      entities[i].base_damage = damage;
      entities[i].base_armor = armor;
      entities[i].health = health;
      entities[i].damage = damage;
      entities[i].armor = armor;
      return 1;
    }
  }
  return 0;
}

int rpg_world_despawn(int id) {
  rpg_world_entity *e = rpg_world_find(id);
  if (!e) return 0;
  e->active = 0;
  return 1;
}

void rpg_world_update_entity(rpg_world_entity *entity, int participants, double average_level) {
  rpg_encounter_scaling s;
  if (!entity || !entity->active) return;
  s = rpg_encounter_scale(entity->level, entity->tier, entity->world_difficulty, participants, average_level);
  entity->health = entity->base_health * s.health;
  entity->damage = entity->base_damage * s.damage;
  entity->armor = entity->base_armor * s.armor;
}

int rpg_world_active_count(void) {
  int i, count = 0;
  for (i = 0; i < RPG_MAX_WORLD_ENTITIES; ++i) if (entities[i].active) ++count;
  return count;
}
