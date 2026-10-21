# RPG Character Valuation Runtime

## Purpose

`rpg_character_value.c` provides the bridge between the live RPG character model and the formal PvE/PvP valuation system.

## Inputs

The adapter reads:

- level
- ten primary character attributes
- attack power
- spell power
- armor
- critical chance
- haste
- maximum health
- equipment valuation

The existing character model remains unchanged; valuation is exposed as a side-effect-free adapter.

## PvE/PvP separation

`rpg_character_rating(character, RPG_CONTEXT_PVE)` and
`rpg_character_rating(character, RPG_CONTEXT_PVP)` use the same character state but pass different contexts into the formal valuation model.

PvP therefore inherits the context-specific dampening and defensive weighting defined by `rpg_value.c` rather than duplicating those rules inside the character system.

## Calculation stages

1. Map character attributes into `rpg_value_stats`.
2. Convert combat resources into damage, healing, defensive, and utility values.
3. Apply the character level scale.
4. Add the complete equipment rating for the selected context.
5. Add defensive survival value.
6. Clamp the final result to a non-negative rating.

## Regression coverage

`tests/rpg_character_value_test.c` verifies:

- both PvE and PvP ratings are positive;
- PvE and PvP ratings are distinct;
- increasing character level increases effective PvE rating.

The test is included in `tests/run_rpg_tests.sh`.

## Next integration boundary

The character rating can now feed encounter budgets, boss scaling, combat normalization, loot quality, and party composition calculations without making the character object responsible for those systems.
