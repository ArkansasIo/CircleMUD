#ifndef CIRCLEMUD_RPG_VALUE_H
#define CIRCLEMUD_RPG_VALUE_H

/* Formal item, effect, encounter, and combat valuation model.
 * The constants mirror the canonical PvE/PvP value specification in
 * docs/RPG_VALUE_FORMULAS.md. All functions are deterministic and
 * side-effect free so they can be used by server, tooling, and tests.
 */

typedef enum {
  RPG_CONTEXT_PVE = 0,
  RPG_CONTEXT_PVP = 1
} rpg_value_context;

typedef struct {
  double str;
  double dex;
  double con;
  double intelligence;
  double wis;
  double vit;
  double spi;
  double luck;
  double wil;
  double cha;
  double crit_chance;
  double crit_damage;
  double attack_speed;
  double cooldown_reduction;
  double armor;
  double resistance;
  double resilience;
  double health;
  double damage;
  double healing;
  double cc_duration;
} rpg_value_stats;

typedef struct {
  double magnitude;
  double uptime;
  double impact_weight;
} rpg_value_proc;

typedef struct {
  double stat_power;
  double proc_power;
  double set_bonus_power;
  double total_power;
} rpg_item_value;

double rpg_stat_weight(const char *stat_name, rpg_value_context context);
double rpg_soft_cap(double raw, double soft_cap);
double rpg_diminishing_value(double raw, double cap, double k);
double rpg_stat_value(double raw, double weight, double soft_cap, double cap, double k);

double rpg_weapon_power_pve(double base_damage, double weapon_speed_factor,
                            double main_stat, double stat_coef,
                            double elemental_damage, double elemental_coef,
                            double pve_multiplier);
double rpg_weapon_power_pvp(double normalized_damage, double main_stat,
                            double stat_coef, double pvp_stat_scale,
                            double pvp_dampening);

double rpg_armor_mitigation(double armor, double attacker_level, double k);
double rpg_defensive_power(double hp, double armor, double resistance,
                           double hp_weight, double armor_weight,
                           double resist_weight, double survival_multiplier);

double rpg_proc_value(const rpg_value_proc *proc);
double rpg_item_power(const rpg_value_stats *stats,
                      rpg_value_context context,
                      const rpg_value_proc *procs, int proc_count,
                      double set_bonus_value);

double rpg_buff_value(double stat_increase, double duration,
                      double target_count, double context_multiplier);
double rpg_cc_value(double base_duration, double control_weight,
                    double dr_multiplier, double target_importance);

double rpg_party_multiplier(int players);
double rpg_boss_hp(double base_hp, double level_diff, int players,
                   double difficulty_multiplier);
double rpg_boss_damage(double base_damage, double level_diff,
                       double difficulty_multiplier);
double rpg_encounter_power_budget(double player_effective_power_sum,
                                  double difficulty_factor);

double rpg_context_multiplier(rpg_value_context context, const char *stat_name);
int rpg_value_clamp_caps(double *crit_chance, double *cdr,
                         double *cc_duration, double *damage_reduction,
                         double *healing_bonus, rpg_value_context context);

#endif
