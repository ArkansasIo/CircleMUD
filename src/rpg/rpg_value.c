#include "rpg_value.h"
#include <math.h>
#include <string.h>

static double clampd(double v, double lo, double hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

double rpg_context_multiplier(rpg_value_context context, const char *stat_name) {
  if (!stat_name) return 1.0;
  if (context == RPG_CONTEXT_PVE) return 1.0;
  if (strcmp(stat_name, "damage") == 0) return 0.70;
  if (strcmp(stat_name, "crit_chance") == 0) return 0.50;
  if (strcmp(stat_name, "crit_damage") == 0) return 0.60;
  if (strcmp(stat_name, "healing") == 0) return 0.65;
  if (strcmp(stat_name, "cooldown_reduction") == 0) return 0.50;
  if (strcmp(stat_name, "armor") == 0) return 1.10;
  if (strcmp(stat_name, "cc_duration") == 0) return 0.40;
  return 1.0;
}

double rpg_stat_weight(const char *stat_name, rpg_value_context context) {
  double base = 1.0;
  if (!stat_name) return base;
  if (strcmp(stat_name, "str") == 0 || strcmp(stat_name, "dex") == 0 ||
      strcmp(stat_name, "int") == 0 || strcmp(stat_name, "intelligence") == 0) base = 1.00;
  else if (strcmp(stat_name, "vit") == 0 || strcmp(stat_name, "con") == 0) base = 0.85;
  else if (strcmp(stat_name, "crit_chance") == 0) base = 2.50;
  else if (strcmp(stat_name, "crit_damage") == 0) base = 1.75;
  else if (strcmp(stat_name, "attack_speed") == 0) base = 2.00;
  else if (strcmp(stat_name, "cooldown_reduction") == 0) base = 2.25;
  else if (strcmp(stat_name, "armor") == 0) base = 0.75;
  else if (strcmp(stat_name, "resistance") == 0) base = 0.90;
  else if (strcmp(stat_name, "resilience") == 0) base = 3.00;
  return base * rpg_context_multiplier(context, stat_name);
}

double rpg_soft_cap(double raw, double soft_cap) {
  if (raw <= 0.0 || soft_cap <= 0.0) return raw > 0.0 ? raw : 0.0;
  return (raw / (raw + soft_cap)) * soft_cap;
}

double rpg_diminishing_value(double raw, double cap, double k) {
  if (raw <= 0.0 || cap <= 0.0 || k <= 0.0) return 0.0;
  return cap * (1.0 - exp(-raw / k));
}

double rpg_stat_value(double raw, double weight, double soft_cap, double cap, double k) {
  double value = raw;
  if (soft_cap > 0.0) value = rpg_soft_cap(raw, soft_cap);
  if (cap > 0.0 && k > 0.0) value = rpg_diminishing_value(value, cap, k);
  return value * weight;
}

double rpg_weapon_power_pve(double base_damage, double weapon_speed_factor,
                            double main_stat, double stat_coef,
                            double elemental_damage, double elemental_coef,
                            double pve_multiplier) {
  return (base_damage * weapon_speed_factor + main_stat * stat_coef +
          elemental_damage * elemental_coef) * pve_multiplier;
}

double rpg_weapon_power_pvp(double normalized_damage, double main_stat,
                            double stat_coef, double pvp_stat_scale,
                            double pvp_dampening) {
  return (normalized_damage + main_stat * stat_coef * pvp_stat_scale) * pvp_dampening;
}

double rpg_armor_mitigation(double armor, double attacker_level, double k) {
  double denominator = armor + k * attacker_level;
  if (armor <= 0.0 || denominator <= 0.0) return 0.0;
  return clampd(armor / denominator, 0.0, 0.95);
}

double rpg_defensive_power(double hp, double armor, double resistance,
                           double hp_weight, double armor_weight,
                           double resist_weight, double survival_multiplier) {
  return (hp * hp_weight + armor * armor_weight + resistance * resist_weight) * survival_multiplier;
}

double rpg_proc_value(const rpg_value_proc *proc) {
  if (!proc) return 0.0;
  return proc->magnitude * clampd(proc->uptime, 0.0, 1.0) * proc->impact_weight;
}

double rpg_item_power(const rpg_value_stats *stats,
                      rpg_value_context context,
                      const rpg_value_proc *procs, int proc_count,
                      double set_bonus_value) {
  double total = 0.0;
  int i;
  if (stats) {
    total += stats->str * rpg_stat_weight("str", context);
    total += stats->dex * rpg_stat_weight("dex", context);
    total += stats->intelligence * rpg_stat_weight("int", context);
    total += stats->vit * rpg_stat_weight("vit", context);
    total += stats->con * rpg_stat_weight("con", context);
    total += stats->crit_chance * rpg_stat_weight("crit_chance", context);
    total += stats->crit_damage * rpg_stat_weight("crit_damage", context);
    total += stats->attack_speed * rpg_stat_weight("attack_speed", context);
    total += stats->cooldown_reduction * rpg_stat_weight("cooldown_reduction", context);
    total += stats->armor * rpg_stat_weight("armor", context);
    total += stats->resistance * rpg_stat_weight("resistance", context);
    total += stats->resilience * rpg_stat_weight("resilience", context);
    total += stats->damage * rpg_stat_weight("damage", context);
    total += stats->healing * rpg_stat_weight("healing", context);
    total += stats->cc_duration * rpg_stat_weight("cc_duration", context);
  }
  for (i = 0; i < proc_count; ++i) total += rpg_proc_value(&procs[i]);
  return total + set_bonus_value;
}

double rpg_buff_value(double stat_increase, double duration,
                      double target_count, double context_multiplier) {
  return stat_increase * duration * target_count * context_multiplier;
}

double rpg_cc_value(double base_duration, double control_weight,
                    double dr_multiplier, double target_importance) {
  return base_duration * control_weight * dr_multiplier * target_importance;
}

double rpg_party_multiplier(int players) {
  if (players < 1) players = 1;
  return 1.0 + (players - 1) * 0.65;
}

double rpg_boss_hp(double base_hp, double level_diff, int players,
                   double difficulty_multiplier) {
  if (level_diff < 0.0) level_diff = 0.0;
  return base_hp * (1.0 + level_diff * 0.18) *
         rpg_party_multiplier(players) * difficulty_multiplier;
}

double rpg_boss_damage(double base_damage, double level_diff,
                       double difficulty_multiplier) {
  if (level_diff < 0.0) level_diff = 0.0;
  return base_damage * (1.0 + level_diff * 0.12) * difficulty_multiplier;
}

double rpg_encounter_power_budget(double player_effective_power_sum,
                                  double difficulty_factor) {
  return player_effective_power_sum * difficulty_factor;
}

int rpg_value_clamp_caps(double *crit_chance, double *cdr,
                         double *cc_duration, double *damage_reduction,
                         double *healing_bonus, rpg_value_context context) {
  if (!crit_chance || !cdr || !cc_duration || !damage_reduction || !healing_bonus) return 0;
  *crit_chance = clampd(*crit_chance, 0.0, context == RPG_CONTEXT_PVP ? 0.40 : 0.70);
  *cdr = clampd(*cdr, 0.0, context == RPG_CONTEXT_PVP ? 0.35 : 0.60);
  if (context == RPG_CONTEXT_PVP) *cc_duration = clampd(*cc_duration, 0.0, 0.50);
  *damage_reduction = clampd(*damage_reduction, 0.0, 0.70);
  if (context == RPG_CONTEXT_PVP) *healing_bonus = clampd(*healing_bonus, 0.0, 0.50);
  return 1;
}
