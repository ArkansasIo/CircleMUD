#include <assert.h>
#include <stdio.h>

#include "../src/rpg/rpg_circlemud_combat_bridge.h"
#include "../src/rpg/rpg_circlemud_combat_consumer.h"
#include "../src/rpg/rpg_live.h"

int main(void) {
  rpg_live_init();
  rpg_circlemud_combat_bridge_init();

  assert(rpg_live_register_player(77, 10) == 1);
  assert(rpg_circlemud_queue_damage(77, 10, 10010, 25.0) == 1);
  assert(rpg_circlemud_queue_xp(77, 10, 100.0) == 1);
  assert(rpg_circlemud_combat_event_count() == 2);
  assert(rpg_circlemud_combat_consume_queue() == 2);
  assert(rpg_circlemud_combat_event_count() == 0);

  /* Unknown players are rejected and the queue remains intact. */
  assert(rpg_circlemud_queue_damage(9999, 10, 10010, 5.0) == 1);
  assert(rpg_circlemud_combat_consume_queue() == 0);
  assert(rpg_circlemud_combat_event_count() == 1);
  rpg_circlemud_combat_bridge_clear();

  /* Malformed events cannot cross the authorization boundary. */
  {
    rpg_circlemud_combat_event bad;
    bad.type = RPG_CIRCLEMUD_EVENT_DAMAGE;
    bad.player_id = -1;
    bad.monster_id = 10;
    bad.source_key = 10010;
    bad.amount = 1.0;
    bad.catalog_index = -1;
    bad.sequence = 1;
    assert(rpg_circlemud_combat_consume_event(&bad) == 0);
  }

  printf("rpg CircleMUD combat consumer tests passed\n");
  return 0;
}
