#include <assert.h>

#include "rpg_quest_runtime.h"

int main(void) {
  rpg_quest_runtime runtime;
  rpg_quest *quest;

  rpg_quest_runtime_init(&runtime);
  assert(rpg_quest_runtime_count(&runtime) == 0);

  quest = rpg_quest_runtime_add(&runtime, 1001, 20);
  assert(quest != 0);
  assert(quest->state == RPG_QUEST_AVAILABLE);
  assert(rpg_quest_add_objective(quest, RPG_QUEST_KILL, 42, 3) == 1);
  assert(rpg_quest_add_objective(quest, RPG_QUEST_COLLECT, 77, 2) == 1);

  assert(rpg_quest_runtime_activate(&runtime, 1001) == 1);
  assert(rpg_quest_runtime_progress(&runtime, 1001, 0, 2) == 1);
  assert(quest->objectives[0].progress == 2);
  assert(quest->state == RPG_QUEST_ACTIVE);
  assert(rpg_quest_runtime_progress(&runtime, 1001, 0, 1) == 1);
  assert(rpg_quest_runtime_progress(&runtime, 1001, 1, 2) == 1);
  assert(quest->state == RPG_QUEST_COMPLETE);
  assert(rpg_quest_runtime_complete(&runtime, 1001) == 1);

  assert(rpg_quest_runtime_remove(&runtime, 1001) == 1);
  assert(rpg_quest_runtime_count(&runtime) == 0);
  assert(rpg_quest_runtime_find(&runtime, 1001) == 0);
  return 0;
}
