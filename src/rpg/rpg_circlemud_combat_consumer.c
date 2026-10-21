#include "rpg_circlemud_combat_consumer.h"

#include "rpg_circlemud_combat_transaction.h"
#include "rpg_live.h"

/*
 * The consumer is intentionally a transaction gate. It does not mutate
 * CircleMUD structures. Only events that match the current RPG snapshot are
 * acknowledged as eligible for the eventual host-side mutation adapter.
 */
int rpg_circlemud_combat_consume_event(const rpg_circlemud_combat_event *event) {
  return rpg_circlemud_combat_transaction_validate(event) ==
         RPG_CIRCLEMUD_TX_ACCEPTED;
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
