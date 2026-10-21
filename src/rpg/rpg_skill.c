#include "rpg_skill.h"

void rpg_skill_init(rpg_skill *skill, int id, int class_id, int specialization_id, int max_rank) {
  if (!skill) return;
  skill->id = id;
  skill->class_id = class_id;
  skill->specialization_id = specialization_id;
  skill->rank = 0;
  skill->max_rank = max_rank > 0 ? max_rank : 100;
  skill->proficiency = 0.0;
  skill->mastery = 0.0;
}

int rpg_skill_train(rpg_skill *skill, double amount) {
  if (!skill || amount <= 0.0 || skill->rank >= skill->max_rank) return 0;
  skill->proficiency += amount;
  while (skill->proficiency >= 100.0 && skill->rank < skill->max_rank) {
    skill->proficiency -= 100.0;
    ++skill->rank;
  }
  if (skill->rank >= skill->max_rank) skill->proficiency = 0.0;
  skill->mastery = (double)skill->rank / (double)skill->max_rank;
  return 1;
}

double rpg_skill_effectiveness(const rpg_skill *skill) {
  if (!skill || skill->max_rank <= 0) return 0.0;
  return ((double)skill->rank + skill->proficiency / 100.0) / (double)skill->max_rank;
}
