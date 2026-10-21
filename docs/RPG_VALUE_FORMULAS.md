# RPG Value Formulas

This document is the canonical numerical valuation layer for PvE and PvP equipment, effects, encounters, and rewards.

The model separates **raw stat value**, **effective power**, **PvE/PvP context**, and **encounter scaling**. The runtime implementation is `src/rpg/rpg_value.h` and `src/rpg/rpg_value.c`.

## 1. Stat value

```text
StatValuePU = RawStat × StatWeight × ScalingCurve
```

Baseline weights:

| Stat | Weight |
|---|---:|
| STR / DEX / INT | 1.00 |
| VIT / CON | 0.85 |
| Crit Chance | 2.50 |
| Crit Damage | 1.75 |
| Attack Speed | 2.00 |
| CDR | 2.25 |
| Armor | 0.75 |
| Resistance | 0.90 |
| PvP Resilience | 3.00 |

The PvP context applies the specified conversion multipliers: damage 0.70, crit chance 0.50, crit damage 0.60, healing 0.65, CDR 0.50, armor 1.10, and CC duration 0.40.

## 2. Scaling curves

Soft cap:

```text
EffectiveStat = RawStat / (RawStat + SoftCap)
ScalingCurve = EffectiveStat × SoftCap
```

Diminishing-return curve:

```text
EffectiveValue = Cap × (1 - e^(-RawStat / K))
```

Use the soft-cap form for ordinary progression and the diminishing-return form where a bounded value is required.

## 3. Weapon power

PvE:

```text
WeaponPowerPvE =
  (BaseDamage × WeaponSpeedFactor
   + MainStat × StatCoef
   + ElementalDamage × ElementCoef)
  × PvEMultiplier
```

PvP normalized:

```text
WeaponPowerPvP =
  (NormalizedBaseDamage
   + MainStat × StatCoef × PvPStatScale)
  × PvPDampening
```

The baseline specification uses `PvPStatScale = 0.65` and a `PvPDampening` tuning range of `0.75–0.85`.

## 4. Armor and defensive power

```text
Mitigation = Armor / (Armor + K × AttackerLevel)
```

```text
TDP =
  (HP × HPWeight
   + Armor × ArmorWeight
   + Resistance × ResistWeight)
  × SurvivalMultiplier
```

The runtime clamps armor mitigation to a maximum of 95% as an implementation safety guard.

## 5. Item power

```text
ItemPower = Σ(StatValuePU) + Σ(ProcValue) + SetBonusValue
```

For a proc:

```text
ProcValue = EffectMagnitude × Uptime × ImpactWeight
```

Uptime is bounded to `[0,1]` by the runtime valuation function.

## 6. Buff and crowd-control value

```text
BuffValue =
  (StatIncrease × Duration × TargetCount)
  × ContextMultiplier
```

```text
CCValue =
  (BaseDuration × ControlWeight)
  × DRMultiplier
  × TargetImportance
```

## 7. Boss and encounter scaling

Party multiplier:

```text
PartyMult = 1 + (Players - 1) × 0.65
```

Boss HP:

```text
BossHP = BaseHP
       × (1 + LevelDiff × 0.18)
       × PartyMult
       × DifficultyMultiplier
```

Boss damage:

```text
BossDamage = BaseDamage
           × (1 + LevelDiff × 0.12)
           × DifficultyMultiplier
```

The intentional rule is that HP scales faster than damage to reduce one-shot behavior as encounter size increases.

Encounter power budget:

```text
EncounterPB = Σ(PlayerEffectivePower) × DifficultyFactor
```

## 8. Hard caps

| Stat | PvE | PvP |
|---|---:|---:|
| Crit Chance | 70% | 40% |
| CDR | 60% | 35% |
| CC Duration | — | 50% |
| Damage Reduction | 70% | 70% |
| Healing Bonus | — | 50% |

The runtime exposes `rpg_value_clamp_caps()` so server-side calculations can enforce these limits before combat resolution.

## 9. Example

For a PvE weapon with:

```text
STR +50        = 50 PU
Crit +5%       = 12.5 PU
Fire +120      = 156 PU
Proc           = 90 PU
```

```text
Total Item Power = 308.5 PU
```

The same item can be evaluated under the PvP context without creating a separate item identity; the PvP conversion table changes its effective combat value.

## 10. Runtime API

The implementation provides:

- `rpg_stat_value()`
- `rpg_weapon_power_pve()`
- `rpg_weapon_power_pvp()`
- `rpg_armor_mitigation()`
- `rpg_defensive_power()`
- `rpg_proc_value()`
- `rpg_item_power()`
- `rpg_buff_value()`
- `rpg_cc_value()`
- `rpg_boss_hp()`
- `rpg_boss_damage()`
- `rpg_encounter_power_budget()`
- `rpg_value_clamp_caps()`

These functions are deterministic and contain no persistent state, making them suitable for authoritative server simulation, unit tests, balancing tools, and future UI/API exposure.
