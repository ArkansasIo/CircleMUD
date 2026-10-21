#include "rpg_quest_runtime.h"

#include <string.h>

void rpg_quest_runtime_init(rpg_quest_runtime *runtime) {
  if (!runtime) return;
  memset(runtime, 0, sizeof(*runtime));
}

rpg_quest *rpg_quest_runtime_find(rpg_quest_runtime *runtime, int id) {
  int i;
  if (!runtime) return 0;
  for (i = 0; i < runtime->count; ++i) {
    if (runtime->quests[i].id == id) return &runtime->quests[i];
  }
  return 0;
}

rpg_quest *rpg_quest_runtime_add(rpg_quest_runtime *runtime, int id, int level) {
  rpg_quest *quest;
  if (!runtime || runtime->count >= RPG_QUEST_RUNTIME_MAX_ACTIVE) return 0;
  if (level < 1 || level > 720 || id < 0) return 0;
  if (rpg_quest_runtime_find(runtime, id)) return 0;
  quest = &runtime->quests[runtime->count++];
  rpg_quest_init(quest, id, level);
  return quest;
}

int rpg_quest_runtime_activate(rpg_quest_runtime *runtime, int id) {
  rpg_quest *quest = rpg_quest_runtime_find(runtime, id);
  if (!quest || quest->state != RPG_QUEST_AVAILABLE) return 0;
  quest->state = RPG_QUEST_ACTIVE;
  return 1;
}

int rpg_quest_runtime_progress(rpg_quest_runtime *runtime, int id, int objective_index, int amount) {
  rpg_quest *quest = rpg_quest_runtime_find(runtime, id);
  if (!quest || quest->state != RPG_QUEST_ACTIVE || amount <= 0) return 0;
  if (!rpg_quest_progress(quest, objective_index, amount)) return 0;
  if (rpg_quest_is_complete(quest)) quest->state = RPG_QUEST_COMPLETE;
  return 1;
}

int rpg_quest_runtime_complete(rpg_quest_runtime *runtime, int id) {
  rpg_quest *quest = rpg_quest_runtime_find(runtime, id);
  if (!quest || !rpg_quest_is_complete(quest)) return 0;
  quest->state = RPG_QUEST_COMPLETE;
  return 1;
}

int rpg_quest_runtime_remove(rpg_quest_runtime *runtime, int id) {
  int i;
  if (!runtime) return 0;
  for (i = 0; i < runtime->count; ++i) {
    if (runtime->quests[i].id == id) {
      runtime->quests[i] = runtime->quests[runtime->count - 1];
      --runtime->count;
      return 1;
    }
  }
  return 0;
}

int rpg_quest_runtime_count(const rpg_quest_runtime *runtime) {
  return runtime ? runtime->count : 0;
}
