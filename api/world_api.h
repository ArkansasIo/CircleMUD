#ifndef CIRCLEMUD_WORLD_API_H
#define CIRCLEMUD_WORLD_API_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cm_map_point {
  int x;
  int y;
} cm_map_point;

typedef struct cm_map_location {
  const char *id;
  const char *name;
  const char *kind;
  const char *region;
  cm_map_point position;
} cm_map_location;

int cm_world_map_find_location(const char *id, cm_map_location *out);
int cm_world_map_player_position(cm_map_point *out);

#ifdef __cplusplus
}
#endif

#endif
