# AetherWorld Source Map

## Runtime boundary

AetherWorld is intentionally isolated from the legacy CircleMUD command loop until each subsystem has regression coverage. This keeps the classic server behavior stable while allowing new RPG features to be introduced incrementally.

```text
src/aetherworld.h
      |
      +--> src/aetherworld.c
      |      - initialization
      |      - XP progression
      |      - quest availability
      |      - registry validation
      |
      +--> src/aetherworld_story.c
             - 12 campaign acts
             - 50 linked quests

src/rpg/
  - level calculations
  - encounter scaling
  - loot
  - RNG
  - world difficulty
```

## Public API

`aetherworld_init()` initializes the runtime registry.

`aetherworld_get_act(id)` returns one of the twelve campaign acts.

`aetherworld_get_quest(id)` returns one of the fifty quest definitions.

`aetherworld_validate()` verifies quest IDs, act references, level bounds, and prerequisite ordering.

`aetherworld_required_xp(level)` returns the cumulative XP threshold used by the new AetherWorld progression layer.

`aetherworld_level_from_xp(xp)` converts cumulative XP to a level between 1 and 720.

`aetherworld_quest_available(quest_id, level, completed_quest_id)` evaluates the simple prerequisite chain used by the campaign registry.

## Integration rule

Do not directly mutate the legacy player structures from this module. Future integration should add an explicit adapter that translates AetherWorld progression state into the server's character state, with save/load and multiplayer tests before activation.

## Testing

`tests/aetherworld_runtime_test.c` exercises the public API without launching a network server or accessing production player files.
