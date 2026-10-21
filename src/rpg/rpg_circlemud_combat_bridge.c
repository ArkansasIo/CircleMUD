#include "rpg_circlemud_combat_bridge.h"
#include "rpg_catalog.h"
#include <string.h>

static rpg_circlemud_combat_event events[RPG_CIRCLEMUD_EVENT_QUEUE];
static int event_count;
static unsigned long next_sequence;

void rpg_circlemud_combat_bridge_init(void) {
  memset(events, 0, sizeof(events));
  event_count = 0;
  next_sequence = 1;
}

static int queue_event(rpg_circlemud_event_type type, int player_id,
                       int monster_id, int source_key, double amount,
                       int catalog_index) {
  rpg_circlemud_combat_event *event;
  if (player_id < 0 || monster_id <= 0 || event_count >= RPG_CIRCLEMUD_EVENT_QUEUE)
    return 0;
  if (amount < 0.0)
    return 0;
  if (catalog_index >= 0 && !rpg_catalog_valid_index(catalog_index))
    return 0;
  event = &events[event_count++];
  memset(event, 0, sizeof(*event));
  event->type = type;
  event->player_id = player_id;
  event->monster_id = monster_id;
  event->source_key = source_key;
  event->amount = amount;
  event->catalog_index = catalog_index;
  event->sequence = next_sequence++;
  if (next_sequence == 0)
    next_sequence = 1;
  return 1;
}

int rpg_circlemud_queue_damage(int player_id, int monster_id, int source_key, double amount) {
  return queue_event(RPG_CIRCLEMUD_EVENT_DAMAGE, player_id, monster_id, source_key, amount, -1);
}

int rpg_circlemud_queue_death(int player_id, int monster_id, int source_key) {
  return queue_event(RPG_CIRCLEMUD_EVENT_DEATH, player_id, monster_id, source_key, 0.0, -1);
}

int rpg_circlemud_queue_xp(int player_id, int monster_id, double amount) {
  return queue_event(RPG_CIRCLEMUD_EVENT_XP, player_id, monster_id, 0, amount, -1);
}

int rpg_circlemud_queue_loot(int player_id, int monster_id, int catalog_index) {
  return queue_event(RPG_CIRCLEMUD_EVENT_LOOT, player_id, monster_id, 0, 0.0, catalog_index);
}

int rpg_circlemud_combat_event_count(void) {
  return event_count;
}

int rpg_circlemud_combat_event_get(int index, rpg_circlemud_combat_event *event) {
  if (!event || index < 0 || index >= event_count)
    return 0;
  *event = events[index];
  return 1;
}

void rpg_circlemud_combat_bridge_clear(void) {
  event_count = 0;
}

int rpg_circlemud_combat_event_authorized(const rpg_circlemud_combat_event *event) {
  if (!event || event->sequence == 0 || event->player_id < 0 || event->monster_id <= 0)
    return 0;
  switch (event->type) {
    case RPG_CIRCLEMUD_EVENT_DAMAGE:
    case RPG_CIRCLEMUD_EVENT_DEATH:
    case RPG_CIRCLEMUD_EVENT_XP:
      return event->amount >= 0.0;
    case RPG_CIRCLEMUD_EVENT_LOOT:
      return event->catalog_index >= 0 && rpg_catalog_valid_index(event->catalog_index);
    default:
      return 0;
  }
}
