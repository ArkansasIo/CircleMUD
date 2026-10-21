#include "game_engine.h"

#include <string.h>

void ge_init(ge_engine *engine)
{
  if (engine == 0)
    return;
  memset(engine, 0, sizeof(*engine));
  engine->running = 1;
}

void ge_shutdown(ge_engine *engine)
{
  if (engine == 0)
    return;
  engine->running = 0;
}

int ge_register_system(ge_engine *engine, const char *name, ge_update_fn update)
{
  ge_system *system;
  if (engine == 0 || update == 0 || engine->system_count >= GE_MAX_SYSTEMS)
    return -1;
  system = &engine->systems[engine->system_count++];
  memset(system, 0, sizeof(*system));
  if (name != 0)
    strncpy(system->name, name, sizeof(system->name) - 1);
  system->update = update;
  system->enabled = 1;
  return engine->system_count - 1;
}

int ge_register_event_handler(ge_engine *engine, ge_event_fn handler)
{
  if (engine == 0 || handler == 0 || engine->event_count >= GE_MAX_EVENTS)
    return -1;
  engine->event_handlers[engine->event_count++] = handler;
  return engine->event_count - 1;
}

void ge_emit_event(ge_engine *engine, const ge_event *event)
{
  int i;
  if (engine == 0 || event == 0)
    return;
  for (i = 0; i < engine->event_count; ++i)
    if (engine->event_handlers[i] != 0)
      engine->event_handlers[i](engine, event);
}

void ge_update(ge_engine *engine, float delta_seconds)
{
  int i;
  ge_event tick_event;
  if (engine == 0 || !engine->running)
    return;
  engine->time_seconds += delta_seconds;
  ++engine->tick;
  for (i = 0; i < engine->system_count; ++i)
    if (engine->systems[i].enabled && engine->systems[i].update != 0)
      engine->systems[i].update(engine, delta_seconds);
  memset(&tick_event, 0, sizeof(tick_event));
  tick_event.type = GE_EVENT_TICK;
  ge_emit_event(engine, &tick_event);
}

void ge_run(ge_engine *engine, float fixed_delta, unsigned long max_ticks)
{
  unsigned long count = 0;
  if (fixed_delta <= 0.0f)
    fixed_delta = 0.1f;
  while (engine != 0 && engine->running && (max_ticks == 0 || count < max_ticks)) {
    ge_update(engine, fixed_delta);
    ++count;
  }
}
