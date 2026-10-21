#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "game_types.h"

#define GE_MAX_ZONES 256

typedef struct ge_subzone {
  int id;
  char name[64];
  int biome_id;
} ge_subzone;

typedef struct ge_zone {
  int id;
  char name[64];
  int biome_id;
  int subbiome_id;
  int subzone_count;
  ge_subzone subzones[16];
} ge_zone;

typedef struct ge_world {
  int zone_count;
  ge_zone zones[GE_MAX_ZONES];
} ge_world;

void ge_world_init(ge_world *world);
int ge_world_add_zone(ge_world *world, int id, const char *name, int biome_id, int subbiome_id);
int ge_zone_add_subzone(ge_zone *zone, int id, const char *name, int biome_id);
ge_zone *ge_world_find_zone(ge_world *world, int id);

#endif
