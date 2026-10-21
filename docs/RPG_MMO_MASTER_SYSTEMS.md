# RPG/MMORPG Master Systems

This document defines the complete gameplay surface for the 1–720 CircleMUD RPG/MMORPG layer.

## Character

- level 1–720
- experience and milestone progression
- race, racial type and subtype
- class, specialization and role
- ten primary attributes
- secondary combat statistics
- resources and resistances
- talents and passive traits
- abilities and spell books
- equipment and loadouts
- titles, achievements and cosmetics

## Equipment

Weapons: swords, axes, maces, daggers, spears, polearms, bows, crossbows, staffs, wands, tomes, shields and unarmed/fist weapons.

Armor: cloth, leather, mail, plate and cosmetic equipment.

Slots: head, neck, shoulders, chest, back, wrists, hands, waist, legs, feet, two rings, two trinkets, weapon/off-hand/two-hand/ranged slots.

Equipment supports rarity, item level, durability, binding, affixes, sockets, upgrades, set bonuses, transmog/cosmetic appearance and randomized rolls.

## Item families

- weapons
- armor
- accessories
- consumables
- crafting materials
- recipes
- quest items
- currency items
- mounts
- pets
- cosmetics
- collectibles
- keys
- dungeon/raid tokens
- reputation items

## World population

The world supports normal mobs, elites, rare mobs, champions, named NPCs, faction guards, quest NPCs, vendors, trainers, profession masters, traveling NPCs, event NPCs and world bosses.

Mobs have level ranges, archetypes, factions, behavior profiles, abilities, resistances, loot tables, spawn rules, patrol paths and encounter tags.

## World bosses

World bosses are persistent open-world encounters. They can scale to participating players and average player level. A world boss may own a world-event state, spawn announcement, map marker, encounter phase system, contribution tracker, loot eligibility, achievements, titles and reputation rewards.

## Difficulty

Dungeon and instanced content uses five difficulties:

1. **Normal** — baseline mechanics.
2. **Veteran** — stronger enemies and additional mechanics.
3. **Hard** — substantially stronger enemies, enhanced bosses and reduced recovery.
4. **Nightmare** — encounter affixes, enrage timers, limited resurrection and elite reinforcement mechanics.
5. **Ultimate Nightmare** — maximum difficulty, randomized encounter mutations, no automatic resurrection, exclusive loot and leaderboard eligibility.

Difficulty modifies enemy health, enemy damage, loot quality, mechanics, rewards and encounter behavior. Difficulty is independent of character level.

## Dungeons

Every dungeon contains:

- entrance
- matchmaking requirements
- party size
- recommended level
- difficulty list
- rooms or encounter nodes
- trash packs
- elites
- objectives
- checkpoints
- bosses
- loot tables
- completion rewards
- achievements
- timers where applicable

## Trails

Trails are staged expedition content. Each trail contains stages, branching objectives, checkpoints, optional challenges, a final encounter, medal grading and escalating rewards.

## Raids

Raids contain wings, trash encounters, bosses, phases, role requirements, wipe/reset handling, lockouts, loot distribution, achievements and difficulty-specific mechanics. Supported party profiles are configurable, including 10/20/40-player templates.

## Tower

The Tower contains 100 floors. Every ten floors is a checkpoint/boss milestone, with floor 100 as the final tower encounter. Floors can contain combat, elite, hazard, challenge and reward encounters.

## Economy

- currencies
- vendors
- repair
- auction/trading layer
- crafting economy
- resource gathering
- item sinks
- repair sinks
- progression currencies
- dungeon and raid tokens

## Social

- party
- raid groups
- guilds
- friends
- ignore list
- mail
- chat channels
- group finder
- guild progression
- guild bank
- guild achievements

## PvE and PvP

PvE includes open-world encounters, quests, dungeons, trails, raids, world events and tower content.

PvP can include duels, arenas, battlegrounds, open-world PvP zones, faction conflicts, ratings and seasonal rewards.

## World simulation

- day/night cycle
- weather
- seasons
- dynamic events
- faction control
- NPC schedules
- resource respawns
- world boss schedules
- zone states
- exploration discovery
- travel routes
- mounts
- teleportation/recall

## Content scaling

`EncounterPower = BasePower × LevelScale × DifficultyScale × PlayerScale × AffixScale`

`RewardPower = BaseReward × DifficultyReward × EncounterTier × ParticipationModifier`

All multipliers are data-driven and should be versioned so balance changes do not require source-code rewrites.

## Source/data boundary

C owns authoritative runtime state. JSON/data registries define content. Lua handles controlled gameplay scripting. UI consumes API contracts. Assets are referenced by stable IDs instead of hard-coded filesystem paths.
