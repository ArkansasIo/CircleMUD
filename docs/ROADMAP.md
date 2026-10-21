# AetherWorld Modernization Roadmap

## Mission

Maintain the legacy CircleMUD codebase as a buildable, documented text-MUD foundation while progressively adding AetherWorld RPG systems. Preserve upstream attribution and keep new gameplay isolated until it is tested.

## Phase 1 — Reproducible baseline

- [x] Linux GitHub Actions build workflow.
- [x] Document source layout and basic build entry point.
- [x] Compile the AetherWorld runtime modules through the main Makefile.
- [x] Add isolated RPG/AetherWorld regression tests.
- [ ] Record compiler warnings and distinguish known legacy warnings from regressions.

## Phase 2 — Campaign and progression

- [x] 12-act AetherWorld campaign registry.
- [x] 50 linked quests and prerequisite chain.
- [x] Level progression through level 720.
- [x] Quest availability and registry validation.
- [ ] Persist campaign progress in player save data through an explicit adapter.
- [ ] Add branching quest state and optional side objectives.
- [ ] Add quest rewards for equipment, abilities, faction reputation, and titles.

## Phase 3 — RPG systems

- [x] Existing RPG level, RNG, loot, encounter, and world-difficulty helpers.
- [ ] Character attribute and derived-stat layer.
- [ ] Classes, subclasses, talents, and specialization trees.
- [ ] Equipment slots, item affixes, rarity, set bonuses, and durability.
- [ ] Crafting, gathering, recipes, professions, and economy.
- [ ] Parties, guilds, faction reputation, and social progression.

## Phase 4 — World and endgame

- [ ] Data-driven zones, biomes, encounters, and dynamic events.
- [ ] Dungeons, raids, trials, and tower progression.
- [ ] World bosses and scalable encounter modifiers.
- [ ] Seasonal and account-wide progression.
- [ ] Post-campaign legacy/NG+ systems.

## Phase 5 — Developer experience

- [ ] Architecture diagrams and module lifecycle documentation.
- [ ] Data validation tools for world, quest, item, and encounter files.
- [ ] Deterministic content validation in CI.
- [ ] Structured logging and metrics.
- [ ] Safe builder/OLC workflows for AetherWorld content.

## Phase 6 — API and UI

- [ ] Versioned player, quest, world-map, and campaign APIs.
- [ ] Character sheet UI.
- [ ] Campaign and quest journal.
- [ ] World map and zone browser.
- [ ] Inventory/equipment/crafting interfaces.
- [ ] Party, guild, and social panels.

## Compatibility and licensing

AetherWorld is the new project identity, while CircleMUD remains the upstream engine attribution and compatibility reference. Do not remove or rewrite upstream credits and license notices. Review the included license materials before distributing modifications, binaries, or hosted services.

## Definition of done

A feature is complete only when its source compiles, its isolated behavior has regression coverage where practical, the relevant data is validated, and the documentation describes integration and compatibility boundaries.
