#include "rpg_circlemud_host_native.h"

#include "conf.h"
#include "sysdep.h"

#include "rpg_circlemud_host_adapter.h"
#include "rpg_circlemud_item_map.h"
#include "rpg_circlemud_mob_bridge.h"
#include "rpg_live.h"

#include "db.h"
#include "handler.h"
#include "structs.h"
#include "utils.h"

static struct char_data *find_player(int player_id) {
  struct char_data *ch;
  if (player_id < 0) return NULL;
  for (ch = character_list; ch; ch = ch->next)
    if (!IS_NPC(ch) && (int)GET_IDNUM(ch) == player_id) return ch;
  return NULL;
}

static int event_source_key(int monster_id, int source_key) {
  const rpg_live_state *state;
  int i;
  if (source_key > 0) return source_key;
  state = rpg_live_get_state();
  if (!state || monster_id <= 0) return 0;
  for (i = 0; i < RPG_LIVE_MAX_MONSTERS; ++i) {
    const rpg_monster *monster = &state->monsters[i];
    if (monster->catalog_index < 0 || monster->id != monster_id) continue;
    return monster->source_key;
  }
  return 0;
}

static struct char_data *find_event_monster(int monster_id, int source_key) {
  int resolved_key = event_source_key(monster_id, source_key);
  if (resolved_key <= 0) return NULL;
  return rpg_circlemud_find_mob_by_source_key(resolved_key);
}

static int native_apply_damage(int player_id, int monster_id, int source_key, int amount) {
  struct char_data *attacker = find_player(player_id);
  struct char_data *victim = find_event_monster(monster_id, source_key);
  int result;
  if (!attacker || !victim || !IS_NPC(victim) || amount <= 0) return 0;
  result = damage(attacker, victim, amount, TYPE_HIT);
  return result >= 0 || result == -1;
}

static int native_apply_death(int player_id, int monster_id, int source_key) {
  struct char_data *attacker = find_player(player_id);
  struct char_data *victim = find_event_monster(monster_id, source_key);
  int lethal;
  if (!attacker || !victim || !IS_NPC(victim)) return 0;
  if (GET_POS(victim) <= POS_DEAD) return 1;
  lethal = GET_HIT(victim) + 11;
  if (lethal < 1) lethal = 1;
  return damage(attacker, victim, lethal, TYPE_HIT) == -1;
}

static int native_grant_xp(int player_id, int monster_id, int amount) {
  struct char_data *player = find_player(player_id);
  (void)monster_id;
  if (!player || amount <= 0) return 0;
  gain_exp(player, amount);
  return 1;
}

static int native_materialize_loot(int player_id, int monster_id, int catalog_index) {
  struct char_data *player = find_player(player_id);
  struct obj_data *obj;
  int vnum;
  (void)monster_id;
  if (!player) return 0;
  vnum = rpg_circlemud_item_vnum(catalog_index);
  if (vnum < 0) return 0;
  obj = read_object((obj_vnum)vnum, VIRTUAL);
  if (!obj) return 0;
  obj_to_char(obj, player);
  return 1;
}

int rpg_circlemud_install_native_host_callbacks(void) {
  rpg_circlemud_host_callbacks callbacks;
  callbacks.apply_damage = native_apply_damage;
  callbacks.apply_death = native_apply_death;
  callbacks.grant_xp = native_grant_xp;
  callbacks.materialize_loot = native_materialize_loot;
  return rpg_circlemud_host_adapter_register(&callbacks);
}
