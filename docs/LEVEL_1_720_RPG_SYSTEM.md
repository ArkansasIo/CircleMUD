# CircleMUD Level 1–720 RPG/MMORPG System

## Progression

The game uses a 720-level progression model divided into 72 ten-level bands. Level is an integer in `[1,720]` and is separate from item level, encounter tier, and content difficulty.

### Level bands

| Levels | Tier | Design purpose |
|---|---|---|
| 1–10 | Initiate | tutorial and first zone |
| 11–20 | Adventurer | early dungeons |
| 21–30 | Vanguard | advanced zones |
| 31–40 | Veteran | veteran content |
| 41–50 | Champion | heroic content |
| 51–60 | Heroic | raids and advanced trails |
| 61–70 | Mythic | mythic progression |
| 71–80 | Ascendant | high-end world |
| 81–90 | Paragon | endgame progression |
| 91–100 | Apex | apex encounters |
| 101–120 | Transcendent I | post-100 progression |
| 121–180 | Ascension I | expansion-style progression |
| 181–240 | Ascension II | expansion-style progression |
| 241–300 | Ascension III | expansion-style progression |
| 301–360 | Ascension IV | expansion-style progression |
| 361–420 | Ascension V | expansion-style progression |
| 421–480 | Ascension VI | expansion-style progression |
| 481–540 | Ascension VII | expansion-style progression |
| 541–600 | Ascension VIII | expansion-style progression |
| 601–660 | Ascension IX | expansion-style progression |
| 661–719 | Ascension X | final progression band |
| 720 | Eternal Apex | ultimate level |

## XP

Base requirement:

`XP(level) = floor(500 * level^2.35)`

Late-game progression may use configurable diminishing multipliers, rested XP, catch-up XP, quest XP, dungeon XP, raid XP, exploration XP, achievement XP, and account-wide bonuses.

## Level rewards

Every level can award one or more of:

- attribute points
- talent points
- ability unlocks
- passive traits
- equipment slots
- content access
- travel options
- crafting recipes
- titles
- cosmetic rewards
- reputation thresholds

Milestone levels are configurable and default to 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, then every 20 levels through 720.

## Power separation

Character power is calculated from:

`CharacterPower = LevelScale × Attributes × GearScale × TalentScale × AbilityScale × TemporaryModifiers`

Level alone does not determine victory. Encounters, mechanics, resistances, roles, builds, equipment and player decisions remain relevant.

## Level-aware content

Every zone, mob, NPC, quest, dungeon, trail, raid, world boss and tower floor declares a recommended level range and scaling policy.
