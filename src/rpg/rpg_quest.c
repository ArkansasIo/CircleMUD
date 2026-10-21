#include "rpg_quest.h"

void rpg_quest_init(rpg_quest *quest, int id, int level) {
  int i;
  if (!quest) return;
  quest->id = id;
  quest->level = level;
  quest->state = RPG_QUEST_AVAILABLE;
  quest->reward_xp = level > 0 ? level * level * 50 : 0;
  quest->reward_item_catalog_index = -1;
  quest->objective_count = 0;
  for (i = 0; i < 8; ++i) quest->objectives[i].completed = 0;
}

int rpg_quest_add_objective(rpg_quest *quest, rpg_quest_objective_type type, int target_id, int required) {
  rpg_quest_objective *o;
  if (!quest || required <= 0 || quest->objective_count >= 8) return 0;
  o = &quest->objectives[quest->objective_count++];
  o->id = quest->objective_count;
  o->type = type;
  o->target_id = target_id;
  o->required = required;
  o->progress = 0;
  o->completed = 0;
  return 1;
}

int rpg_quest_progress(rpg_quest *quest, int objective_index, int amount) {
  rpg_quest_objective *o;
  if (!quest || objective_index < 0 || objective_index >= quest->objective_count || amount <= 0) return 0;
  o = &quest->objectives[objective_index];
  if (o->completed) return 0;
  o->progress += amount;
  if (o->progress >= o->required) { o->progress = o->required; o->completed = 1; }
  if (rpg_quest_is_complete(quest)) quest->state = RPG_QUEST_COMPLETE;
  else quest->state = RPG_QUEST_ACTIVE;
  return 1;
}

int rpg_quest_is_complete(const rpg_quest *quest) {
  int i;
  if (!quest || quest->objective_count == 0) return 0;
  for (i = 0; i < quest->objective_count; ++i) if (!quest->objectives[i].completed) return 0;
  return 1;
}
