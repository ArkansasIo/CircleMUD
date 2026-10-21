#include "game_script.h"

#include <stdio.h>
#include <string.h>

/* Lua is optional at the build boundary. The engine can run without it. */
#ifdef GAMEENGINE_WITH_LUA
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#endif

int ge_script_init(ge_script_runtime *runtime, const char *script_root)
{
  if (runtime == 0)
    return -1;
  memset(runtime, 0, sizeof(*runtime));
  runtime->enabled = 1;
  if (script_root != 0)
    strncpy(runtime->script_root, script_root, sizeof(runtime->script_root) - 1);
#ifdef GAMEENGINE_WITH_LUA
  runtime->lua_state = luaL_newstate();
  if (runtime->lua_state == 0)
    return -1;
  luaL_openlibs((lua_State *)runtime->lua_state);
#endif
  return 0;
}

void ge_script_shutdown(ge_script_runtime *runtime)
{
  if (runtime == 0)
    return;
#ifdef GAMEENGINE_WITH_LUA
  if (runtime->lua_state != 0)
    lua_close((lua_State *)runtime->lua_state);
#endif
  runtime->lua_state = 0;
  runtime->enabled = 0;
}

int ge_script_load(ge_script_runtime *runtime, const char *file_name)
{
#ifdef GAMEENGINE_WITH_LUA
  char path[GE_SCRIPT_PATH_MAX * 2];
  lua_State *L;
  if (runtime == 0 || runtime->lua_state == 0 || file_name == 0)
    return -1;
  L = (lua_State *)runtime->lua_state;
  if (runtime->script_root[0] != '\0')
    snprintf(path, sizeof(path), "%s/%s", runtime->script_root, file_name);
  else
    snprintf(path, sizeof(path), "%s", file_name);
  if (luaL_dofile(L, path) != LUA_OK) {
    lua_pop(L, 1);
    return -1;
  }
  return 0;
#else
  (void)runtime;
  (void)file_name;
  return -1;
#endif
}

int ge_script_call_update(ge_script_runtime *runtime, float delta_seconds)
{
#ifdef GAMEENGINE_WITH_LUA
  lua_State *L;
  if (runtime == 0 || runtime->lua_state == 0)
    return -1;
  L = (lua_State *)runtime->lua_state;
  lua_getglobal(L, "Game_Update");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 1);
    return 0;
  }
  lua_pushnumber(L, (lua_Number)delta_seconds);
  if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
    lua_pop(L, 1);
    return -1;
  }
  return 0;
#else
  (void)runtime;
  (void)delta_seconds;
  return -1;
#endif
}

int ge_script_emit_event(ge_script_runtime *runtime, const ge_event *event)
{
#ifdef GAMEENGINE_WITH_LUA
  lua_State *L;
  if (runtime == 0 || runtime->lua_state == 0 || event == 0)
    return -1;
  L = (lua_State *)runtime->lua_state;
  lua_getglobal(L, "Game_Event");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 1);
    return 0;
  }
  lua_pushinteger(L, (lua_Integer)event->type);
  lua_pushinteger(L, (lua_Integer)event->actor_id);
  lua_pushinteger(L, (lua_Integer)event->target_id);
  lua_pushinteger(L, (lua_Integer)event->value);
  lua_pushstring(L, event->name != 0 ? event->name : "");
  if (lua_pcall(L, 5, 0, 0) != LUA_OK) {
    lua_pop(L, 1);
    return -1;
  }
  return 0;
#else
  (void)runtime;
  (void)event;
  return -1;
#endif
}
