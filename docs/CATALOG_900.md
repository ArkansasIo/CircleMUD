# CircleMUD 900-Entry RPG Content Catalog

The repository now defines a deterministic catalog of **900 gameplay records** generated from `data/rpg/catalog_900.json`.

## Distribution

| Category | Count |
|---|---:|
| Weapons | 150 |
| Armor | 150 |
| Items | 200 |
| Consumables | 150 |
| Materials | 100 |
| Monsters | 100 |
| NPCs | 50 |
| **Total** | **900** |

## Content coverage

- Weapons: swords, axes, maces, daggers, spears, polearms, bows, crossbows, staffs, wands, tomes, shields, greatblades, warhammers, and glaives.
- Armor: head, chest, shoulders, hands, legs, feet, cloak, bracers, belt, rings, amulets, and trinkets across cloth, leather, mail, plate, and cosmetic classes.
- Items: relics, charms, runes, totems, idols, sigils, lanterns, compasses, keys, banners, maps, gems, orbs, coins, and tokens.
- Consumables: potions, elixirs, tonics, flasks, rations, antidotes, scrolls, bombs, meals, and brews.
- Materials: ore, wood, cloth, leather, crystal, essence, herbs, scales, bone, and shards.
- Monsters: normal, elite, rare, champion, and world-boss tiers with spawn behavior and loot-table bindings.
- NPCs: merchants, trainers, quest givers, guards, and craftsmen tied to the major world factions.

## Scaling

Every generated record receives a deterministic level from 1–720 and a rarity from Common through Eternal. The generator does not use network services or third-party Python packages, so the same specification produces the same catalog on every machine.

## Generate the full record set

```text
python3 scripts/tools/generate_catalog_900.py
```

The generated file is `data/rpg/catalog_900.generated.json`.

## Validate

```text
python3 scripts/tools/validate_catalog_900.py
```

The validation script asserts that the configured distribution is exactly 900 records. The generated catalog is intended to feed the RPG item, equipment, world-population, loot, crafting, NPC, and encounter runtimes.
