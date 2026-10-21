# AetherWorld — ArkansasIo

AetherWorld is the modernized RPG/MUD project built on the legacy CircleMUD 3.1 codebase. The repository keeps the upstream engine, credits, and license materials intact while adding an incremental, data-driven RPG layer.

## Project status

The classic C / Make-based server remains the compatibility foundation. AetherWorld now includes an isolated runtime module for campaign progression, a 12-act story, 50 linked quests, level progression through 720, and regression tests. Legacy gameplay remains unchanged unless a new system is explicitly integrated into the command/game loop.

## Build on Linux

A typical build uses a POSIX-compatible environment and a C toolchain:

```sh
cd src
../configure
make
```

Read `doc/README.UNIX` and the original `README` for platform-specific details. The source is historically old, so modern compilers may expose compatibility warnings; CI treats an actual build failure as a failure rather than assuming warnings are harmless.

## RPG runtime tests

Run the isolated regression suite with:

```sh
sh tests/run_rpg_tests.sh
```

The suite covers the existing RPG helpers plus the AetherWorld campaign registry and progression API. Tests use no production player files.

## AetherWorld systems

- 12-act campaign progression.
- 50 main and side quests with prerequisite chains.
- Level range 1–720.
- Deterministic XP progression helpers.
- Quest availability validation.
- Data-driven story and quest definitions separated from the legacy server loop.
- Existing RPG difficulty, encounter, loot, and world systems remain available under `src/rpg/`.

## CI

GitHub Actions runs the Linux build and isolated RPG/AetherWorld regression tests. The workflows do not deploy a public game server or publish player data.

## Documentation

- [Modernization roadmap](docs/ROADMAP.md)
- [AetherWorld RPG source map](docs/AETHERWORLD_SOURCE_MAP.md)
- [Story campaign](docs/STORY_CAMPAIGN.md)
- [RPG master systems](docs/RPG_MMO_MASTER_SYSTEMS.md)
- Original documentation: `doc/`
- Original project notes: `README`, `ChangeLog`, and `FAQ`

## Development principles

1. Preserve CircleMUD attribution and the original license notices.
2. Keep gameplay changes incremental and testable.
3. Keep new AetherWorld systems isolated until their integration is regression-tested.
4. Avoid committing player data, passwords, generated binaries, or local runtime configuration.
5. Treat an internet-facing server as untrusted input: validate protocol data and apply operational security controls before public deployment.
