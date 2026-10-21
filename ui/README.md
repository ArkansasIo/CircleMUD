# AetherWorld UI Shell

The UI layer provides one application model with multiple presentations.

## Presentations

- `ui/gui/index.html` — launcher and navigation shell.
- `ui/gui/game-client.html` — primary browser client.
- `ui/gui/game-hud.html` — RPG HUD presentation.
- `ui/map/world-map.html` — world and zone map.
- `ui/api/client.js` — client API adapter.
- `ui/text/circlemud-ui.lua` — legacy text presentation retained for compatibility.

## AetherWorld direction

The new product identity is **AetherWorld**. Legacy CircleMUD terminology is retained where it refers to the upstream engine, file formats, protocol, or historical compatibility surface. New player-facing systems should use AetherWorld naming.

The UI should expose campaign acts, quest state, level progression, world difficulty, inventory, equipment, map, party, guild, crafting, and endgame systems through stable API contracts rather than coupling directly to the legacy server internals.
