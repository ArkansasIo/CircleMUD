#include "rpg_live.h"
#include "rpg_rng.h"

#include <string.h>

static rpg_live_state state;
static rpg_rng rng;

static rpg_live_player *find_player(int player_id) {
  int i;
  for (i = 0; i < RPG_LIVE_MAX_PLAYERS; ++i)
    if (state.players[i].active && state.players[i].player_id == player_id)
      return &state.players[i];
  return NULL;
}

static rpg_monster *find_monster(int monster_id) {
  int i;
  for (i = 0; i < RPG_LIVE_MAX_MONSTERS; ++i)
    if (state.monsters[i].id == monster_id && state.monsters[i].catalog_index >= 0)
      return &state.monsters[i];
  return NULL;
}

void rpg_live_init(void) {
  int i;
  memset(&state, 0, sizeof(state));
  state.initialized = 1;
  state.next_monster_id = 1;
  for (i = 0; i < RPG_LIVE_MAX_MONSTERS; ++i)
    state.monsters[i].catalog_index = -1;
  rpg_rng_seed(&rng, 0xC1A1E57u);
}

void rpg_live_update(double delta_seconds) {
  int i;
  if (!state.initialized || delta_seconds <= 0.0) return;
  state.elapsed_seconds += delta_seconds;
  for (i = 0; i < RPG_LIVE_MAX_PLAYERS; ++i) {
    if (!state.players[i].active) continue;
    state.players[i].combat_cooldown -= delta_seconds;
    if (state.players[i].combat_cooldown < 0.0)
      state.players[i].combat_cooldown = 0.0;
  }
}

const rpg_live_state *rpg_live_get_state(void) { return &state; }

int rpg_live_register_player(int player_id, int level) {
  int i;
  rpg_live_player *player;
  if (!state.initialized || player_id < 0) return 0;
  player = find_player(player_id);
  if (player) return 1;
  for (i = 0; i < RPG_LIVE_MAX_PLAYERS; ++i) {
    if (state.players[i].active) continue;
    memset(&state.players[i], 0, sizeof(state.players[i]));
    state.players[i].active = 1;
    state.players[i].player_id = player_id;
    state.players[i].room_id = -1;
    state.players[i].zone_id = -1;
    rpg_character_init(&state.players[i].character, player_id);
    if (!rpg_character_set_level(&state.players[i].character, level)) {
      state.players[i].active = 0;
      return 0;
    }
    return 1;
  }
  return 0;
}

int rpg_live_unregister_player(int player_id) {
  rpg_live_player *player = find_player(player_id);
  if (!player) return 0;
  memset(player, 0, sizeof(*player));
  return 1;
}

rpg_character *rpg_live_get_character(int player_id) {
  rpg_live_player *player = find_player(player_id);
  return player ? &player->character : NULL;
}

rpg_live_player *rpg_live_get_player(int player_id) {
  return find_player(player_id);
}

int rpg_live_set_player_world_state(int player_id, int room_id, int zone_id,
                                    int inventory_count, int equipment_count,
                                    int inventory_weight) {
  rpg_live_player *player = find_player(player_id);
  if (!player || room_id < -1 || zone_id < -1 ||
      inventory_count < 0 || equipment_count < 0 || inventory_weight < 0)
    return 0;

  player->room_id = room_id;
  player->zone_id = zone_id;
  player->inventory_count = inventory_count;
  player->equipment_count = equipment_count;
  player->inventory_weight = inventory_weight;
  return 1;
}

int rpg_live_spawn_monster(int catalog_index, int zone_id, int difficulty) {
  int i;
  if (!state.initialized) return 0;
  for (i = 0; i < RPG_LIVE_MAX_MONSTERS; ++i) {
    if (state.monsters[i].catalog_index >= 0 && state.monsters[i].alive) continue;
    if (!rpg_monster_init(&state.monsters[i], state.next_monster_id++, catalog_index,
                          zone_id, difficulty))
      return 0;
    return state.monsters[i].id;
  }
  return 0;
}

int rpg_live_attack(int player_id, int monster_id, rpg_damage_type type) {
  rpg_live_player *player = find_player(player_id);
  rpg_monster *monster = find_monster(monster_id);
  rpg_attack_result result;
  if (!player || !monster || !monster->alive || player->combat_cooldown > 0.0) return 0;
  if (!rpg_apply_attack(&player->character, monster, type, &rng, &result)) return 0;
  player->combat_cooldown = 1.0 / (player->character.stats.haste > 0.01
                                      ? player->character.stats.haste
                                      : 1.0);
  if (!monster->alive) {
    player->kills++;
    rpg_character_gain_xp(&player->character, monster->experience_reward);
  }
  return 1;
}

int rpg_live_collect_loot(int player_id, int monster_id, rpg_loot_drop *drops, int max_drops) {
  rpg_live_player *player = find_player(player_id);
  rpg_monster *monster = find_monster(monster_id);
  int count;
  if (!player || !monster || monster->alive || !drops || max_drops <= 0) return 0;
  count = rpg_roll_monster_loot(monster->catalog_index, monster->world_difficulty,
                                monster->loot_scale, rpg_monster_is_boss(monster),
                                &rng, drops, max_drops);
  if (count > 0) player->loot_events++;
  return count;
}
