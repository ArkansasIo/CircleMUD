#include <assert.h>
#include <stdio.h>
#include "aetherworld.h"

int main(void) {
  const struct aetherworld_act *act;
  const struct aetherworld_quest *quest;

  aetherworld_init();
  assert(aetherworld_validate());
  assert(aetherworld_act_count() == 12);
  assert(aetherworld_quest_count() == 50);

  act = aetherworld_get_act(12);
  assert(act != 0);
  assert(act->level_max == 720);

  quest = aetherworld_get_quest(50);
  assert(quest != 0);
  assert(quest->act_id == 12);
  assert(quest->required_quest == 49);
  assert(aetherworld_quest_available(50, 720, 49));
  assert(!aetherworld_quest_available(50, 719, 49));

  assert(aetherworld_level_from_xp(0) == 1);
  assert(aetherworld_required_xp(2) > aetherworld_required_xp(1));
  assert(aetherworld_level_from_xp(aetherworld_required_xp(720)) == 720);

  puts("AetherWorld runtime tests passed");
  return 0;
}
