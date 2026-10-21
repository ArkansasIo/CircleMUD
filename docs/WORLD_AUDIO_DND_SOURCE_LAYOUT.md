# World, Audio and Rules Source Layout

This is the implementation layout for the systems added in the world-building phase.

```text
CircleMUD/
├── data/
│   ├── world/
│   │   ├── continents.json
│   │   ├── regions.json
│   │   ├── biomes.json
│   │   ├── subbiomes.json
│   │   ├── zones.json
│   │   ├── subzones.json
│   │   ├── landmarks.json
│   │   └── encounters.json
│   ├── audio/
│   │   ├── music.json
│   │   ├── ambience.json
│   │   ├── sfx.json
│   │   └── zone_audio.json
│   └── rules/
│       ├── rulesets.json
│       ├── dnd5e_2014.json
│       ├── dnd5e_2024.json
│       ├── dice.json
│       ├── conditions.json
│       ├── skills.json
│       ├── spells.json
│       └── damage_types.json
│
├── assets/
│   └── audio/
│       ├── music/
│       ├── ambience/
│       └── sfx/
│
├── src/
│   ├── world/
│   │   ├── biome.h/.c
│   │   ├── zone.h/.c
│   │   ├── subzone.h/.c
│   │   ├── encounter.h/.c
│   │   └── world_state.h/.c
│   ├── audio/
│   │   ├── audio_events.h/.c
│   │   ├── music_registry.h/.c
│   │   ├── ambience_registry.h/.c
│   │   └── sfx_registry.h/.c
│   └── rules/
│       ├── ruleset.h/.c
│       ├── dnd5e.h/.c
│       ├── dnd5e_dice.h/.c
│       ├── dnd5e_checks.h/.c
│       ├── dnd5e_combat.h/.c
│       ├── dnd5e_conditions.h/.c
│       ├── dnd5e_spells.h/.c
│       ├── dnd5e_actions.h/.c
│       └── dnd5e_monsters.h/.c
│
└── docs/
    ├── WORLD_BIOMES_ZONES.md
    ├── AUDIO_MUSIC_SYSTEM.md
    ├── DND5E_RULES_INTEGRATION.md
    └── WORLD_AUDIO_DND_SOURCE_LAYOUT.md
```

## Server/client boundary

The CircleMUD server should send semantic state:

```text
ZONE_ENTERED zone_id
BIOME_CHANGED biome_id
WEATHER_CHANGED weather_id
AUDIO_MUSIC music_id
AUDIO_AMBIENCE ambience_id
AUDIO_SFX sfx_id
COMBAT_STATE state
RULESET ruleset_id
```

The client resolves these IDs to presentation assets.

## No hard dependency on audio

The server must build and run without an audio library. Audio is a client concern.

## No hard dependency on D&D

D&D-compatible mechanics are selected by ruleset. Legacy CircleMUD mechanics remain available.

## Data-first rule

World, audio, and rules content belongs in data files whenever practical. C code provides validation, state transitions, event dispatch, and deterministic calculations.
