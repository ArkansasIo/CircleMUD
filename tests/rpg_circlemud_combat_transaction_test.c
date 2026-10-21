#include <assert.h>
#include <stdio.h>

#include "../src/rpg/rpg_circlemud_combat_bridge.h"
#include "../src/rpg/rpg_circlemud_combat_transaction.h"
#include "../src/rpg/rpg_live.h"

int main(void) {
  rpg_live_state const *state;
  int monster_id;

  rpg_live_init();
  rpg_circlemud_combat_bridge_init();

  assert(rpg_live_register_player(42, 5) == 1);
  monster_id = rpg_live_spawn_monster(0, 7, 1);
  assert(monster_id > 0);

  state = rpg_live_get_state();
  assert(state != NULL);

  assert(rpg_circlemud_queue_damage(42, monster_id, 0, 10.0) == 1);
  {
    rpg_circlemud_combat_event event;
    assert(rpg_circlemud_combat_event_get(0, &event) == 1);
    assert(rpg_circlemud_combat_transaction_validate(&event) == RPG_CIRCLEMUD_TX_ACCEPTED);
  }

  rpg_circlemud_combat_bridge_clear();
  assert(rpg_circlemud_queue_damage(999, monster_id, 0, 10.0) == 1);
  {
    rpg_circlemud_combat_event event;
    assert(rpg_circlemud_combat_event_get(0, &event) == 1);
    assert(rpg_circlemud_combat_transaction_validate(&event) == RPG_CIRCLEMUD_TX_REJECTED);
  }

  printf("rpg CircleMUD combat transaction tests passed\n");
  return 0;
}
