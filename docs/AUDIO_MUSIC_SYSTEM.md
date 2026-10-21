# CircleMUD Audio, Sound Effect and Music System

## Goal

Define an optional audio layer for a future web, desktop, or game client. The classic text client remains fully functional without audio.

## Audio hierarchy

```text
AudioSystem
├── Music
│   ├── Title
│   ├── Exploration
│   ├── Combat
│   ├── Dungeon
│   ├── Raid
│   ├── Boss
│   ├── Faction
│   ├── City
│   └── Ending
├── Ambience
│   ├── Forest
│   ├── Desert
│   ├── Mountain
│   ├── Swamp
│   ├── Ocean
│   ├── Underground
│   ├── Arcane
│   └── Void
└── SFX
    ├── Combat
    ├── Magic
    ├── UI
    ├── Environment
    ├── Movement
    ├── Crafting
    └── Quest
```

## Original title-screen theme

**Theme:** `THE_BROKEN_CIRCLE_TITLE`

Concept:

- 72–96 BPM
- low orchestral drone
- distant bell motif
- strings introduce the Circle motif
- percussion enters slowly
- final unresolved chord leading into the game

Suggested structure:

```text
Intro 0:00–0:20
Motif 0:20–0:55
Build 0:55–1:25
Main Theme 1:25–2:10
Outro 2:10–2:30
```

This is an original composition specification; no copyrighted game soundtrack is required.

## In-game music themes

| ID | Theme | Trigger |
|---|---|---|
| MUSIC_GREENVALE | Greenvale Dawn | starter zone exploration |
| MUSIC_ASHROAD | Ashroad March | Act II |
| MUSIC_ARCHIVE | Drowned Memory | Act III |
| MUSIC_KINGDOM | Two Crowns | Act IV |
| MUSIC_ROTWILD | Roots of Decay | Act V |
| MUSIC_FORGE | Black Iron | Act VI |
| MUSIC_VEILED | City Behind Mist | Act VII |
| MUSIC_UNDERDEEP | Beneath the World | Act VIII |
| MUSIC_KEYS | Twelve Keys | Act IX |
| MUSIC_WARFRONT | Broken Circle War | Act X |
| MUSIC_DESCENT | Last Descent | Act XI |
| MUSIC_NEWCIRCLE | A New Circle | Act XII |

## Combat themes

```text
COMBAT_MINOR
COMBAT_ELITE
COMBAT_BOSS
COMBAT_WORLD_BOSS
COMBAT_DUNGEON
COMBAT_RAID
COMBAT_FINAL
```

## Ambient sound profiles

### Forest

- wind through leaves
- birds
- insects
- distant animals
- branches
- rain

### Mountain

- wind gusts
- rock movement
- distant thunder
- cave echoes

### Desert

- wind
- sand movement
- distant wildlife
- heat shimmer ambience

### Swamp

- frogs
- insects
- bubbling water
- reeds
- distant creatures

### Ocean

- waves
- gulls
- wind
- water movement
- ship creaks

### Underground

- dripping water
- stone resonance
- distant creatures
- cavern wind

### Arcane

- low magical hum
- crystal resonance
- energy pulses
- ethereal particles

### Void

- reversed-feeling drones
- low rumbles
- distant whispers
- unstable energy pulses

## Sound-effect registry

### Combat

```text
SFX_SWORD_SWING
SFX_SWORD_HIT
SFX_AXE_SWING
SFX_AXE_HIT
SFX_MACE_HIT
SFX_DAGGER_STRIKE
SFX_ARROW_RELEASE
SFX_ARROW_HIT
SFX_SHIELD_BLOCK
SFX_PARRY
SFX_CRITICAL_HIT
SFX_DODGE
SFX_DEATH
```

### Magic

```text
SFX_SPELL_CAST
SFX_SPELL_IMPACT
SFX_HEAL
SFX_BUFF
SFX_DEBUFF
SFX_FIRE
SFX_FROST
SFX_ARCANE
SFX_NATURE
SFX_SHADOW
SFX_HOLY
SFX_VOID
SFX_SUMMON
```

### World

```text
SFX_DOOR_OPEN
SFX_DOOR_CLOSE
SFX_CHEST_OPEN
SFX_LEVER
SFX_SWITCH
SFX_WATER
SFX_FIREPLACE
SFX_WIND
SFX_THUNDER
SFX_EARTHQUAKE
SFX_PORTAL
```

### UI

```text
SFX_UI_CLICK
SFX_UI_CONFIRM
SFX_UI_CANCEL
SFX_UI_ERROR
SFX_QUEST_ACCEPT
SFX_QUEST_COMPLETE
SFX_LEVEL_UP
SFX_LOOT_RARE
SFX_LOOT_EPIC
SFX_LOOT_LEGENDARY
```

## Dynamic music state machine

```text
EXPLORE
  -> ALERT
  -> COMBAT
  -> VICTORY
  -> EXPLORE

EXPLORE
  -> DUNGEON
  -> BOSS_WARNING
  -> BOSS
  -> VICTORY

EXPLORE
  -> STORY
  -> CINEMATIC
  -> EXPLORE
```

Music transitions should use short crossfades instead of abrupt track changes.

## Zone audio configuration

```text
zone_id
music_theme
combat_theme
boss_theme
ambient_profile
weather_profile
reverb_profile
music_volume
ambient_volume
sfx_volume
voice_volume
```

## Audio events

```text
AUDIO_ENTER_ZONE
AUDIO_ENTER_SUBZONE
AUDIO_START_COMBAT
AUDIO_END_COMBAT
AUDIO_START_BOSS
AUDIO_END_BOSS
AUDIO_QUEST_ACCEPT
AUDIO_QUEST_COMPLETE
AUDIO_LEVEL_UP
AUDIO_LOOT
AUDIO_WEATHER_CHANGE
AUDIO_STORY_EVENT
AUDIO_FACTION_EVENT
```

## Client API

A future client can consume server events:

```text
AUDIO PLAY <theme>
AUDIO STOP <theme>
AUDIO CROSSFADE <theme> <milliseconds>
AUDIO SFX <effect>
AUDIO AMBIENCE <profile>
AUDIO VOLUME <bus> <value>
```

The server should send semantic audio events, not raw audio files. The client maps IDs to locally hosted or licensed assets.

## Asset layout

```text
assets/audio/
  music/title/
  music/world/
  music/combat/
  music/dungeon/
  music/raid/
  music/boss/
  music/story/
  ambience/forest/
  ambience/desert/
  ambience/mountain/
  ambience/swamp/
  ambience/ocean/
  ambience/underground/
  ambience/arcane/
  ambience/void/
  sfx/combat/
  sfx/magic/
  sfx/world/
  sfx/ui/
  sfx/quest/
```

All shipped audio should be original, public-domain, or appropriately licensed.
