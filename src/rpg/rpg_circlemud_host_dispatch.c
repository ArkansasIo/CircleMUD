#include "rpg_circlemud_host_dispatch.h"

#include "rpg_circlemud_combat_bridge.h"
#include "rpg_circlemud_combat_transaction.h"
#include "rpg_circlemud_host_adapter.h"

int rpg_circlemud_host_dispatch_queue(void) {
  int i;
  int consumed = 0;
  int count = rpg_circlemud_combat_event_count();

  if (count <= 0)
    return 0;

  for (i = 0; i < count; ++i) {
    rpg_circlemud_combat_event event;

    if (!rpg_circlemud_combat_event_get(i, &event))
      continue;

    if (rpg_circlemud_combat_transaction_validate(&event) !=
        RPG_CIRCLEMUD_TX_ACCEPTED)
      continue;

    if (rpg_circlemud_host_adapter_apply(&event))
      ++consumed;
  }

  /* Preserve the queue if even one event could not cross the host boundary. */
  if (consumed == count)
    rpg_circlemud_combat_bridge_clear();

  return consumed;
}
