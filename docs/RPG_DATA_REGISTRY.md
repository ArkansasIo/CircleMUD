# CircleMUD RPG Data Registry

This registry turns the RPG design into implementation-ready data. Names are used as fantasy archetypes; the framework is intentionally data-driven so the project can add original races and classes without changing the engine.

## 1. Race registry

```text
01 Human          | Alliance/Horde compatible | Humanoid | Adaptable
02 Dwarf          | Alliance                 | Humanoid | Stoneborn
03 Night Elf      | Alliance                 | Humanoid | Elven
04 Gnome          | Alliance                 | Humanoid | Mechanical
05 Draenei        | Alliance                 | Humanoid | Draenic
06 Worgen         | Alliance                 | Humanoid | Beastkin
07 Orc            | Horde                    | Humanoid | Orcish
08 Forsaken       | Horde                    | Undead   | Forsaken
09 Tauren         | Horde                    | Humanoid | Tauren
10 Troll          | Horde                    | Humanoid | Trollkin
11 Blood Elf      | Horde                    | Humanoid | Elven
12 Goblin         | Horde                    | Humanoid | Goblin
13 Pandaren       | Neutral                  | Humanoid | Pandaren
14 Draenei-LF     | Alliance                 | Humanoid | Lightforged
15 Void Elf       | Alliance                 | Humanoid | Void-touched
16 Dark Iron      | Alliance                 | Humanoid | Dwarven
17 Kul Tiran      | Alliance                 | Humanoid | Humanlike
18 Mechagnome     | Alliance                 | Construct| Mechanical
19 Nightborne     | Horde                    | Humanoid | Elven
20 Highmountain   | Horde                    | Humanoid | Tauren
21 Mag'har Orc    | Horde                    | Humanoid | Orcish
22 Zandalari      | Horde                    | Humanoid | Trollkin
23 Vulpera        | Horde                    | Humanoid | Beastkin
24 Dracthyr       | Neutral                  | Dragonkin| Draconic
25 Earthen        | Alliance/Horde          | Construct| Stoneborn
26 Haranir        | Alliance/Horde          | Humanoid | Rootborn
```

Haranir are included because Blizzard currently lists them as a playable allied race in Midnight; they can join either faction and use Druid, Hunter, Mage, Monk, Priest, Rogue, Shaman, Warlock, or Warrior. citeturn0search0turn0search1

## 2. Class registry

```text
01 Warrior
02 Paladin
03 Hunter
04 Rogue
05 Priest
06 Death Knight
07 Shaman
08 Mage
09 Warlock
10 Monk
11 Druid
12 Demon Hunter
13 Evoker
```

## 3. Subclass registry

```text
Warrior       : Arms, Fury, Protection
Paladin       : Holy, Protection, Retribution
Hunter        : Beast Mastery, Marksmanship, Survival
Rogue         : Assassination, Outlaw, Subtlety
Priest        : Discipline, Holy, Shadow
Death Knight  : Blood, Frost, Unholy
Shaman        : Elemental, Enhancement, Restoration
Mage          : Arcane, Fire, Frost
Warlock       : Affliction, Demonology, Destruction
Monk          : Brewmaster, Mistweaver, Windwalker
Druid         : Balance, Feral, Guardian, Restoration
Demon Hunter  : Havoc, Vengeance, Devourer
Evoker        : Devastation, Preservation, Augmentation
```

Blizzard's Midnight release notes confirm Devourer as the Demon Hunter's third specialization. citeturn0search1turn0search2

## 4. Role registry

```text
TANK
HEALER
MELEE_DPS
RANGED_DPS
SUPPORT
CONTROLLER
SOLO
PETMASTER
```

## 5. Combat types

```text
MELEE
RANGED
SPELLCASTER
HYBRID
PETMASTER
SUPPORT
SHAPESHIFTER
SUMMONER
```

## 6. Combat subtypes

```text
WeaponMaster
Berserker
Guardian
Crusader
HolyCaster
ShadowCaster
Elementalist
Beastmaster
Marksman
Assassin
Duelist
Runeblade
Necromancer
FrostCaster
FireCaster
ArcaneCaster
Demonologist
SoulHarvester
MartialArtist
Mistweaver
Shapeshifter
NatureCaster
FelCaster
VoidCaster
DragonCaster
Augmenter
BattleSupport
```

## 7. Primary stats

```text
STR  Strength
DEX  Dexterity
CON  Constitution
INT  Intelligence
WIS  Wisdom
VIT  Vitality
SPI  Spirit
LCK  Luck
WIL  Willpower
CHA  Charisma
```

## 8. Secondary stats

```text
AttackPower
SpellPower
RangedPower
Armor
CriticalStrike
CriticalDamage
Haste
Accuracy
Expertise
ArmorPenetration
SpellPenetration
Dodge
Parry
Block
Avoidance
Tenacity
Versatility
Mastery
Leech
ResourceEfficiency
MovementSpeed
```

## 9. Resource stats

```text
MaxHealth
HealthRegen
MaxMana
ManaRegen
MaxEnergy
EnergyRegen
MaxRage
RageGeneration
MaxFocus
FocusRegen
MaxRunicPower
RunicPowerGeneration
MaxHolyPower
MaxMaelstrom
MaxChi
MaxFury
MaxEssence
```

## 10. Resistance stats

```text
PhysicalResistance
ArcaneResistance
FireResistance
FrostResistance
NatureResistance
ShadowResistance
HolyResistance
FelResistance
VoidResistance
ElementalResistance
PoisonResistance
DiseaseResistance
BleedResistance
FearResistance
SilenceResistance
StunResistance
RootResistance
SlowResistance
```

## 11. Derived combat values

```text
MaxHP
MaxMP
AttackRating
SpellRating
ArmorRating
PhysicalDamage
MagicDamage
HealingPower
CriticalChance
CriticalMultiplier
DodgeChance
ParryChance
BlockChance
DamageReduction
ThreatGeneration
ThreatReduction
MovementRate
CastSpeed
AttackSpeed
CooldownRecovery
ResourceRecovery
```

## 12. Stat scaling model

All class and race modifiers are additive data records followed by multiplicative scaling rules.

```text
BaseStat
+ RaceModifier
+ ClassModifier
+ SubclassModifier
+ LevelGrowth
+ EquipmentBonus
+ TalentBonus
+ BuffBonus
- DebuffPenalty
= EffectiveStat
```

Then:

```text
EffectiveStat
 -> SecondaryRating
 -> DerivedCombatValue
 -> AbilityCoefficient
 -> FinalResult
```

## 13. Example class profiles

### Warrior

```text
Primary: STR, CON, VIT
Secondary: CriticalStrike, Haste, Armor
Resources: Rage
Types: MELEE, HYBRID

Arms       -> MELEE_DPS
Fury       -> MELEE_DPS
Protection -> TANK
```

### Paladin

```text
Primary: STR, WIS, VIT
Secondary: Armor, Versatility, CriticalStrike
Resources: Mana, HolyPower
Types: MELEE, SPELLCASTER, HYBRID

Holy        -> HEALER
Protection  -> TANK
Retribution -> MELEE_DPS
```

### Mage

```text
Primary: INT
Secondary: SpellPower, Haste, CriticalStrike, Mastery
Resource: Mana
Type: SPELLCASTER

Arcane -> RANGED_DPS
Fire   -> RANGED_DPS
Frost  -> RANGED_DPS/CONTROL
```

### Druid

```text
Primary: INT, AGI/DEX, WIS, VIT
Secondary: Mastery, Haste, CriticalStrike
Resources: Mana + form-specific resources
Type: SHAPESHIFTER/HYBRID

Balance    -> RANGED_DPS
Feral      -> MELEE_DPS
Guardian   -> TANK
Restoration-> HEALER
```

### Demon Hunter

```text
Primary: DEX, VIT, WIL
Secondary: Haste, CriticalStrike, Mastery
Resources: Fury
Type: MELEE/RANGED/HYBRID

Havoc      -> MELEE_DPS
Vengeance  -> TANK
Devourer   -> RANGED_DPS
```

Devourer is modeled as a Void-oriented ranged specialization, matching Blizzard's current Midnight description. citeturn0search2

## 14. Hero/Apex extension layer

The registry supports a fourth specialization layer without changing the base class model:

```text
Class
 -> Subclass
   -> HeroSpecialization
     -> ApexTalent
```

Examples:

```text
Warrior -> Arms -> HeroTree -> ApexTalent
Druid -> Guardian -> HeroTree -> ApexTalent
DemonHunter -> Devourer -> Annihilator/Scarred -> ApexTalent
```

Blizzard's Midnight system expands specialization progression with Hero Talents and Apex Talents, so this is represented as a separate extension layer rather than another class. citeturn0search1turn0search5

## 15. Recommended runtime structs

```c
struct rpg_stats {
  int strength;
  int dexterity;
  int constitution;
  int intelligence;
  int wisdom;
  int vitality;
  int spirit;
  int luck;
  int willpower;
  int charisma;
};

struct rpg_secondary_stats {
  int attack_power;
  int spell_power;
  int armor;
  int crit_rating;
  int haste_rating;
  int mastery_rating;
  int versatility_rating;
  int accuracy_rating;
  int dodge_rating;
  int parry_rating;
  int block_rating;
};

struct rpg_character_profile {
  int race_id;
  int class_id;
  int subclass_id;
  int role_id;
  int combat_type;
  int combat_subtype;
  int level;
  long experience;
  struct rpg_stats primary;
  struct rpg_secondary_stats secondary;
};
```

## 16. Next implementation stage

Implement the registry as actual C modules:

```text
src/rpg/rpg_registry.h
src/rpg/rpg_registry.c
src/rpg/rpg_races.h
src/rpg/rpg_races.c
src/rpg/rpg_classes.h
src/rpg/rpg_classes.c
src/rpg/rpg_stats.h
src/rpg/rpg_stats.c
src/rpg/rpg_combat.h
src/rpg/rpg_combat.c
src/rpg/rpg_character.h
src/rpg/rpg_character.c
```

Then add:

```text
quest -> class/race requirements
combat -> stat scaling
skills -> subclass requirements
talents -> subclass requirements
equipment -> stat modifiers
NPCs -> class/race profiles
mobs -> type/subtype profiles
```

The legacy CircleMUD player structure remains authoritative until an RPG profile is explicitly enabled.