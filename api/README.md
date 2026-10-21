# CircleMUD Application API

The application API is the boundary between the CircleMUD authoritative C engine and the desktop/web/terminal user interfaces.

## Layers

```text
GUI / Text UI / Loading UI
          |
          v
     Application API
          |
   +------+------+
   |             |
   v             v
Game Engine    MySQL Adapter
   |
   v
Lua Gameplay SDK
```

## API domains

- `/api/v1/system` — identity, version, build, health, uptime
- `/api/v1/session` — login/session lifecycle
- `/api/v1/player` — character/profile operations
- `/api/v1/world` — zones, entities, biomes
- `/api/v1/quests` — quest state and objectives
- `/api/v1/combat` — validated combat requests
- `/api/v1/inventory` — inventory/equipment
- `/api/v1/factions` — faction state
- `/api/v1/admin` — development/admin operations

The HTTP/API layer must never directly mutate authoritative game state. Requests are validated and converted into engine commands.

## System response

```json
{
  "ok": true,
  "api": "circlemud",
  "version": "0.1.0",
  "channel": "pre-alpha",
  "build_id": "ARK-CIRCLE-GE-0.1.0-PA-0001"
}
```

## UI targets

The same API contract supports:

1. Terminal/text interface.
2. Desktop GUI.
3. Browser administration/development UI.
4. Boot/loading screen.
5. In-game loading/progress screen.

No database credentials or secrets belong in the client UI.
