#include "rpg_circlemud_host_dispatch.h"

#include "rpg_circlemud_combat_bridge.h"
#include "rpg_circlemud_combat_transaction.h"
#include "rpg_circlemud_host_adapter.h"

int rpg_circlemud_host_dispatch_queue(void) {
  int i;
  int count = rpg_circlemud_combat_event_count();
  int processed = 0;

  if (count <= 0 || !rpg_circlemud_host_adapter_available())
    return 0;

  /* Events are FIFO and sequence-numbered. A successful prefix is committed;
   * the first rejected/unapplied event remains queued for the next heartbeat. */
  for (i = 0; i < count; ++i) {
    rpg_circlemud_combat_event event;

    if (!rpg_circlemud_combat_event_get(i, &event))
      break;

    if (rpg_circlemud_combat_transaction_validate(&event) !=
        RPG_CIRCLEMUD_TX_ACCEPTED)
      break;

    if (!rpg_circlemud_host_adapter_apply(&event))
      break;

    ++processed;
  }

  if (processed == count)
    rpg_circlemud_combat_bridge_clear();

  return processed;
}
