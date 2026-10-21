#ifndef CIRCLEMUD_RPG_RNG_H
#define CIRCLEMUD_RPG_RNG_H

#include "rpg_types.h"

void rpg_rng_seed(rpg_rng *rng, uint32_t seed);
uint32_t rpg_rng_next(rpg_rng *rng);
double rpg_rng_unit(rpg_rng *rng);
int rpg_rng_range(rpg_rng *rng, int min, int max);

#endif
