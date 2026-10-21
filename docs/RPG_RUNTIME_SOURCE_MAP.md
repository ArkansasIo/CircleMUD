# RPG Runtime Source Map

The RPG/MMORPG layer now has an executable C runtime foundation instead of being data-only.

## Runtime modules

```text
src/rpg/
├── rpg_types.h
├── rpg_level.h/.c
├── rpg_world_difficulty.h/.c
├── rpg_rng.h/.c
├── rpg_loot.h/.c
├── rpg_encounter.h/.c
├── rpg_world.h/.c
└── rpg_system.h/.c
```

## Responsibilities

- `rpg_types` — shared enums and runtime records.
- `rpg_level` — level validation, XP curve, power scaling and milestones for levels 1–720.
- `rpg_world_difficulty` — nine open-world difficulty tiers.
- `rpg_rng` — deterministic seeded RNG for reproducible tests and loot rolls.
- `rpg_loot` — rarity selection, affix counts and rarity power multipliers.
- `rpg_encounter` — participant/average-level/tier/difficulty scaling.
- `rpg_world` — active world entities and dynamic scaling.
- `rpg_system` — lifecycle and global world-difficulty coordinator.

## World difficulty

World difficulty is independent of character level and instanced-content difficulty.

```text
1 Frontier
2 Dangerous
3 Hostile
4 Brutal
5 Deadly
6 Cataclysmic
7 Mythic World
8 Apocalypse
9 Eternal Nightmare
```

The existing five dungeon difficulties remain independent: Normal, Veteran, Hard, Nightmare and Ultimate Nightmare.

## Runtime formula

```text
EncounterPower = LevelPower × MobTier × WorldDifficulty × PlayerScale
```

The runtime keeps authoritative encounter values in C while JSON remains the balance/data source and Lua remains the controlled scripting layer.

## Testing

Run:

```sh
sh tests/run_rpg_tests.sh
```

The test suite validates the 1–720 level bounds, XP progression, milestone behavior, world-difficulty bounds, world scaling, deterministic RNG and loot rarity selection.
