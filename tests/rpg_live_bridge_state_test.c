#include <assert.h>
#include <stdio.h>

#include "../src/rpg/rpg_live.h"

int main(void) {
  rpg_live_player *player;
  const rpg_live_state *state;

  rpg_live_init();

  assert(rpg_live_register_player(1001, 12) == 1);
  assert(rpg_live_register_player(1001, 12) == 1);

  player = rpg_live_get_player(1001);
  assert(player != NULL);
  assert(player->active == 1);
  assert(player->room_id == -1);
  assert(player->zone_id == -1);

  assert(rpg_live_set_player_world_state(1001, 42, 7, 5, 4, 37) == 1);
  assert(rpg_live_set_player_world_state(1001, 42, 7, -1, 4, 37) == 0);

  player = rpg_live_get_player(1001);
  assert(player->room_id == 42);
  assert(player->zone_id == 7);
  assert(player->inventory_count == 5);
  assert(player->equipment_count == 4);
  assert(player->inventory_weight == 37);

  assert(rpg_live_unregister_player(1001) == 1);
  assert(rpg_live_get_player(1001) == NULL);
  assert(rpg_live_unregister_player(1001) == 0);

  state = rpg_live_get_state();
  assert(state->initialized == 1);

  printf("rpg live bridge state tests passed\n");
  return 0;
}
