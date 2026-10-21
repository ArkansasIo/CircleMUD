#include "rpg_circlemud_combat_transaction.h"

#include "rpg_live.h"
#include "rpg_catalog.h"

static int monster_matches(const rpg_circlemud_combat_event *event) {
  const rpg_live_state *state;
  int i;

  if (!event)
    return 0;

  state = rpg_live_get_state();
  if (!state)
    return 0;

  for (i = 0; i < RPG_LIVE_MAX_MONSTERS; ++i) {
    const rpg_monster *monster = &state->monsters[i];
    if (!monster->active)
      continue;
    if (monster->id != event->monster_id)
      continue;
    if (event->source_key != 0 && monster->source_key != event->source_key)
      return 0;
    return 1;
  }

  return 0;
}

rpg_circlemud_transaction_result
rpg_circlemud_combat_transaction_validate(const rpg_circlemud_combat_event *event) {
  if (!rpg_circlemud_combat_event_authorized(event))
    return RPG_CIRCLEMUD_TX_REJECTED;

  if (!rpg_live_get_player(event->player_id))
    return RPG_CIRCLEMUD_TX_REJECTED;

  if (!monster_matches(event))
    return RPG_CIRCLEMUD_TX_REJECTED;

  return RPG_CIRCLEMUD_TX_ACCEPTED;
}
