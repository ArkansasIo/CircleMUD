#ifndef AETHERWORLD_H
#define AETHERWORLD_H

#define AETHERWORLD_MAX_LEVEL 720
#define AETHERWORLD_ACT_COUNT 12
#define AETHERWORLD_QUEST_COUNT 50

struct aetherworld_act {
  int id;
  const char *title;
  const char *theme;
  int level_min;
  int level_max;
};

struct aetherworld_quest {
  int id;
  int act_id;
  const char *title;
  const char *objective;
  int level;
  int xp;
  int gold;
  int required_quest;
};

void aetherworld_init(void);
const struct aetherworld_act *aetherworld_get_act(int id);
const struct aetherworld_quest *aetherworld_get_quest(int id);
int aetherworld_quest_count(void);
int aetherworld_act_count(void);
int aetherworld_validate(void);
long aetherworld_required_xp(int level);
int aetherworld_level_from_xp(long xp);
int aetherworld_quest_available(int quest_id, int level, int completed_quest_id);

#endif
