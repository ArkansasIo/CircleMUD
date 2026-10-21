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
  /* RPG monster IDs are not CircleMUD mob VNUMs. Until the mob bridge exposes
   * a concrete instance resolver, fail closed rather than claiming that an
   * arbitrary NPC matches the event. */
  (void)monster_id;
  (void)source_key;
  return 0;
}

void rpg_circlemud_install_host_adapter(void) {
  /* Native callback installation remains deferred until the authoritative
   * CircleMUD combat/persistence entry points are selected. */
}
