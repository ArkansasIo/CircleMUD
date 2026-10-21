#ifndef GAME_SCRIPT_H
#define GAME_SCRIPT_H

#include "game_engine.h"

#define GE_SCRIPT_PATH_MAX 256

typedef struct ge_script_runtime {
  void *lua_state;
  int enabled;
  char script_root[GE_SCRIPT_PATH_MAX];
} ge_script_runtime;

int ge_script_init(ge_script_runtime *runtime, const char *script_root);
void ge_script_shutdown(ge_script_runtime *runtime);
int ge_script_load(ge_script_runtime *runtime, const char *file_name);
int ge_script_call_update(ge_script_runtime *runtime, float delta_seconds);
int ge_script_emit_event(ge_script_runtime *runtime, const ge_event *event);

#endif
