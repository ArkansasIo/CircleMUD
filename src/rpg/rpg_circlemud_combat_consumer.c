#include "rpg_circlemud_combat_consumer.h"

#include "rpg_live.h"

/*
 * This layer deliberately stops at the RPG runtime boundary for now.
 *
 * It validates and acknowledges events without writing into CircleMUD's
 * char_data/obj_data structures. A future host adapter can replace the
 * acknowledgement with an authorized transaction after it has resolved the
 * legacy player/NPC instance and persistence policy.
 */
int rpg_circlemud_combat_consume_event(const rpg_circlemud_combat_event *event) {
  if (!rpg_circlemud_combat_event_authorized(event))
    return 0;

  if (!rpg_live_get_player(event->player_id))
    return 0;

  return 1;
}

int rpg_circlemud_combat_consume_queue(void) {
  int i;
  int consumed = 0;
  int count = rpg_circlemud_combat_event_count();

  for (i = 0; i < count; ++i) {
    rpg_circlemud_combat_event event;
    if (!rpg_circlemud_combat_event_get(i, &event))
      continue;
    if (rpg_circlemud_combat_consume_event(&event))
      ++consumed;
  }

  if (consumed == count)
    rpg_circlemud_combat_bridge_clear();

  return consumed;
}
