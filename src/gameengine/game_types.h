#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ge_engine ge_engine;

typedef enum ge_event_type {
  GE_EVENT_NONE = 0,
  GE_EVENT_TICK,
  GE_EVENT_ENTER_ZONE,
  GE_EVENT_LEAVE_ZONE,
  GE_EVENT_DAMAGE,
  GE_EVENT_HEAL,
  GE_EVENT_QUEST,
  GE_EVENT_ABILITY,
  GE_EVENT_SCRIPT
} ge_event_type;

typedef struct ge_event {
  ge_event_type type;
  long actor_id;
  long target_id;
  long value;
  const char *name;
  const char *payload;
} ge_event;

typedef struct ge_entity {
  long id;
  char name[64];
  int active;
  float x;
  float y;
  float z;
  int hp;
  int max_hp;
} ge_entity;

#ifdef __cplusplus
}
#endif

#endif
