#include <assert.h>
#include <stdio.h>
#include "../src/rpg/rpg_runtime_tick.h"
#include "../src/rpg/rpg_system.h"

int main(void) {
  rpg_runtime_tick_init();
  assert(rpg_system_get_world_difficulty() == 1);
  assert(rpg_system_get_elapsed_seconds() == 0.0);

  rpg_runtime_tick(0.25);
  rpg_runtime_tick(0.75);
  assert(rpg_system_get_elapsed_seconds() == 1.0);

  rpg_runtime_tick(0.0);
  rpg_runtime_tick(-1.0);
  assert(rpg_system_get_elapsed_seconds() == 1.0);

  printf("rpg runtime tick tests passed\n");
  return 0;
}
