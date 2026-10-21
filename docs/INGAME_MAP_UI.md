# In-Game Map and GUI

CircleMUD now has a map/UI prototype modeled after the usability patterns of modern MMORPG world maps while using original CircleMUD presentation and world data.

## Map features

- World regions and biome labels.
- Roads and travel paths.
- Capital, village, and dungeon markers.
- Player position marker.
- Clickable locations.
- Pan/drag navigation.
- Mouse-wheel zoom.
- Zoom controls and reset.
- Minimap panel.
- Location inspector.
- Map legend.
- Navigation shortcuts.

## Intended runtime architecture

```text
CircleMUD World Registry
        |
        v
   World Map API
        |
   +----+----+
   |         |
 Text UI    GUI
   |         |
   +----+----+
        |
      Map UI
```

The client should consume map data from `/api/v1/world/map` rather than owning authoritative world state. The server remains authoritative for player coordinates, discovered locations, zone transitions, quests, and access restrictions.

## Future map layers

- Continent view
- Region view
- Zone view
- Sub-zone view
- Dungeon/raid maps
- Quest objectives
- NPC markers
- Flight/transport paths
- Player/group/party markers
- PvP and faction territories
- Fog of war
- Discovery state
- Dynamic events
- Instance entrances
- Resource nodes
- Level-range overlays
