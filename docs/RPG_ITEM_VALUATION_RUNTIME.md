# RPG Item Valuation Runtime

## Purpose

The RPG item layer now exposes a catalog-driven valuation function that uses the formal PvE/PvP value model in `src/rpg/rpg_value.c`.

## API

```c
double rpg_item_rating(const rpg_item *item, rpg_value_context context);
```

The function maps catalog data into the formal value model:

- Weapons use catalog `power` as damage.
- Armor uses catalog `defense` as armor and catalog `power` as a health contribution.
- Other catalog entries use catalog `power` as health and a small value-derived bonus.
- Rarity contributes a deterministic 10% scaling step per rarity tier.
- PvP valuation uses the existing PvP dampening and defensive weighting rules.

## Runtime boundary

`rpg_item_rating()` is intentionally read-only. It does not mutate item durability, stack size, binding, or catalog state. This makes it safe for equipment previews, loot comparison, inventory UI, vendor pricing, and server-side validation.

## Regression coverage

`tests/rpg_item_value_test.c` verifies initialization, equipment classification, and positive PvE/PvP ratings. `tests/run_rpg_tests.sh` executes the test as part of the RPG runtime suite.
