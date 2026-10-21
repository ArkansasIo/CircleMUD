#include "rpg_runtime_tick.h"
#include "rpg_system.h"

void rpg_runtime_tick_init(void) {
  rpg_system_init();
}

void rpg_runtime_tick(double delta_seconds) {
  if (delta_seconds <= 0.0) return;
  rpg_system_update(delta_seconds);
}
