#include <assert.h>
#include <stdio.h>

#include "../src/rpg/rpg_live.h"

int main(void) {
  const rpg_live_state *state;
  int first_id;
  int second_id;

  rpg_live_init();
  rpg_live_begin_monster_sync();

  first_id = rpg_live_sync_monster_source(101, 750, 2, 1, 5,
                                           120.0, 120.0, 15.0, 4.0);
  assert(first_id > 0);

  second_id = rpg_live_sync_monster_source(102, 751, 2, 1, 6,
                                            150.0, 150.0, 18.0, 5.0);
  assert(second_id > 0);
  assert(second_id != first_id);

  rpg_live_end_monster_sync();
  state = rpg_live_get_state();
  assert(state->monsters[0].source_key == 101);
  assert(state->monsters[1].source_key == 102);

  rpg_live_begin_monster_sync();
  assert(rpg_live_sync_monster_source(101, 752, 3, 1, 7,
                                      175.0, 175.0, 20.0, 6.0) == first_id);
  rpg_live_end_monster_sync();

  state = rpg_live_get_state();
  assert(state->monsters[0].id == first_id);
  assert(state->monsters[0].catalog_index == 752);
  assert(state->monsters[0].zone_id == 3);
  assert(state->monsters[0].health == 175.0);

  /* Source 102 was not seen in the second reconciliation pass and is removed. */
  assert(state->monsters[1].source_key == 0);
  assert(state->monsters[1].catalog_index == -1);

  printf("rpg live monster source tests passed\n");
  return 0;
}
