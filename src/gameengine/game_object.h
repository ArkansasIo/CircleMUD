#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "game_types.h"

#define GE_OBJECT_NAME_MAX 64

typedef struct ge_object ge_object;
typedef void (*ge_object_update_fn)(ge_object *object, float delta_seconds);
typedef void (*ge_object_event_fn)(ge_object *object, const ge_event *event);

struct ge_object {
  long id;
  char name[GE_OBJECT_NAME_MAX];
  int enabled;
  void *userdata;
  ge_object_update_fn update;
  ge_object_event_fn on_event;
};

void ge_object_init(ge_object *object, long id, const char *name);
void ge_object_update(ge_object *object, float delta_seconds);
void ge_object_event(ge_object *object, const ge_event *event);
void ge_object_set_update(ge_object *object, ge_object_update_fn update);
void ge_object_set_event_handler(ge_object *object, ge_object_event_fn handler);

#endif
