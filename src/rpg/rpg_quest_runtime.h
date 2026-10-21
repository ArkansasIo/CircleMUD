#ifndef CIRCLEMUD_RPG_QUEST_RUNTIME_H
#define CIRCLEMUD_RPG_QUEST_RUNTIME_H

#include "rpg_quest.h"

#define RPG_QUEST_RUNTIME_MAX_ACTIVE 64

typedef struct {
  rpg_quest quests[RPG_QUEST_RUNTIME_MAX_ACTIVE];
  int count;
} rpg_quest_runtime;

void rpg_quest_runtime_init(rpg_quest_runtime *runtime);
rpg_quest *rpg_quest_runtime_add(rpg_quest_runtime *runtime, int id, int level);
rpg_quest *rpg_quest_runtime_find(rpg_quest_runtime *runtime, int id);
int rpg_quest_runtime_activate(rpg_quest_runtime *runtime, int id);
int rpg_quest_runtime_progress(rpg_quest_runtime *runtime, int id, int objective_index, int amount);
int rpg_quest_runtime_complete(rpg_quest_runtime *runtime, int id);
int rpg_quest_runtime_remove(rpg_quest_runtime *runtime, int id);
int rpg_quest_runtime_count(const rpg_quest_runtime *runtime);

#endif
