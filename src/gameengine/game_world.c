#include "game_world.h"

#include <string.h>

void ge_world_init(ge_world *world)
{
  if (world != 0)
    memset(world, 0, sizeof(*world));
}

int ge_world_add_zone(ge_world *world, int id, const char *name, int biome_id, int subbiome_id)
{
  ge_zone *zone;
  if (world == 0 || world->zone_count >= GE_MAX_ZONES)
    return -1;
  zone = &world->zones[world->zone_count++];
  memset(zone, 0, sizeof(*zone));
  zone->id = id;
  zone->biome_id = biome_id;
  zone->subbiome_id = subbiome_id;
  if (name != 0)
    strncpy(zone->name, name, sizeof(zone->name) - 1);
  return world->zone_count - 1;
}

int ge_zone_add_subzone(ge_zone *zone, int id, const char *name, int biome_id)
{
  ge_subzone *subzone;
  if (zone == 0 || zone->subzone_count >= 16)
    return -1;
  subzone = &zone->subzones[zone->subzone_count++];
  memset(subzone, 0, sizeof(*subzone));
  subzone->id = id;
  subzone->biome_id = biome_id;
  if (name != 0)
    strncpy(subzone->name, name, sizeof(subzone->name) - 1);
  return zone->subzone_count - 1;
}

ge_zone *ge_world_find_zone(ge_world *world, int id)
{
  int i;
  if (world == 0)
    return 0;
  for (i = 0; i < world->zone_count; ++i)
    if (world->zones[i].id == id)
      return &world->zones[i];
  return 0;
}
