#include "rpg_circlemud_bridge.h"

#include "conf.h"
#include "sysdep.h"

#include "rpg_live.h"
#include "rpg_circlemud_item_bridge.h"

#include "db.h"
#include "structs.h"
#include "utils.h"

static int circlemud_player_id(const struct char_data *ch) {
  if (!ch)
    return -1;

  return (int)ch->char_specials.saved.idnum;
}

static int circlemud_player_present(int id) {
  struct char_data *ch;

  for (ch = character_list; ch; ch = ch->next) {
    if (!IS_NPC(ch) && circlemud_player_id(ch) == id)
      return 1;
  }

  return 0;
}

static void circlemud_inventory_snapshot(const struct char_data *ch,
                                         int *inventory_count,
                                         int *inventory_weight,
                                         int *equipment_count) {
  struct obj_data *obj;
  int count = 0;
  int weight = 0;
  int equipped = 0;
  int slot;

  if (!ch) {
    if (inventory_count) *inventory_count = 0;
    if (inventory_weight) *inventory_weight = 0;
    if (equipment_count) *equipment_count = 0;
    return;
  }

  for (obj = ch->carrying; obj; obj = obj->next_content) {
    ++count;
    if (GET_OBJ_WEIGHT(obj) > 0)
      weight += GET_OBJ_WEIGHT(obj);
  }

  for (slot = 0; slot < NUM_WEARS; ++slot)
    if (GET_EQ(ch, slot))
      ++equipped;

  if (inventory_count) *inventory_count = count;
  if (inventory_weight) *inventory_weight = weight;
  if (equipment_count) *equipment_count = equipped;
}

int rpg_circlemud_sync_character(struct char_data *ch) {
  int id;
  int inventory_count;
  int inventory_weight;
  int equipment_count;
  int room_id = -1;
  int zone_id = -1;
  rpg_character *rpg;

  if (!ch || IS_NPC(ch))
    return 0;

  id = circlemud_player_id(ch);
  if (id < 0)
    return 0;

  rpg = rpg_live_get_character(id);
  if (!rpg) {
    if (!rpg_live_register_player(id, ch->player.level))
      return 0;
    rpg = rpg_live_get_character(id);
  }

  if (!rpg)
    return 0;

  if (ch->player.level > 0)
    rpg_character_set_level(rpg, ch->player.level);

  rpg->xp = (double)ch->points.exp;
  rpg->class_id = ch->player.chclass;
  rpg->stats.str = ch->aff_abils.str;
  rpg->stats.dex = ch->aff_abils.dex;
  rpg->stats.con = ch->aff_abils.con;
  rpg->stats.intel = ch->aff_abils.intel;
  rpg->stats.wis = ch->aff_abils.wis;
  rpg->stats.cha = ch->aff_abils.cha;
  rpg->stats.attack_power = (double)ch->points.damroll;
  rpg->stats.armor = (double)ch->points.armor;
  rpg->stats.max_health = (double)ch->points.max_hit;
  rpg->stats.health = (double)ch->points.hit;
  rpg->stats.max_mana = (double)ch->points.max_mana;
  rpg->stats.mana = (double)ch->points.mana;
  rpg->stats.max_stamina = (double)ch->points.max_move;
  rpg->stats.stamina = (double)ch->points.move;

  if (IN_ROOM(ch) != NOWHERE && IN_ROOM(ch) >= 0 && IN_ROOM(ch) <= top_of_world) {
    room_id = (int)IN_ROOM(ch);
    zone_id = world[IN_ROOM(ch)].zone;
  }

  circlemud_inventory_snapshot(ch, &inventory_count, &inventory_weight,
                               &equipment_count);

  if (!rpg_live_set_player_world_state(id, room_id, zone_id,
                                       inventory_count, equipment_count,
                                       inventory_weight))
    return 0;

  if (!rpg_circlemud_sync_items(ch))
    return 0;

  return 1;
}

int rpg_circlemud_unsync_character(struct char_data *ch) {
  int id;

  if (!ch || IS_NPC(ch))
    return 0;

  id = circlemud_player_id(ch);
  if (id < 0)
    return 0;

  return rpg_live_unregister_player(id);
}

void rpg_circlemud_on_character_extracted(struct char_data *ch) {
  (void)rpg_circlemud_unsync_character(ch);
}

void rpg_circlemud_sync_players(void) {
  struct char_data *ch;
  const rpg_live_state *state;
  int i;

  for (ch = character_list; ch; ch = ch->next)
    rpg_circlemud_sync_character(ch);

  state = rpg_live_get_state();
  for (i = 0; state && i < RPG_LIVE_MAX_PLAYERS; ++i) {
    if (state->players[i].active &&
        !circlemud_player_present(state->players[i].player_id)) {
      rpg_live_unregister_player(state->players[i].player_id);
    }
  }
}
