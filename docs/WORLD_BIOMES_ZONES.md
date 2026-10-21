# CircleMUD World Biome, Zone and Sub-Zone System

This defines an original fantasy world framework for the 12-act campaign. It uses Warcraft-style MMO geographic scale as inspiration while using original names and content.

## World hierarchy

```text
World
└── Continent
    └── Region
        └── Biome
            └── SubBiome
                └── Zone
                    └── SubZone
                        └── Landmark
                            └── Dungeon / Raid / Encounter
```

## Biome registry

| Biome | Sub-biomes | Example zones |
|---|---|---|
| Temperate Forest | Pinewood, Old Growth, Mistwood, Enchanted Grove | Greenvale, Whispering Woods |
| Tropical Jungle | Rainforest, Fungal Jungle, Mangrove, Rootway | Harandar-inspired Rootwild, Emerald Basin |
| Plains | Grassland, Savannah, Steppe, Wildflower | Golden Plains, Horsewind |
| Desert | Sand Sea, Badlands, Salt Flats, Oasis | Sunscar, Red Dunes |
| Tundra | Snowfield, Glacier, Ice Cave, Frozen Coast | Frostmarch, White Crown |
| Taiga | Conifer Forest, Frozen Marsh, Alpine Wood | Wolfpine, Northwatch |
| Mountain | Alpine, Volcanic, Cliffside, High Pass | Stormpeak, Ironfang |
| Swamp | Bog, Fen, Mire, Mangrove | Blackfen, Drowned Reach |
| Coast | Beach, Cliffs, Tidepool, Coastal Forest | Azure Coast, Stormshore |
| Ocean | Open Sea, Deep Sea, Reef, Abyss | Sapphire Expanse, Abyssal Trench |
| Underground | Cavern, Deep Road, Crystal Cave, Rootway | Underdeep, Stone Labyrinth |
| Volcanic | Lava Field, Ashland, Obsidian, Magma Cavern | Cinderfall, Black Forge |
| Arcane | Ley Field, Mana Forest, Floating Isles, Crystal Waste | Aetherreach |
| Void | Voidstorm, Shadow Waste, Riftland, Null Zone | The Shattered Sky |
| Feywild | Moonwood, Dream Meadow, Thorn Court, Fairy Ring | Moonveil |
| Shadowlands | Soul Marsh, Grave Plain, Spirit Forest, Bone Desert | Mourning Vale |
| Celestial | Starfield, Sky Temple, Radiant Isle, Cloud Sea | Astral Crown |
| Ruins | Fallen City, Battlefield, Ancient Temple, Buried Kingdom | Old Empire |

## 12 campaign regions

### Act I — Greenvale Frontier

- Greenvale Village
- North Road
- Whispering Woods
- Broken Ward Hills
- Cracked Warden Shrine

### Act II — Ashroad March

- Ashroad
- Burned Caravan Grounds
- Marauder Camp
- Cinder Canyon
- Ashroad Fortress

### Act III — Drowned Archive

- Flooded Marsh
- Sunken Causeway
- Drowned Archive
- Lower Stacks
- Archivist Vault

### Act IV — Twin Kingdoms

- Eastern Crownlands
- Western Crownlands
- Border March
- Red Banner Field
- Council Citadel

### Act V — Rotwild

- Rotwild Forest
- Druid Grove
- Ranger Heights
- Root Caverns
- Rotheart Sanctuary

### Act VI — Black Forge

- Ironfang Mountains
- Smelter Pass
- Black Iron Mines
- Forgeworks
- Tyrant's Furnace

### Act VII — Veiled City

- Mistwood
- Hidden Road
- Veiled Gate
- Lower City
- Magister Spire

### Act VIII — Underdeep

- Deep Road
- Stone Labyrinth
- Fungal Caverns
- Deep Kingdom
- Varkul's Throne

### Act IX — Twelve Keys

- Stormpeak
- Sapphire Expanse
- Starfall Observatory
- Ancient Key Sanctums
- Circle Heart Approach

### Act X — Broken Circle Warfront

- Burning Frontier
- Allied Camp
- Last Ward
- Circle Battlefield
- Circlebreaker Citadel

### Act XI — Sealed Depths

- Forbidden Descent
- First Age Ruins
- Memory Vault
- Bound Chamber
- Ancient Prison

### Act XII — New Circle

- Circle Heart
- Restoration Sanctum
- Liberation Rift
- Destruction Crucible
- New Dawn Citadel

## Zone types

```text
STARTER
SETTLEMENT
WILDERNESS
DUNGEON
RAID
BATTLEGROUND
CAPITAL
FACTION
QUEST
WORLD_BOSS
SECRET
PVP
CRAFTING
RESOURCE
STORY
ENDGAME
```

## Sub-zone types

```text
Village
Town
City
Capital
Camp
Road
Pass
Forest
Grove
Cave
Mine
Ruins
Temple
Fortress
Castle
Tower
Arena
Harbor
Farm
Shrine
DungeonWing
RaidWing
BossRoom
QuestArea
HiddenArea
```

## Zone metadata

```text
ZoneId
Name
RegionId
BiomeId
SubBiomeId
LevelMin
LevelMax
RecommendedPartySize
ZoneType
Faction
Climate
WeatherProfile
MusicTheme
AmbientProfile
EncounterTable
ResourceTable
QuestTable
DungeonId
WorldBossId
PvPEnabled
FastTravelNode
DiscoveryReward
```

## Dynamic world state

Zones can change according to campaign state:

```text
Peaceful
Threatened
Corrupted
Occupied
Liberated
Besieged
Reclaimed
Destroyed
Restored
```

The state can alter NPCs, mobs, quests, vendors, music, weather, lighting, and available sub-zones.

## Encounter ecology

Each biome should define creature families, resource nodes, weather, and ambient events.

Example:

```text
Rotwild Forest
  Creatures: wolves, corrupted beasts, treants, spiders
  Resources: herbs, hardwood, mushrooms
  Weather: rain, fog, storm
  Events: ranger rescue, corruption surge, rare beast
  Boss: Rotheart
```

## Dungeon tiers

```text
Tier 1  Story Dungeon
Tier 2  Veteran Dungeon
Tier 3  Heroic Dungeon
Tier 4  Mythic Dungeon
Tier 5  Mythic+ style scalable dungeon
Tier 6  Raid Wing
Tier 7  Endgame Raid
```

## World map implementation

The world map should use stable IDs rather than display names:

```text
continent_id
region_id
biome_id
subbiome_id
zone_id
subzone_id
room_vnum
```

This allows the legacy CircleMUD room/VNUM system to remain authoritative while the RPG layer adds higher-level geography.
