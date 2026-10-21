# RPG 900 Content Runtime

CircleMUD now has a deterministic 900-record RPG content model and a native C runtime for consuming it.

## Distribution

- 150 weapons
- 150 armor pieces
- 200 items
- 150 consumables
- 100 crafting materials
- 100 monsters
- 50 NPCs

Total: **900 records**.

The catalog specification and Python generator remain in `data/rpg/catalog_900.json` and `scripts/tools/generate_catalog_900.py`. The C runtime exposes the same 900 records through `rpg_catalog_get()` without requiring a third-party JSON library.

## Runtime layers

`src/rpg/` now contains:

- `rpg_catalog.*` — 900-record deterministic catalog index.
- `rpg_item.*` — item instances, stacking, binding and equipment classification.
- `rpg_equipment.*` — 12 equipment slots and derived combat values.
- `rpg_character.*` — levels 1–720, XP, attributes and resources.
- `rpg_monster.*` — 100 monster records, tiers and world-difficulty scaling.
- `rpg_npc.*` — 50 NPC records, roles and faction association.
- `rpg_combat.*` — physical/elemental damage, mitigation and critical strikes.
- `rpg_loot.*` / `rpg_loot_drop.*` — rarity, affixes and monster drops.
- `rpg_crafting.*` — material recipes and validation.
- `rpg_ability.*` — active/passive/toggle/ultimate abilities.
- `rpg_skill.*` — proficiency, rank and mastery.
- `rpg_quest.*` — multi-objective quest state and progression.
- `rpg_world.*` — active world entities.
- `rpg_encounter.*` — level, party, tier and world-difficulty scaling.

## Build

`src/Makefile.in` compiles the RPG modules into the CircleMUD executable and links the math library required by the level-scaling formulas.

## Validation

`tests/run_rpg_catalog_runtime_test.sh` validates all 900 catalog indexes and exercises character, equipment, monster, combat and crafting paths. The GitHub Actions RPG workflow runs both the existing RPG tests and this integrated catalog test.
