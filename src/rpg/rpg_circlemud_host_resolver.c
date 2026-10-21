#include "rpg_circlemud_host_resolver.h"

#include "rpg_circlemud_mob_bridge.h"

#include "db.h"
#include "structs.h"
#include "utils.h"

int rpg_circlemud_host_player_exists(int player_id) {
  struct char_data *ch;

  if (player_id < 0)
    return 0;

  for (ch = character_list; ch; ch = ch->next) {
    if (!IS_NPC(ch) && (int)GET_IDNUM(ch) == player_id)
      return 1;
  }

  return 0;
}

int rpg_circlemud_host_monster_exists(int monster_id, int source_key) {
  (void)monster_id;
  return rpg_circlemud_find_mob_by_source_key(source_key) != NULL;
}

void rpg_circlemud_install_host_adapter(void) {
  /* Native callback installation is performed by the heartbeat hook after
   * the RPG runtime has initialized. */
}
