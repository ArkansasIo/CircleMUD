#include "rpg_system.h"
#include "rpg_world.h"
#include "rpg_world_difficulty.h"

static int current_difficulty = 1;
static double elapsed_seconds = 0.0;

void rpg_system_init(void) {
  current_difficulty = 1;
  elapsed_seconds = 0.0;
  rpg_world_init();
}

void rpg_system_update(double delta_seconds) {
  if (delta_seconds > 0.0) elapsed_seconds += delta_seconds;
}

void rpg_system_set_world_difficulty(int tier) {
  if (rpg_world_difficulty_valid(tier)) current_difficulty = tier;
}

int rpg_system_get_world_difficulty(void) { return current_difficulty; }

rpg_world_difficulty rpg_system_get_difficulty_info(void) {
  return *rpg_world_difficulty_get(current_difficulty);
}
