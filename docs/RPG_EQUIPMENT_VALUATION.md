# RPG Equipment Valuation Runtime

The equipment layer now consumes the formal PvE/PvP item valuation model instead of treating equipment power as a single context-free number.

## Runtime fields

`rpg_equipment` maintains:

- `attack_power`
- `spell_power`
- `armor`
- `critical`
- `haste`
- `movement`
- `pve_rating`
- `pvp_rating`

The legacy combat-facing aggregates remain available. The two rating fields are derived from the canonical `rpg_item_rating()` function for every equipped item.

## Rating flow

```text
catalog item
    -> rpg_item
    -> rpg_item_rating(PvE/PvP)
    -> rpg_equipment_recalculate()
    -> pve_rating / pvp_rating
```

`rpg_equipment_rating()` provides a context selector for callers that need one rating without duplicating the PvE/PvP branching logic.

## Context behavior

PvE and PvP ratings intentionally differ. The formal value model applies PvP dampening to offensive, healing, crowd-control, and cooldown-related effects, so an item can have a different effective rating in player-versus-player content than in player-versus-environment content.

## Recalculation semantics

Ratings are recalculated whenever an item is equipped or unequipped. The operation is deterministic and does not mutate item state. Empty equipment has zero PvE/PvP rating.

## Regression coverage

`tests/rpg_equipment_value_test.c` verifies:

1. catalog-backed weapon and armor initialization;
2. equipment insertion and removal;
3. legacy attack/armor aggregation;
4. positive PvE/PvP ratings;
5. context-specific rating selection; and
6. different PvE and PvP results.

The test is included by `tests/run_rpg_tests.sh`.
