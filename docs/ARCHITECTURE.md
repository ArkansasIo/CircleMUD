# CircleMUD Architecture

## Runtime model

CircleMUD is a single server process organized as a set of C translation units. The build links the server from the object files listed by `src/Makefile.in`.

The major responsibilities are:

| Area | Representative modules | Responsibility |
|---|---|---|
| Communications | `comm.c`, `act.comm.c` | Network connections, descriptors, output, communication commands |
| Command interpreter | `interpreter.c` | Parse player commands and dispatch command handlers |
| World/database | `db.c`, `constants.c`, `config.c` | Load and maintain rooms, mobiles, objects, zones, and global configuration |
| Character actions | `act.*.c` | Player and NPC commands grouped by gameplay domain |
| Combat | `fight.c`, `act.offensive.c` | Combat state, attacks, damage, and offensive actions |
| Spells/magic | `magic.c`, `spells.c`, `spell_parser.c` | Spell definitions, casting, effects, and parsing |
| Objects/items | `handler.c`, `objsave.c`, `act.item.c` | Object manipulation, persistence, and item commands |
| NPC behavior | `mobact.c`, `spec_procs.c` | Mobile behavior and special procedures |
| Shops/houses | `shop.c`, `house.c` | Shops and player housing systems |
| World interaction | `graph.c`, `weather.c`, `limits.c` | Movement/pathing, weather, and character limits |
| Administration | `act.wizard.c`, `modify.c`, `olc.c` | Immortal commands, editing, and online creation |
| Utilities | `util/`, `random.c`, `utils.c` | Build-time tools and shared helpers |

## Build dependency flow

1. `configure` generates platform-specific build configuration.
2. `src/Makefile.in` becomes the source Makefile.
3. C modules compile into object files.
4. The linker creates `bin/circle`.
5. The same build invokes the utilities under `src/util`.

The existing Makefile deliberately keeps the object list explicit. Preserve that behavior when adding a module so legacy builds remain predictable.

## Safe modernization boundary

New systems should prefer isolated modules and explicit interfaces rather than broad rewrites of legacy globals. A practical sequence is:

`legacy subsystem -> characterization tests -> interface boundary -> new implementation -> compatibility adapter`

Do not introduce a new database, network API, scripting runtime, or persistent data format until the existing server behavior has regression coverage.

## Proposed future layers

### Core

- Event loop and descriptor management
- World state
- Character state
- Object state
- Persistence

### Gameplay services

- Combat
- Abilities/spells
- Quests
- Progression
- Crafting
- Guilds/parties

### Operational services

- Structured logging
- Metrics
- Audit events
- Moderation tools
- Backup/restore

### Optional external services

- Web status/API service
- Administration UI
- Account service
- Analytics

External services should remain optional so the classic text-MUD server can still run independently.
