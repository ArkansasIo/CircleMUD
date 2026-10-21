#include "rpg_combat.h"
#include "rpg_value.h"

static double type_multiplier(rpg_damage_type type, int subtype) {
  unsigned int seed = (unsigned int)(subtype * 1103515245u + 12345u);
  switch (type) {
    case RPG_DAMAGE_FIRE: return 1.05 + (seed % 6) * 0.01;
    case RPG_DAMAGE_FROST: return 1.00 + (seed % 5) * 0.01;
    case RPG_DAMAGE_SHADOW: return 1.08;
    case RPG_DAMAGE_ARCANE: return 1.10;
    case RPG_DAMAGE_NATURE: return 1.03;
    default: return 1.00;
  }
}

rpg_attack_result rpg_calculate_attack(const rpg_character *attacker, const rpg_monster *target, rpg_damage_type type, rpg_rng *rng) {
  rpg_attack_result r;
  double mitigation;
  double variance;
  double crit_chance;
  double crit_damage;
  double effective_power;
  r.raw_damage = 0.0;
  r.mitigated_damage = 0.0;
  r.critical_multiplier = 2.0;
  r.critical = 0;
  if (!attacker || !target || !target->alive || !rng) return r;

  crit_chance = rpg_diminishing_value(attacker->stats.crit * 100.0, 70.0, 35.0) / 100.0;
  crit_damage = rpg_stat_weight("crit_damage", RPG_CONTEXT_PVE);
  effective_power = attacker->stats.attack_power + attacker->equipment.pve_rating * 0.05;
  variance = 0.90 + rpg_rng_unit(rng) * 0.20;
  r.critical = rpg_rng_unit(rng) < crit_chance;
  r.critical_multiplier = 1.0 + crit_damage;
  r.raw_damage = effective_power * type_multiplier(type, target->catalog_index) * variance;
  if (r.critical) r.raw_damage *= r.critical_multiplier;
  mitigation = 1.0 - rpg_armor_mitigation(target->armor, (double)attacker->level, 100.0);
  r.mitigated_damage = r.raw_damage * mitigation;
  if (r.mitigated_damage < 0.0) r.mitigated_damage = 0.0;
  return r;
}

int rpg_apply_attack(const rpg_character *attacker, rpg_monster *target, rpg_damage_type type, rpg_rng *rng, rpg_attack_result *result) {
  rpg_attack_result local;
  if (!attacker || !target || !rng) return 0;
  local = rpg_calculate_attack(attacker, target, type, rng);
  if (result) *result = local;
  return rpg_monster_damage(target, local.mitigated_damage);
}
