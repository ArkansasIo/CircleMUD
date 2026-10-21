#include <assert.h>
#include <stdio.h>
#include "../src/rpg/rpg_runtime_tick.h"
#include "../src/rpg/rpg_system.h"

static int pending_extract_calls = 0;

void extract_pending_chars(void) {
  pending_extract_calls++;
}

void circlemud_rpg_extract_pending_chars(void);

int main(void) {
  circlemud_rpg_extract_pending_chars();
  assert(pending_extract_calls == 1);
  assert(rpg_system_get_world_difficulty() == 1);
  assert(rpg_system_get_elapsed_seconds() > 0.099);
  assert(rpg_system_get_elapsed_seconds() < 0.101);

  circlemud_rpg_extract_pending_chars();
  assert(pending_extract_calls == 2);
  assert(rpg_system_get_elapsed_seconds() > 0.199);
  assert(rpg_system_get_elapsed_seconds() < 0.201);

  printf("rpg heartbeat hook tests passed\n");
  return 0;
}
