#include <assert.h>
#include <stdio.h>

#include "../src/rpg/rpg_live.h"

int main(void) {
  const rpg_live_state *state;
  rpg_loot_drop drops[RPG_LIVE_MAX_DROPS];
  int monster_id;
  int attempts = 0;
  int loot_count;

  rpg_live_init();
  assert(rpg_live_register_player(42, 10) == 1);
  assert(rpg_live_get_character(42) != NULL);

  monster_id = rpg_live_spawn_monster(750, 1, 1);
  assert(monster_id > 0);

  while (attempts++ < 1000) {
    if (!rpg_live_attack(42, monster_id, RPG_DAMAGE_PHYSICAL)) break;
    rpg_live_update(1.0);
    state = rpg_live_get_state();
    if (state->monsters[0].alive == 0) break;
  }

  state = rpg_live_get_state();
  assert(state->initialized == 1);
  assert(state->elapsed_seconds > 0.0);
  assert(state->players[0].kills == 1);

  loot_count = rpg_live_collect_loot(42, monster_id, drops, RPG_LIVE_MAX_DROPS);
  assert(loot_count >= 0);
  assert(state->players[0].loot_events >= 1);

  assert(rpg_live_unregister_player(42) == 1);
  assert(rpg_live_get_character(42) == NULL);

  printf("rpg live gameplay tests passed\n");
  return 0;
}
