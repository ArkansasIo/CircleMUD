#include "rpg_rng.h"

void rpg_rng_seed(rpg_rng *rng, uint32_t seed) {
  rng->seed = seed;
  rng->state = seed ? seed : 0xA341316Cu;
}

uint32_t rpg_rng_next(rpg_rng *rng) {
  uint32_t x = rng->state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  rng->state = x;
  return x;
}

double rpg_rng_unit(rpg_rng *rng) {
  return (double)rpg_rng_next(rng) / 4294967296.0;
}

int rpg_rng_range(rpg_rng *rng, int min, int max) {
  if (max <= min) return min;
  return min + (int)(rpg_rng_unit(rng) * (double)(max - min + 1));
}
