#include "rpg_circlemud_host_adapter.h"

#include <string.h>

static rpg_circlemud_host_callbacks callbacks;
static int initialized;

void rpg_circlemud_host_adapter_init(void) {
  memset(&callbacks, 0, sizeof(callbacks));
  initialized = 1;
}

int rpg_circlemud_host_adapter_register(const rpg_circlemud_host_callbacks *new_callbacks) {
  if (!new_callbacks)
    return 0;

  if (!new_callbacks->apply_damage || !new_callbacks->apply_death ||
      !new_callbacks->grant_xp || !new_callbacks->materialize_loot)
    return 0;

  callbacks = *new_callbacks;
  initialized = 1;
  return 1;
}

int rpg_circlemud_host_adapter_available(void) {
  return initialized && callbacks.apply_damage && callbacks.apply_death &&
         callbacks.grant_xp && callbacks.materialize_loot;
}

int rpg_circlemud_host_adapter_apply(const rpg_circlemud_combat_event *event) {
  if (!event || !rpg_circlemud_host_adapter_available())
    return 0;

  switch (event->type) {
    case RPG_CIRCLEMUD_EVENT_DAMAGE:
      return callbacks.apply_damage(event->player_id, event->monster_id,
                                    event->source_key, (int)event->amount);
    case RPG_CIRCLEMUD_EVENT_DEATH:
      return callbacks.apply_death(event->player_id, event->monster_id,
                                   event->source_key);
    case RPG_CIRCLEMUD_EVENT_XP:
      return callbacks.grant_xp(event->player_id, event->monster_id,
                                (int)event->amount);
    case RPG_CIRCLEMUD_EVENT_LOOT:
      return callbacks.materialize_loot(event->player_id, event->monster_id,
                                        event->catalog_index);
    default:
      return 0;
  }
}
