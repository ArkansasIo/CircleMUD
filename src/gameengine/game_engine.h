#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "game_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GE_MAX_SYSTEMS 64
#define GE_MAX_EVENTS 128

typedef void (*ge_update_fn)(ge_engine *engine, float delta_seconds);
typedef void (*ge_event_fn)(ge_engine *engine, const ge_event *event);

typedef struct ge_system {
  char name[64];
  ge_update_fn update;
  int enabled;
} ge_system;

typedef struct ge_engine {
  int running;
  unsigned long tick;
  double time_seconds;
  ge_system systems[GE_MAX_SYSTEMS];
  int system_count;
  ge_event_fn event_handlers[GE_MAX_EVENTS];
  int event_count;
} ge_engine;

void ge_init(ge_engine *engine);
void ge_shutdown(ge_engine *engine);
int ge_register_system(ge_engine *engine, const char *name, ge_update_fn update);
void ge_update(ge_engine *engine, float delta_seconds);
int ge_register_event_handler(ge_engine *engine, ge_event_fn handler);
void ge_emit_event(ge_engine *engine, const ge_event *event);
void ge_run(ge_engine *engine, float fixed_delta, unsigned long max_ticks);

#ifdef __cplusplus
}
#endif

#endif
