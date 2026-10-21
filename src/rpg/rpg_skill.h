#ifndef CIRCLEMUD_RPG_SKILL_H
#define CIRCLEMUD_RPG_SKILL_H

typedef struct {
  int id;
  int class_id;
  int specialization_id;
  int rank;
  int max_rank;
  double proficiency;
  double mastery;
} rpg_skill;

void rpg_skill_init(rpg_skill *skill, int id, int class_id, int specialization_id, int max_rank);
int rpg_skill_train(rpg_skill *skill, double amount);
double rpg_skill_effectiveness(const rpg_skill *skill);

#endif
