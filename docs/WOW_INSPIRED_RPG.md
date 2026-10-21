# CircleMUD Warcraft-Inspired RPG Framework

> This is an original CircleMUD implementation specification inspired by the broad MMORPG pattern of races, classes, roles, specializations, talents, stats, and derived stats. It is not a copy of Blizzard game data or assets.

## Current reference model

The modern World of Warcraft model has expanded beyond its original class/race roster. Blizzard's current Midnight material adds the Haranir as a playable allied race and adds Devourer as a Demon Hunter specialization. citeturn0search0turn0search2

CircleMUD should therefore use a **data-driven registry** rather than hard-code race/class combinations into the player structure.

# 1. Character hierarchy

```text
Faction
  -> Race
    -> Racial Type
      -> Racial Subtype
        -> Class
          -> Specialization
            -> Role
              -> Combat Type
                -> Combat Subtype
                  -> Stats
                    -> Sub-Stats
                      -> Skills
                        -> Talents
                          -> Abilities
                            -> Equipment
```

A character may additionally have:

- profession
- secondary profession
- faction reputation
- covenant/faction allegiance
- guild role
- PvE role
- PvP role
- crafting specialization
- companion specialization
- achievement traits

# 2. Races

Use the following as reference-compatible race families, while allowing CircleMUD to add original races.

## Alliance families

- Human
- Dwarf
- Night Elf
- Gnome
- Draenei
- Worgen
- Pandaren
- Void Elf
- Lightforged Draenei
- Dark Iron Dwarf
- Kul Tiran
- Mechagnome
- Earthen
- Highmountain-style Tauren-compatible custom race

## Horde families

- Orc
- Undead/Forsaken
- Tauren
- Troll
- Blood Elf
- Goblin
- Pandaren
- Nightborne
- Highmountain Tauren
- Mag'har Orc
- Zandalari Troll
- Vulpera

## Neutral / allied / expandable

- Pandaren
- Dracthyr
- Earthen
- Haranir
- Void-aligned custom races
- Original CircleMUD races

The Earthen are an official playable race in The War Within, and Haranir are an official playable allied race in Midnight. citeturn0search8turn0search0

## Race record

```text
RaceId
Name
FactionAffinity
RaceFamily
RacialType
RacialSubtype
Size
BaseHealth
BaseMana
BaseEnergy
BaseArmor
BaseMove
PrimaryStatBias
SecondaryStatBias
Resistances
Immunities
MovementType
Languages
StartingZone
AvailableClasses
RacialTraits
UnlockRequirements
```

# 3. Racial types and subtypes

Every race receives tags that can drive rules without special-case C code.

### Physical type

- Humanoid
- Beastkin
- Giantkin
- Elemental
- Dragonkin
- Construct
- Undead
- Demon
- Plant
- Aquatic
- Insectoid
- Voidborn

### Physical subtypes

Examples:

- Humanlike
- Elven
- Dwarven
- Orcish
- Trollkin
- Taurenkin
- Goblinlike
- Fungal
- Stoneborn
- Draconic
- Fel-touched
- Void-touched
- Mechanical

# 4. Classes

The modern Warcraft class model includes 13 major classes in the retail-style ruleset represented here:

1. Warrior
2. Paladin
3. Hunter
4. Rogue
5. Priest
6. Death Knight
7. Shaman
8. Mage
9. Warlock
10. Monk
11. Druid
12. Demon Hunter
13. Evoker

Blizzard's published material confirms the class/spec structure and the expanded race/class system; Midnight specifically adds Devourer for Demon Hunters. citeturn0search2turn0search3

# 5. Class specializations / subclasses

For CircleMUD, call these **subclasses** in code while retaining a display label of Specialization.

| Class | Subclasses | Primary roles |
|---|---|---|
| Warrior | Arms, Fury, Protection | DPS, Tank |
| Paladin | Holy, Protection, Retribution | Healer, Tank, DPS |
| Hunter | Beast Mastery, Marksmanship, Survival | DPS |
| Rogue | Assassination, Outlaw, Subtlety | DPS |
| Priest | Discipline, Holy, Shadow | Healer, DPS |
| Death Knight | Blood, Frost, Unholy | Tank, DPS |
| Shaman | Elemental, Enhancement, Restoration | DPS, Healer |
| Mage | Arcane, Fire, Frost | DPS |
| Warlock | Affliction, Demonology, Destruction | DPS |
| Monk | Brewmaster, Mistweaver, Windwalker | Tank, Healer, DPS |
| Druid | Balance, Feral, Guardian, Restoration | DPS, Tank, Healer |
| Demon Hunter | Havoc, Vengeance, Devourer | DPS, Tank |
| Evoker | Devastation, Preservation, Augmentation | DPS, Healer, Support |

Devourer is the current additional Demon Hunter specialization in Midnight and uses Void-based ranged gameplay. citeturn0search2turn0search10

# 6. Class types

Each class receives one or more capability types.

### Combat type

- Melee
- Ranged
- Spellcaster
- Hybrid
- Petmaster
- Support

### Role type

- Tank
- Healer
- Melee DPS
- Ranged DPS
- Support
- Controller
- Solo

### Resource type

- Mana
- Rage
- Energy
- Combo Points
- Runic Power
- Runes
- Focus
- Holy Power
- Maelstrom
- Chi
- Fury
- Essence
- CustomResource

# 7. Subtypes

Subtypes allow deeper specialization without creating hundreds of classes.

Examples:

```text
Warrior
  -> Melee
    -> WeaponMaster
    -> Berserker
    -> Defender

Mage
  -> Spellcaster
    -> ArcaneCaster
    -> FireCaster
    -> FrostCaster

Hunter
  -> Ranged
    -> Beastmaster
    -> Marksman
    -> Survivalist

Druid
  -> Hybrid
    -> Shapeshifter
    -> NatureCaster
    -> Guardian
    -> Feral

Paladin
  -> Hybrid
    -> HolyKnight
    -> Protector
    -> Crusader
```

# 8. Primary stats

CircleMUD should support a larger RPG stat model than the original MUD while keeping the values data-driven.

### Core attributes

- Strength — physical power and melee scaling
- Dexterity — accuracy, agility, avoidance
- Constitution — physical durability
- Intelligence — spell power and mana scaling
- Wisdom — healing, perception, spiritual power
- Vitality — maximum health and regeneration
- Spirit — resource regeneration and resistance
- Luck — chance-based outcomes
- Willpower — mental resistance and resource stability
- Charisma — social influence, leadership, trading

# 9. Combat sub-stats

### Offensive

- Attack Power
- Spell Power
- Ranged Power
- Weapon Damage
- Bonus Damage
- Armor Penetration
- Spell Penetration
- Critical Strike
- Critical Damage
- Haste
- Accuracy
- Expertise

### Defensive

- Armor
- Physical Resistance
- Magic Resistance
- Block
- Parry
- Dodge
- Avoidance
- Damage Reduction
- Critical Damage Reduction
- Tenacity
- Crowd Control Resistance

### Resource

- Max Health
- Health Regeneration
- Max Mana
- Mana Regeneration
- Max Energy
- Energy Regeneration
- Max Rage
- Rage Generation
- Max Focus
- Focus Regeneration
- Resource Efficiency

### Healing

- Healing Power
- Critical Healing
- Absorption
- Overheal Efficiency
- Healing Received
- Healing Done
- HoT Power
- Shield Power

### Movement

- Move Speed
- Sprint Speed
- Mount Speed
- Swim Speed
- Jump Power
- Fall Resistance
- Movement Control Resistance

# 10. Magical sub-stats

- Arcane Power
- Fire Power
- Frost Power
- Nature Power
- Shadow Power
- Holy Power
- Fel Power
- Void Power
- Elemental Power
- Spell Crit
- Spell Haste
- Spell Accuracy
- Spell Resistance

These are tags rather than mandatory independent numerical stats; a build can enable only the schools it uses.

# 11. Derived stats

Suggested formulas are configuration data, not hard-coded constants.

```text
MaxHP = BaseHP + Vitality*HPPerVitality + Constitution*HPPerConstitution + GearHP
MaxMana = BaseMana + Intelligence*ManaPerIntelligence + Spirit*ManaPerSpirit + GearMana
AttackPower = BaseAttack + Strength*StrengthScaling + GearAttack
SpellPower = BaseSpellPower + Intelligence*SpellScaling + GearSpellPower
Armor = BaseArmor + Dexterity*ArmorScaling + GearArmor
CritChance = BaseCrit + CritRating/RatingScale
Haste = HasteRating/RatingScale
Dodge = BaseDodge + Dexterity*DodgeScaling
Parry = BaseParry + Strength*ParryScaling
```

All formulas should be versioned in a game-balance data table.

# 12. Stat hierarchy

```text
Primary Stat
  -> Secondary Stat
    -> Combat Rating
      -> Derived Stat
        -> Ability Scaling
          -> Final Combat Result
```

Example:

```text
Strength
 -> Attack Power
   -> Weapon Damage
     -> Ability Coefficient
       -> Final Physical Damage
```

# 13. Talent system

Every subclass can expose three talent categories:

### Core talents

Define the identity of the subclass.

### Utility talents

Movement, control, defense, resource management, and quality-of-life effects.

### Mastery talents

Endgame modifiers that alter rotations, builds, and ability interactions.

Recommended node types:

- Passive
- Active
- Modifier
- Proc
- Transformation
- Resource
- Defensive
- Offensive
- Utility

# 14. Skill categories

```text
Weapon Skills
Armor Skills
Class Skills
Subclass Skills
Race Skills
Profession Skills
Crafting Skills
Gathering Skills
Social Skills
Survival Skills
Mount Skills
Exploration Skills
Faction Skills
```

# 15. Equipment types

### Weapon families

- Sword
- Axe
- Mace
- Dagger
- Polearm
- Spear
- Staff
- Wand
- Bow
- Crossbow
- Gun
- Shield
- Tome
- Fist weapon

### Armor families

- Cloth
- Leather
- Mail
- Plate
- Shield

### Equipment slots

- Head
- Neck
- Shoulders
- Back
- Chest
- Wrists
- Hands
- Waist
- Legs
- Feet
- Ring 1
- Ring 2
- Trinket 1
- Trinket 2
- Main Hand
- Off Hand
- Ranged

# 16. Character build object

```text
CharacterBuild
  Race
  RacialType
  RacialSubtype
  Class
  Subclass
  CombatType
  CombatSubtype
  Role
  Level
  Experience
  PrimaryStats[]
  SecondaryStats[]
  DerivedStats[]
  Skills[]
  Talents[]
  Abilities[]
  Equipment[]
  Resistances[]
  Resources[]
  FactionStanding[]
```

# 17. CircleMUD implementation

Recommended source organization:

```text
src/rpg/
  race.h
  race.c
  class.h
  class.c
  subclass.h
  subclass.c
  stats.h
  stats.c
  derived_stats.h
  derived_stats.c
  talents.h
  talents.c
  skills.h
  skills.c
  abilities.h
  abilities.c
  equipment.h
  equipment.c
  character_build.h
  character_build.c
  quest_hooks.h
  quest_hooks.c
```

Recommended data files:

```text
data/rpg/
  races.json
  classes.json
  subclasses.json
  racial_traits.json
  stats.json
  stat_formulas.json
  skills.json
  talents.json
  abilities.json
  equipment_types.json
  class_race_rules.json
```

The initial implementation should preserve the classic CircleMUD character model and attach an optional RPG profile. This avoids forcing every legacy player/NPC record to use the expanded system.

# 18. Database schema

For a future SQL backend:

```text
rpg_races
rpg_race_traits
rpg_classes
rpg_subclasses
rpg_class_types
rpg_subtypes
rpg_stats
rpg_stat_formulas
rpg_character_stats
rpg_character_subclasses
rpg_talents
rpg_character_talents
rpg_skills
rpg_character_skills
rpg_abilities
rpg_character_abilities
rpg_equipment_types
rpg_character_equipment
rpg_factions
rpg_character_factions
```

# 19. Design rule

Do not hard-code race/class combinations throughout CircleMUD. Store eligibility as data:

```text
race_id
class_id
allowed
requirements
exceptions
```

This makes the system capable of supporting both Warcraft-inspired builds and completely original CircleMUD races/classes later.
