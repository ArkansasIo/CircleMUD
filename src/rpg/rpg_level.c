#include "rpg_level.h"
#include <math.h>

int rpg_level_valid(int level) { return level >= 1 && level <= RPG_MAX_LEVEL; }

double rpg_level_xp_required(int level) {
  if (!rpg_level_valid(level)) return 0.0;
  return floor(500.0 * pow((double)level, 2.35));
}

double rpg_level_power_scale(int level) {
  if (!rpg_level_valid(level)) return 1.0;
  return pow((double)level, 0.72);
}

int rpg_level_band(int level) {
  if (!rpg_level_valid(level)) return 0;
  return ((level - 1) / 10) + 1;
}

int rpg_level_milestone(int level) {
  if (!rpg_level_valid(level)) return 0;
  if (level <= 100) return level % 10 == 0;
  return level % 20 == 0 || level == 720;
}
