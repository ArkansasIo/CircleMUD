#include "rpg_runtime_tick.h"
#include "rpg_system.h"
#include "rpg_live.h"

static double elapsed_seconds = 0.0;

void rpg_runtime_tick_init(void) {
  elapsed_seconds = 0.0;
  rpg_system_init();
  rpg_live_init();
}

void rpg_runtime_tick(double delta_seconds) {
  if (delta_seconds <= 0.0) return;
  elapsed_seconds += delta_seconds;
  rpg_system_update(delta_seconds);
  rpg_live_update(delta_seconds);
}

double rpg_runtime_tick_elapsed_seconds(void) {
  return elapsed_seconds;
}
