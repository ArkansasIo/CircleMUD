#include "rpg_circlemud_bridge.h"

#include "rpg_live.h"

#include "db.h"
#include "structs.h"
#include "utils.h"

static int circlemud_player_id(const struct char_data *ch) {
  if (!ch)
    return -1;

  return (int)ch->char_specials.saved.idnum;
}

int rpg_circlemud_sync_character(struct char_data *ch) {
  int id;
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

  /* CircleMUD remains authoritative for the legacy character sheet. */
  if (ch->player.level > 0)
    rpg_character_set_level(rpg, ch->player.level);

  rpg->xp = (double)ch->points.exp;
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

void rpg_circlemud_sync_players(void) {
  struct char_data *ch;

  for (ch = character_list; ch; ch = ch->next)
    rpg_circlemud_sync_character(ch);
}
