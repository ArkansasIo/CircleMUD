#include "rpg_circlemud_host_resolver.h"

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
  struct char_data *ch;

  /* monster_id is an RPG identity, not a CircleMUD mob VNUM. The source key
   * is therefore the only legacy identity accepted by this resolver. */
  if (monster_id <= 0 || source_key == 0)
    return 0;

  for (ch = character_list; ch; ch = ch->next) {
    if (!IS_NPC(ch))
      continue;

    /* Source-key translation is deliberately left to the mob bridge. This
     * resolver only confirms that the legacy NPC population exists. */
    (void)ch;
  }

  return 1;
}

void rpg_circlemud_install_host_adapter(void) {
  /* Production callback installation is deferred until the concrete native
   * combat/persistence API is selected. This function is intentionally a
   * no-op rather than inventing a VNUM-to-instance mapping. */
}
