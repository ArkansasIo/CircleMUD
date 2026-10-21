#include "game_object.h"

#include <string.h>

void ge_object_init(ge_object *object, long id, const char *name)
{
  if (object == 0)
    return;
  memset(object, 0, sizeof(*object));
  object->id = id;
  object->enabled = 1;
  if (name != 0)
    strncpy(object->name, name, GE_OBJECT_NAME_MAX - 1);
}

void ge_object_update(ge_object *object, float delta_seconds)
{
  if (object != 0 && object->enabled && object->update != 0)
    object->update(object, delta_seconds);
}

void ge_object_event(ge_object *object, const ge_event *event)
{
  if (object != 0 && object->enabled && object->on_event != 0)
    object->on_event(object, event);
}

void ge_object_set_update(ge_object *object, ge_object_update_fn update)
{
  if (object != 0)
    object->update = update;
}

void ge_object_set_event_handler(ge_object *object, ge_object_event_fn handler)
{
  if (object != 0)
    object->on_event = handler;
}
