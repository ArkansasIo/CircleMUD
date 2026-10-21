#ifndef CIRCLEMUD_RPG_QUEST_H
#define CIRCLEMUD_RPG_QUEST_H

typedef enum { RPG_QUEST_KILL = 0, RPG_QUEST_COLLECT, RPG_QUEST_EXPLORE, RPG_QUEST_TALK, RPG_QUEST_CRAFT, RPG_QUEST_BOSS } rpg_quest_objective_type;
typedef enum { RPG_QUEST_AVAILABLE = 0, RPG_QUEST_ACTIVE, RPG_QUEST_COMPLETE, RPG_QUEST_FAILED } rpg_quest_state;

typedef struct {
  int id;
  rpg_quest_objective_type type;
  int target_id;
  int required;
  int progress;
  int completed;
} rpg_quest_objective;

typedef struct {
  int id;
  int level;
  rpg_quest_state state;
  int reward_xp;
  int reward_item_catalog_index;
  rpg_quest_objective objectives[8];
  int objective_count;
} rpg_quest;

void rpg_quest_init(rpg_quest *quest, int id, int level);
int rpg_quest_add_objective(rpg_quest *quest, rpg_quest_objective_type type, int target_id, int required);
int rpg_quest_progress(rpg_quest *quest, int objective_index, int amount);
int rpg_quest_is_complete(const rpg_quest *quest);

#endif
