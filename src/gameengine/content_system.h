#ifndef CIRCLEMUD_CONTENT_SYSTEM_H
#define CIRCLEMUD_CONTENT_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#define GE_CONTENT_MAX_INSTANCES 128
#define GE_TOWER_MAX_FLOORS 100
#define GE_CONTENT_MAX_OBJECTIVES 16
#define GE_CONTENT_MAX_BOSSES 16

typedef enum ge_content_type {
  GE_CONTENT_DUNGEON = 1,
  GE_CONTENT_TRAIL = 2,
  GE_CONTENT_RAID = 3,
  GE_CONTENT_TOWER = 4
} ge_content_type;

typedef enum ge_content_state {
  GE_CONTENT_AVAILABLE = 0,
  GE_CONTENT_FORMING,
  GE_CONTENT_ENTERING,
  GE_CONTENT_ACTIVE,
  GE_CONTENT_CHECKPOINT,
  GE_CONTENT_BOSS,
  GE_CONTENT_COMPLETE,
  GE_CONTENT_RESET
} ge_content_state;

typedef struct ge_content_instance {
  int id;
  int definition_id;
  ge_content_type type;
  ge_content_state state;
  int difficulty;
  int current_stage;
  int current_floor;
  int party_size;
  int deaths;
  int elapsed_seconds;
} ge_content_instance;

typedef struct ge_tower_floor {
  int floor;
  int tier;
  int encounter_id;
  int modifier_id;
  int checkpoint;
  int boss;
} ge_tower_floor;

typedef struct ge_tower_definition {
  int id;
  char name[64];
  int max_floor;
  ge_tower_floor floors[GE_TOWER_MAX_FLOORS];
} ge_tower_definition;

void ge_content_init(void);
int ge_content_create_instance(ge_content_type type, int definition_id, int difficulty, int party_size);
ge_content_instance *ge_content_find_instance(int instance_id);
int ge_content_enter_instance(int instance_id);
int ge_content_start_encounter(int instance_id);
int ge_content_advance_stage(int instance_id);
int ge_content_advance_floor(int instance_id);
int ge_content_activate_checkpoint(int instance_id);
int ge_content_defeat_boss(int instance_id);
int ge_content_complete_instance(int instance_id);
int ge_content_reset_instance(int instance_id);

void ge_tower_init(ge_tower_definition *tower, int id, const char *name);
const ge_tower_floor *ge_tower_get_floor(const ge_tower_definition *tower, int floor);
int ge_tower_set_floor(ge_tower_definition *tower, int floor, int tier, int encounter_id, int modifier_id, int checkpoint, int boss);

#ifdef __cplusplus
}
#endif

#endif
