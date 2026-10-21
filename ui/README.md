# CircleMUD UI Shell

The UI layer provides one application model with multiple presentations.

## Boot sequence

```text
[0%] Starting CircleMUD
[10%] Loading configuration
[25%] Initializing engine
[40%] Initializing database adapter
[55%] Loading world registry
[70%] Loading Lua gameplay modules
[85%] Validating game data
[95%] Starting services
[100%] Ready
```

## Loading screen

The loading screen displays:

- application name
- developer
- version
- pre-alpha channel
- build ID
- current subsystem
- progress percentage
- diagnostic status

## Text UI

```text
+--------------------------------------------------+
| CIRCLEMUD GAME ENGINE 0.1.0 PRE-ALPHA           |
| ArkansasIo | ARK-CIRCLE-GE-0.1.0-PA-0001        |
+--------------------------------------------------+
| 1 World     2 Character     3 Quests             |
| 4 Inventory 5 Abilities     6 Factions           |
| 7 Combat    8 Settings      9 System             |
+--------------------------------------------------+
| >                                               |
+--------------------------------------------------+
```

## GUI shell

The GUI uses the same API models as the text UI:

- top application/title bar
- left navigation/workspace panel
- central game/world viewport
- right inspector/status panel
- bottom event/log console
- modal loading and connection states

The visual implementation can be replaced without changing engine APIs.

## States

`BOOT -> LOADING_CONFIG -> LOADING_DATABASE -> LOADING_WORLD -> LOADING_SCRIPTS -> READY -> RUNNING -> SHUTDOWN`
