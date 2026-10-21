# CircleMUD Story Campaign System

## Overview

This document defines an optional, data-driven narrative campaign layer for CircleMUD. The campaign is designed to sit above the classic MUD world model without requiring the core game to become story-driven.

The campaign contains **12 acts**, **50 main quests**, and optional side-quest hooks. Each act has a narrative purpose, a gameplay escalation, a zone focus, and a state transition. Quest completion is tracked independently from ordinary room, object, NPC, and combat state.

## Campaign: The Broken Circle

A long-sealed force beneath the old world begins disrupting the ancient Circle wards. The player discovers that the disturbances are not random: twelve wardstones are failing, each connected to a forgotten conflict between kingdoms, cults, mages, and the creatures beyond the known world.

The player must recover the wardstones, uncover who is deliberately breaking the Circle, unite rival factions, and decide what should replace the ancient order.

### Campaign state

Recommended persistent fields:

- `campaign_id`
- `campaign_version`
- `current_act`
- `completed_quests`
- `active_quests`
- `discovered_zones`
- `faction_reputation`
- `story_flags`
- `major_choices`
- `bosses_defeated`
- `wardstones_recovered`
- `campaign_completion`

Story flags should be additive and deterministic. A quest should never require deleting historical state.

## Twelve Acts

### Act I — The First Crack

The player notices unnatural disturbances around the starting settlement. Missing travelers and corrupted wildlife point toward the first broken ward.

**Quests:** 1–4
**Boss:** The Cracked Warden
**Story turn:** The Circle is failing.

### Act II — Ashes of the Old Road

The player follows destroyed caravans along an abandoned road and discovers evidence of an organized attack rather than ordinary banditry.

**Quests:** 5–8
**Boss:** Ashroad Marauder Captain
**Story turn:** Someone is testing the defenses.

### Act III — The Drowned Archive

An ancient archive beneath flooded ruins contains records of the original Circle and the first wardstone.

**Quests:** 9–12
**Boss:** Archivist of the Deep
**Story turn:** The Circle was created to contain something, not merely protect civilization.

### Act IV — Kingdoms at Odds

The player becomes involved in a political conflict between two settlements whose leaders blame each other for the disturbances.

**Quests:** 13–16
**Boss:** The Red Banner Champion
**Story turn:** The enemy is exploiting political division.

### Act V — The Green Rot

A forest region becomes infected by supernatural corruption. Druids and rangers disagree about whether the land can be saved.

**Quests:** 17–20
**Boss:** Rotheart Treant
**Story turn:** The corruption is spreading through the ward network.

### Act VI — The Black Forge

The player enters an industrial fortress where corrupted weapons are being manufactured from fragments of broken wardstones.

**Quests:** 21–24
**Boss:** Forge Tyrant
**Story turn:** The enemy can weaponize the Circle itself.

### Act VII — The Veiled City

A hidden city reveals that surviving members of the ancient order have been manipulating events to restore their lost power.

**Quests:** 25–28
**Boss:** The Veiled Magister
**Story turn:** Not every guardian is an ally.

### Act VIII — War Beneath the World

The player descends into an underground civilization preparing for the return of the sealed entity.

**Quests:** 29–32
**Boss:** Deep-King Varkul
**Story turn:** The seal is also imprisoning an ancient civilization.

### Act IX — The Twelve Keys

The remaining wardstones must be recovered. Each key requires a different faction, dungeon, or regional alliance.

**Quests:** 33–36
**Boss:** The Keybearer
**Story turn:** The twelve wardstones form one mechanism.

### Act X — The Broken Circle

The final defenses fail. Major zones enter an emergency state and faction relationships determine which allies arrive.

**Quests:** 37–40
**Boss:** The Circlebreaker
**Story turn:** The player must choose whether to restore, control, or destroy the Circle.

### Act XI — The Last Descent

The player enters the sealed region beneath the world and confronts the source of the corruption.

**Quests:** 41–45
**Boss:** The Bound Ancient
**Story turn:** The enemy's motives are more complicated than the legends claimed.

### Act XII — A New Circle

The player resolves the campaign through one of three broad outcomes: restoration, liberation, or destruction of the Circle.

**Quests:** 46–50
**Final encounters:** The Circle Heart and the chosen faction opposition
**Story turn:** The world enters a new age based on the player's choices.

# Fifty Main Quests

## Act I

1. **A Crack in the Ward** — Investigate strange tremors near the settlement.
2. **Missing on the North Road** — Find three missing travelers.
3. **Corrupted Tracks** — Follow unnatural tracks into the hills.
4. **The Cracked Warden** — Defeat the guardian protecting the damaged wardstone.

## Act II

5. **Burned Wagons** — Investigate a destroyed caravan.
6. **Bandits in Ash** — Locate the marauders responsible.
7. **The Hidden Ledger** — Recover a ledger identifying their employer.
8. **Captain of the Ashroad** — Defeat the marauder captain.

## Act III

9. **Flooded Entrance** — Enter the drowned archive.
10. **Pages Beneath Water** — Recover three Circle records.
11. **The First Seal** — Discover the history of the first wardstone.
12. **Archivist of the Deep** — Defeat the corrupted archivist and recover the record key.

## Act IV

13. **A Kingdom Accuses** — Deliver evidence to the eastern council.
14. **A Kingdom Denies** — Investigate the western settlement.
15. **The Stolen Standard** — Prevent a false-flag attack.
16. **Champion of the Red Banner** — Defeat the champion or negotiate an alliance.

## Act V

17. **Rot in the Roots** — Investigate the forest corruption.
18. **Druid's Warning** — Gather rare cleansing components.
19. **The Ranger's Path** — Rescue rangers trapped behind corrupted growth.
20. **Rotheart** — Defeat the corrupted treant and cleanse the second ward.

## Act VI

21. **Black Iron** — Discover the source of corrupted weapons.
22. **Into the Forge** — Infiltrate the fortress.
23. **Break the Chain** — Free imprisoned workers and craftsmen.
24. **The Forge Tyrant** — Destroy the corrupted forge master.

## Act VII

25. **A City Without a Map** — Find the hidden city.
26. **The Veiled Council** — Gain access to the ancient order.
27. **The Guardian's Lie** — Expose the order's manipulation.
28. **The Veiled Magister** — Defeat or bargain with the order's leader.

## Act VIII

29. **Below the World** — Discover the underground civilization.
30. **The Deep King's Demand** — Negotiate with the underground ruler.
31. **The War Below** — Prevent a surface invasion.
32. **Varkul's Crown** — Defeat or ally with Deep-King Varkul.

## Act IX

33. **The Twelve Keys** — Identify the locations of the remaining keys.
34. **Key of Stone** — Recover the mountain key.
35. **Key of Tides** — Recover the sea key.
36. **Key of Stars** — Recover the final accessible key and locate the Circle Heart.

## Act X

37. **The Circle Burns** — Defend a major settlement during the collapse.
38. **Allies Answer** — Call in factions based on reputation.
39. **The Last Ward** — Hold the final surface ward.
40. **The Circlebreaker** — Defeat the enemy commander.

## Act XI

41. **The Sealed Descent** — Enter the forbidden chamber.
42. **Echoes of the First Age** — Recover memories from the original Circle builders.
43. **The Bound Ancient** — Learn the truth behind the sealed entity.
44. **The Choice of Chains** — Decide whether the ancient power should remain bound.
45. **The Last Descent** — Reach the Circle Heart.

## Act XII

46. **Restore the Circle** — Attempt to repair the ancient system.
47. **Break the Circle** — Destroy the ward network.
48. **Free the Bound** — Release the imprisoned power and accept the consequences.
49. **The Final Battle** — Defeat the final opposition generated by the player's choice.
50. **A New Circle** — Establish the post-campaign world state.

# Side-Quest System

Side quests should be generated or activated independently of the 50 main quests. They can provide lore, reputation, crafting materials, equipment, alternate solutions, and companion relationships.

Recommended side-quest categories:

- Settlement requests
- Faction missions
- Bounty hunts
- Rescue missions
- Investigation quests
- Collection quests
- Crafting quests
- Exploration quests
- Dungeon quests
- Companion quests
- Secret quests
- Repeatable contracts

Side quests may modify story flags but should not make the main campaign impossible to complete.

# Quest Data Model

A future implementation can represent quests as data records:

```text
QuestId
ActId
Title
Summary
QuestType
RecommendedLevel
StartZone
ObjectiveType
ObjectiveTarget
RequiredQuestIds
RequiredStoryFlags
RequiredFactionStanding
RewardXP
RewardGold
RewardItems
RewardReputation
CompletionFlags
FailureFlags
NextQuestIds
Optional
Repeatable
```

## Objective types

- TalkTo
- Kill
- Collect
- Escort
- Explore
- Interact
- Defend
- Craft
- Deliver
- Investigate
- DungeonClear
- BossDefeat
- Choice

# Narrative Choice System

Major decisions should create persistent flags instead of branching the entire codebase. Example flags:

- `circle_restored`
- `circle_destroyed`
- `ancient_freed`
- `eastern_alliance`
- `western_alliance`
- `deep_king_ally`
- `veil_order_exposed`
- `forest_saved`
- `forge_destroyed`

NPC dialogue, faction reactions, available side quests, rewards, and final encounters can query these flags.

# Implementation Plan

1. Add quest and story data structures without changing classic character storage.
2. Add a campaign state component/service.
3. Add quest activation and completion APIs.
4. Add story flag persistence.
5. Add NPC dialogue conditions.
6. Add zone and encounter hooks.
7. Add quest journal commands.
8. Add admin/debug commands for campaign state.
9. Add regression tests for quest prerequisites and completion.
10. Add optional UI/web client integration later.

The campaign layer must remain optional so a traditional CircleMUD server can run without loading the narrative system.