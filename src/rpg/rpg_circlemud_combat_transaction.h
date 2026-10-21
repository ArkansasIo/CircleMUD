#ifndef CIRCLEMUD_RPG_CIRCLEMUD_COMBAT_TRANSACTION_H
#define CIRCLEMUD_RPG_CIRCLEMUD_COMBAT_TRANSACTION_H

#include "rpg_circlemud_combat_bridge.h"

/* Result of applying an RPG event to the authoritative host boundary. */
typedef enum {
  RPG_CIRCLEMUD_TX_REJECTED = 0,
  RPG_CIRCLEMUD_TX_ACCEPTED = 1
} rpg_circlemud_transaction_result;

/*
 * Validate an event against the current RPG snapshot and classify it for the
 * future CircleMUD mutation adapter. No legacy structures are modified here.
 */
rpg_circlemud_transaction_result
rpg_circlemud_combat_transaction_validate(const rpg_circlemud_combat_event *event);

#endif
