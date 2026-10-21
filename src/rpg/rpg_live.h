#ifndef CIRCLEMUD_RPG_LIVE_H
#define CIRCLEMUD_RPG_LIVE_H

#include "rpg_character.h"
#include "rpg_monster.h"
#include "rpg_combat.h"
#include "rpg_loot_drop.h"

#define RPG_LIVE_MAX_PLAYERS 128
#define RPG_LIVE_MAX_MONSTERS 256
#define RPG_LIVE_MAX_DROPS 8

typedef struct {
  int active;
  int player_id;
  rpg_character character;
  double combat_cooldown;
  unsigned long long kills;
  unsigned long long loot_events;
} rpg_live_player;

typedef struct {
  int initialized;
  double elapsed_seconds;
  int next_monster_id;
  rpg_live_player players[RPG_LIVE_MAX_PLAYERS];
  rpg_monster monsters[RPG_LIVE_MAX_MONSTERS];
} rpg_live_state;

void rpg_live_init(void);
void rpg_live_update(double delta_seconds);
const rpg_live_state *rpg_live_get_state(void);

int rpg_live_register_player(int player_id, int level);
int rpg_live_unregister_player(int player_id);
rpg_character *rpg_live_get_character(int player_id);

int rpg_live_spawn_monster(int catalog_index, int zone_id, int difficulty);
int rpg_live_attack(int player_id, int monster_id, rpg_damage_type type);
int rpg_live_collect_loot(int player_id, int monster_id, rpg_loot_drop *drops, int max_drops);

#endif
