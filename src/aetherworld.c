#include "aetherworld.h"

static int initialized = 0;

void aetherworld_init(void) {
  initialized = 1;
}

static long level_base_xp(int level) {
  long n;
  if (level < 1) level = 1;
  n = (long)level;
  return n * n * n * 100L + n * 500L;
}

long aetherworld_required_xp(int level) {
  if (level <= 1) return 0L;
  if (level > AETHERWORLD_MAX_LEVEL) level = AETHERWORLD_MAX_LEVEL;
  return level_base_xp(level);
}

int aetherworld_level_from_xp(long xp) {
  int level;
  if (xp <= 0) return 1;
  for (level = 1; level < AETHERWORLD_MAX_LEVEL; ++level) {
    if (xp < aetherworld_required_xp(level + 1)) return level;
  }
  return AETHERWORLD_MAX_LEVEL;
}

int aetherworld_validate(void) {
  int i;
  int previous_id = 0;
  if (!initialized) aetherworld_init();
  for (i = 1; i <= AETHERWORLD_QUEST_COUNT; ++i) {
    const struct aetherworld_quest *quest = aetherworld_get_quest(i);
    if (!quest || quest->id != i || quest->id <= previous_id) return 0;
    if (!aetherworld_get_act(quest->act_id)) return 0;
    if (quest->level < 1 || quest->level > AETHERWORLD_MAX_LEVEL) return 0;
    if (quest->required_quest >= quest->id) return 0;
    previous_id = quest->id;
  }
  return 1;
}

int aetherworld_quest_available(int quest_id, int level, int completed_quest_id) {
  const struct aetherworld_quest *quest = aetherworld_get_quest(quest_id);
  if (!quest || level < quest->level) return 0;
  if (quest->required_quest > 0 && completed_quest_id < quest->required_quest) return 0;
  return 1;
}
