#include <assert.h>
#include <stdio.h>
#include "../src/rpg/rpg_circlemud_combat_bridge.h"

int main(void) {
  rpg_circlemud_combat_event event;

  rpg_circlemud_combat_bridge_init();
  assert(rpg_circlemud_combat_event_count() == 0);
  assert(rpg_circlemud_queue_damage(1001, 7, 9001, 25.0) == 1);
  assert(rpg_circlemud_queue_death(1001, 7, 9001) == 1);
  assert(rpg_circlemud_queue_xp(1001, 7, 50.0) == 1);
  assert(rpg_circlemud_queue_loot(1001, 7, 0) == 1);
  assert(rpg_circlemud_combat_event_count() == 4);

  assert(rpg_circlemud_combat_event_get(0, &event) == 1);
  assert(event.type == RPG_CIRCLEMUD_EVENT_DAMAGE);
  assert(event.player_id == 1001);
  assert(event.monster_id == 7);
  assert(event.source_key == 9001);
  assert(event.amount == 25.0);
  assert(rpg_circlemud_combat_event_authorized(&event) == 1);

  assert(rpg_circlemud_combat_event_get(3, &event) == 1);
  assert(event.type == RPG_CIRCLEMUD_EVENT_LOOT);
  assert(event.catalog_index == 0);
  assert(rpg_circlemud_combat_event_authorized(&event) == 1);

  assert(rpg_circlemud_queue_damage(-1, 7, 9001, 1.0) == 0);
  assert(rpg_circlemud_queue_damage(1001, 0, 9001, 1.0) == 0);
  assert(rpg_circlemud_queue_damage(1001, 7, 9001, -1.0) == 0);

  rpg_circlemud_combat_bridge_clear();
  assert(rpg_circlemud_combat_event_count() == 0);
  printf("rpg CircleMUD combat bridge tests passed\n");
  return 0;
}
