# World Difficulty 1–9

World difficulty is a separate layer from character level and dungeon difficulty. It changes the danger and activity of the open world without changing the player's level.

## Levels

| Level | Name | Purpose |
|---:|---|---|
| 1 | Frontier | baseline open world |
| 2 | Dangerous | stronger patrols and more elites |
| 3 | Hostile | rare encounters and reinforced camps |
| 4 | Brutal | champions and stronger events |
| 5 | Deadly | elite reinforcements and hazards |
| 6 | Cataclysmic | mutated events and champion mechanics |
| 7 | Mythic World | mythic modifiers, extra boss phases and invasions |
| 8 | Apocalypse | catastrophic events and heavy elite activity |
| 9 | Eternal Nightmare | maximum open-world difficulty |

## What changes

World difficulty modifies:

- normal mob health and damage
- elite spawn frequency
- rare spawn frequency
- champion population
- world-boss power
- world-event frequency
- world-event mechanics
- environmental hazards
- NPC guard strength
- resource contest intensity
- open-world loot quality
- invasion frequency
- rare encounter mutations

## Difficulty stack

World difficulty and instance difficulty are independent.

```text
Character Level
       +
Gear / Build Power
       +
World Difficulty 1–9
       +
Dungeon Difficulty
       +
Encounter Modifiers
       =
Final Encounter Power
```

Example:

```text
Level 120 player
World Difficulty 7
Dungeon Difficulty Mythic
Boss Affix: Reinforced
```

The open-world layer uses World Difficulty 7 while the dungeon instance can independently use its own difficulty tier.

## Open-world formula

```text
FinalMobPower = BaseMobPower
              × WorldDifficultyScale
              × LevelScale
              × PlayerScale
```

World bosses additionally use participant scaling:

```text
FinalWorldBossPower = BaseBossPower
                    × WorldDifficultyScale
                    × AverageParticipantLevelScale
                    × ParticipationScale
```

## World state

A zone stores a current difficulty ID and can change it through server configuration, campaign progression, scheduled events, seasonal rules or administrative controls.

Existing zones already support dynamic states such as Peaceful, Threatened, Corrupted, Occupied, Liberated, Besieged, Reclaimed, Destroyed and Restored. World difficulty can operate alongside those states. 

## Level 9

Level 9 is designed as the extreme open-world mode. It enables:

- persistent invasion events
- mutated world bosses
- extreme environmental hazards
- multiple elite waves
- exclusive world loot
- enhanced rare spawns
- additional boss phases
- high-risk resource areas

It does not automatically change dungeon difficulty; dungeon difficulty remains independently selectable.

## Compatibility

World difficulty applies to the existing hierarchy:

```text
World
 -> Continent
  -> Region
   -> Biome
    -> SubBiome
     -> Zone
      -> SubZone
       -> Landmark
        -> Encounter
```

This preserves the existing world-map architecture and stable IDs used by the RPG layer.
