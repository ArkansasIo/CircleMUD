#ifndef CIRCLEMUD_RPG_CIRCLEMUD_COMBAT_CONSUMER_H
#define CIRCLEMUD_RPG_CIRCLEMUD_COMBAT_CONSUMER_H

#include "rpg_circlemud_combat_bridge.h"

/* Consume one validated event without mutating legacy CircleMUD state. */
int rpg_circlemud_combat_consume_event(const rpg_circlemud_combat_event *event);

/* Consume the current queue and remove only successfully consumed events. */
int rpg_circlemud_combat_consume_queue(void);

#endif
