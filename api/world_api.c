#include "world_api.h"

#include <stddef.h>
#include <string.h>

static const cm_map_location g_locations[] = {
  {"circlehold", "Circlehold", "capital", "Greenfall", {420, 350}},
  {"black-forge", "Black Forge", "dungeon", "Ashen Reach", {1110, 290}},
  {"ravenwatch", "Ravenwatch", "village", "Crown Plains", {760, 930}},
  {"drowned-archive", "Drowned Archive", "dungeon", "Drowned Archive", {1540, 820}}
};

int cm_world_map_find_location(const char *id, cm_map_location *out)
{
  size_t i;
  if (id == NULL || out == NULL)
    return 0;

  for (i = 0; i < sizeof(g_locations) / sizeof(g_locations[0]); ++i) {
    if (strcmp(id, g_locations[i].id) == 0) {
      *out = g_locations[i];
      return 1;
    }
  }
  return 0;
}

int cm_world_map_player_position(cm_map_point *out)
{
  if (out == NULL)
    return 0;
  out->x = 420;
  out->y = 350;
  return 1;
}
