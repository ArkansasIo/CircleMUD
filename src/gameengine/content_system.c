#include "content_system.h"

#include <string.h>

static ge_content_instance instances[GE_CONTENT_MAX_INSTANCES];
static int instance_count;

static int valid_instance(const ge_content_instance *instance)
{
  return instance != 0 && instance->id > 0;
}

void ge_content_init(void)
{
  memset(instances, 0, sizeof(instances));
  instance_count = 0;
}

int ge_content_create_instance(ge_content_type type, int definition_id, int difficulty, int party_size)
{
  ge_content_instance *instance;
  if (instance_count >= GE_CONTENT_MAX_INSTANCES)
    return -1;
  instance = &instances[instance_count];
  memset(instance, 0, sizeof(*instance));
  instance->id = instance_count + 1;
  instance->definition_id = definition_id;
  instance->type = type;
  instance->state = GE_CONTENT_ENTERING;
  instance->difficulty = difficulty;
  instance->party_size = party_size;
  instance->current_stage = 1;
  instance->current_floor = type == GE_CONTENT_TOWER ? 1 : 0;
  ++instance_count;
  return instance->id;
}

ge_content_instance *ge_content_find_instance(int instance_id)
{
  int i;
  for (i = 0; i < instance_count; ++i)
    if (instances[i].id == instance_id)
      return &instances[i];
  return 0;
}

int ge_content_enter_instance(int instance_id)
{
  ge_content_instance *instance = ge_content_find_instance(instance_id);
  if (!valid_instance(instance)) return -1;
  instance->state = GE_CONTENT_ACTIVE;
  return 0;
}

int ge_content_start_encounter(int instance_id)
{
  ge_content_instance *instance = ge_content_find_instance(instance_id);
  if (!valid_instance(instance)) return -1;
  instance->state = GE_CONTENT_BOSS;
  return 0;
}

int ge_content_advance_stage(int instance_id)
{
  ge_content_instance *instance = ge_content_find_instance(instance_id);
  if (!valid_instance(instance) || instance->state == GE_CONTENT_COMPLETE) return -1;
  ++instance->current_stage;
  instance->state = GE_CONTENT_ACTIVE;
  return instance->current_stage;
}

int ge_content_advance_floor(int instance_id)
{
  ge_content_instance *instance = ge_content_find_instance(instance_id);
  if (!valid_instance(instance) || instance->type != GE_CONTENT_TOWER) return -1;
  if (instance->current_floor >= GE_TOWER_MAX_FLOORS) return 100;
  ++instance->current_floor;
  instance->state = GE_CONTENT_ACTIVE;
  return instance->current_floor;
}

int ge_content_activate_checkpoint(int instance_id)
{
  ge_content_instance *instance = ge_content_find_instance(instance_id);
  if (!valid_instance(instance)) return -1;
  instance->state = GE_CONTENT_CHECKPOINT;
  return instance->current_floor > 0 ? instance->current_floor : instance->current_stage;
}

int ge_content_defeat_boss(int instance_id)
{
  ge_content_instance *instance = ge_content_find_instance(instance_id);
  if (!valid_instance(instance)) return -1;
  instance->state = GE_CONTENT_ACTIVE;
  return 0;
}

int ge_content_complete_instance(int instance_id)
{
  ge_content_instance *instance = ge_content_find_instance(instance_id);
  if (!valid_instance(instance)) return -1;
  instance->state = GE_CONTENT_COMPLETE;
  return 0;
}

int ge_content_reset_instance(int instance_id)
{
  ge_content_instance *instance = ge_content_find_instance(instance_id);
  if (!valid_instance(instance)) return -1;
  instance->state = GE_CONTENT_RESET;
  instance->current_stage = 1;
  instance->current_floor = instance->type == GE_CONTENT_TOWER ? 1 : 0;
  instance->deaths = 0;
  instance->elapsed_seconds = 0;
  return 0;
}

void ge_tower_init(ge_tower_definition *tower, int id, const char *name)
{
  int i;
  if (tower == 0) return;
  memset(tower, 0, sizeof(*tower));
  tower->id = id;
  tower->max_floor = GE_TOWER_MAX_FLOORS;
  if (name != 0)
    strncpy(tower->name, name, sizeof(tower->name) - 1);
  for (i = 0; i < GE_TOWER_MAX_FLOORS; ++i) {
    tower->floors[i].floor = i + 1;
    tower->floors[i].tier = (i / 10) + 1;
    tower->floors[i].checkpoint = ((i + 1) % 10) == 0;
    tower->floors[i].boss = ((i + 1) % 10) == 0 || (i + 1) == 100;
  }
}

const ge_tower_floor *ge_tower_get_floor(const ge_tower_definition *tower, int floor)
{
  if (tower == 0 || floor < 1 || floor > tower->max_floor)
    return 0;
  return &tower->floors[floor - 1];
}

int ge_tower_set_floor(ge_tower_definition *tower, int floor, int tier, int encounter_id, int modifier_id, int checkpoint, int boss)
{
  ge_tower_floor *entry;
  if (tower == 0 || floor < 1 || floor > GE_TOWER_MAX_FLOORS)
    return -1;
  entry = &tower->floors[floor - 1];
  entry->floor = floor;
  entry->tier = tier;
  entry->encounter_id = encounter_id;
  entry->modifier_id = modifier_id;
  entry->checkpoint = checkpoint;
  entry->boss = boss;
  return 0;
}
