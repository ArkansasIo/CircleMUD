# CircleMUD Quest System

## Purpose

The quest system provides a compatibility-friendly framework for the 12-act campaign and future procedural content. It separates narrative state from legacy room and character structures.

## Quest lifecycle

```text
LOCKED -> AVAILABLE -> ACTIVE -> OBJECTIVES_COMPLETE -> REWARD_PENDING -> COMPLETE
                  \-> FAILED -> AVAILABLE
```

A quest can also be `ABANDONED`, `HIDDEN`, or `EXPIRED` when those states are needed.

## Core systems

### Quest Registry

Stores static quest definitions and indexes them by ID, act, zone, faction, and prerequisite.

### Quest State

Stores player-specific progress:

- active objective counters
- completion state
- timestamps
- selected choices
- reward state
- prerequisite flags

### Story State

Stores campaign-level flags and act progression separately from individual quest records.

### Objective Engine

Consumes gameplay events such as kills, item pickups, dialogue, room entry, crafting, escort progress, and boss victories.

### Reward Engine

Grants XP, currency, equipment, crafting resources, reputation, unlocks, and story flags exactly once.

## Side quests

Side quests use the same engine as main quests but have `QuestType=Side`. They can be:

- one-time
- repeatable
- daily
- faction-specific
- companion-specific
- zone-specific
- hidden until discovered
- dependent on a main quest choice

## Quest commands

Recommended player commands:

```text
quest
quests
quest list
quest active
quest completed
quest abandon <id>
quest track <id>
quest objectives <id>
quest lore <id>
```

Recommended administrative commands:

```text
questadmin show <player> <quest>
questadmin start <player> <quest>
questadmin complete <player> <quest>
questadmin reset <player> <quest>
questadmin flag <player> <flag> <value>
questadmin act <player> <number>
```

## Event integration

Gameplay events should publish small, stable events rather than directly manipulating quest records.

Examples:

```text
EVENT_NPC_TALKED
EVENT_MOB_KILLED
EVENT_ITEM_ACQUIRED
EVENT_ROOM_ENTERED
EVENT_ITEM_CRAFTED
EVENT_BOSS_DEFEATED
EVENT_FACTION_CHANGED
EVENT_STORY_FLAG_CHANGED
```

Quest objective handlers subscribe to relevant events.

## Compatibility rule

The quest system must not require every existing CircleMUD command to understand quests. Integration should occur at well-defined event points. Existing gameplay remains functional when the campaign feature is disabled.

## Persistence

Quest state should be versioned. Suggested record format:

```text
QUEST <id> <state> <objective_progress...>
FLAG <name> <value>
ACT <number>
CHOICE <id> <value>
```

A migration layer should tolerate missing optional fields so old player files can continue to load.

## Testing requirements

Test at minimum:

1. prerequisites block unavailable quests
2. completing prerequisites unlocks dependent quests
3. objective progress survives save/load
4. rewards are granted once
5. repeated event delivery does not duplicate rewards
6. abandoned quests can be restarted where allowed
7. story flags persist
8. faction requirements work
9. act transitions are deterministic
10. old characters without quest records remain valid
