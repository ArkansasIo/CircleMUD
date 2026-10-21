# D&D 5e Compatible Rules Integration

## Rule-set target

CircleMUD should implement a selectable **5e-compatible rules profile** rather than embedding every rule directly into the legacy MUD engine.

The official D&D SRD is available under Creative Commons. SRD 5.1 supports the 2014-era fifth-edition rules, while SRD 5.2.1 is the current reference for the updated rules and is also CC-BY-4.0. citeturn0search0turn0search1

Recommended configuration:

```text
ruleset = DND5E_2014_COMPAT
# or
ruleset = DND5E_2024_COMPAT
```

Do not copy non-SRD book text, proprietary setting lore, or copyrighted adventure content into the repository.

## Core ability scores

```text
STR Strength
DEX Dexterity
CON Constitution
INT Intelligence
WIS Wisdom
CHA Charisma
```

## Ability modifier

For a score-based system:

```text
modifier = floor((score - 10) / 2)
```

Store scores as integers and compute modifiers at runtime.

## Proficiency

```text
proficiency_bonus(level)
```

The implementation should keep the proficiency progression in a ruleset table rather than scattering constants across combat code.

## Core d20 resolution

```text
roll = d20 + ability_modifier + proficiency_bonus_if_proficient + other_modifiers
success if roll >= target
```

Support advantage/disadvantage as a first-class roll mode:

```text
NORMAL
ADVANTAGE
DISADVANTAGE
```

## Saving throws

```text
STR_SAVE
DEX_SAVE
CON_SAVE
INT_SAVE
WIS_SAVE
CHA_SAVE
```

## Skills

```text
Acrobatics       DEX
AnimalHandling   WIS
Arcana           INT
Athletics        STR
Deception        CHA
History          INT
Insight          WIS
Intimidation     CHA
Investigation    INT
Medicine         WIS
Nature           INT
Perception       WIS
Performance      CHA
Persuasion       CHA
Religion         INT
SleightOfHand    DEX
Stealth          DEX
Survival         WIS
```

Skill mappings should be data-driven so the CircleMUD engine can add original skills.

## Combat model

### Initiative

```text
initiative = d20 + DEX modifier + initiative bonuses
```

### Armor Class

Maintain an AC abstraction that can represent both classic CircleMUD armor and 5e-style ascending defense.

```text
armor_class
legacy_armor_class
```

### Attack

```text
attack_roll = d20 + attack_modifier
hit if attack_roll >= armor_class
```

### Critical

The ruleset configuration controls the critical threshold and damage behavior.

### Damage

```text
damage = weapon_or_spell_dice + ability_modifier + bonuses
```

Damage types should be tagged rather than hard-coded:

```text
bludgeoning
piercing
slashing
acid
cold
fire
force
lightning
necrotic
poison
psychic
radiant
thunder
```

## Conditions

Implement conditions as flags with duration and source:

```text
Blinded
Charmed
Deafened
Exhaustion
Frightened
Grappled
Incapacitated
Invisible
Paralyzed
Petrified
Poisoned
Prone
Restrained
Stunned
Unconscious
```

The condition engine should expose:

```c
apply_condition(character, condition, source, duration);
remove_condition(character, condition);
has_condition(character, condition);
```

## Actions

Support an action economy abstraction:

```text
ACTION
BONUS_ACTION
REACTION
MOVEMENT
FREE_INTERACTION
```

The legacy MUD command system can translate commands into actions without replacing the existing command interpreter.

## Character progression

```text
Level
Experience
HitDice
ProficiencyBonus
AbilityScores
SavingThrows
Skills
Features
Feats
Equipment
```

## Class architecture

D&D-compatible classes can coexist with the Warcraft-inspired MMORPG classes as separate registries:

```text
Ruleset
 ├── DND5E class registry
 ├── MMO class registry
 └── CircleMUD legacy class registry
```

## Spell system

Use a data-driven spell record:

```text
SpellId
Name
Level
School
CastingTime
Range
Components
Duration
TargetType
DamageType
SavingThrow
AttackType
Dice
Scaling
Concentration
Ritual
AreaShape
```

Spell schools:

```text
Abjuration
Conjuration
Divination
Enchantment
Evocation
Illusion
Necromancy
Transmutation
```

## Spell slots and resources

The ruleset layer should support both traditional spell slots and MMORPG resources:

```text
ResourceSystem
 ├── SpellSlots
 ├── Mana
 ├── Rage
 ├── Energy
 ├── Focus
 ├── RunicPower
 └── Custom
```

## Rest and recovery

Implement configurable recovery profiles:

```text
SHORT_REST
LONG_REST
ZONE_REST
COMBAT_REGEN
OUT_OF_COMBAT_REGEN
```

## Equipment

Items should expose:

```text
ArmorClass
WeaponDamage
DamageType
AbilityRequirements
ProficiencyRequirements
Properties
Weight
Rarity
MagicBonus
Charges
Durability
```

## Feat system

```text
FeatId
Name
Prerequisites
PassiveEffects
ActiveEffects
AbilityScoreChanges
Tags
```

## Monster conversion

Every CircleMUD mob can optionally receive a 5e-compatible profile:

```text
CreatureType
ChallengeRating
ArmorClass
HitPoints
Speed
AbilityScores
SavingThrows
Skills
DamageResistances
DamageImmunities
ConditionImmunities
Senses
Languages
Actions
Reactions
LegendaryActions
```

The classic mob data remains usable when no 5e profile exists.

## Encounter difficulty

Support:

```text
TRIVIAL
EASY
MEDIUM
HARD
DEADLY
EPIC
```

A separate MMO scaling layer can calculate encounters for solo, party, dungeon, and raid content.

## Source organization

```text
src/rules/
  ruleset.h
  ruleset.c
  dnd5e.h
  dnd5e.c
  dnd5e_dice.h
  dnd5e_dice.c
  dnd5e_checks.h
  dnd5e_checks.c
  dnd5e_combat.h
  dnd5e_combat.c
  dnd5e_conditions.h
  dnd5e_conditions.c
  dnd5e_spells.h
  dnd5e_spells.c
  dnd5e_actions.h
  dnd5e_actions.c
  dnd5e_character.h
  dnd5e_character.c
  dnd5e_monsters.h
  dnd5e_monsters.c
```

## Testing requirements

Add deterministic tests for:

1. ability modifiers
2. proficiency progression
3. normal/advantage/disadvantage rolls
4. saving throws
5. skill checks
6. attack resolution
7. critical resolution
8. damage types
9. conditions and duration
10. action economy
11. spell targeting
12. rest recovery
13. character level progression
14. monster conversion
15. legacy CircleMUD compatibility

## Attribution

For content derived from SRD 5.1 or SRD 5.2.1, include the appropriate Creative Commons attribution required by the applicable SRD license. The official D&D Beyond SRD page identifies SRD 5.2.1 as the current SRD and provides the applicable attribution/licensing information. citeturn0search0
