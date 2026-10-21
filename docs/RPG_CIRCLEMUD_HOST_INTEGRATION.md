# RPG ↔ CircleMUD Host Integration

## Architecture

The RPG runtime treats CircleMUD as the authoritative legacy host. RPG combat events cross the boundary through four stages:

1. `rpg_circlemud_combat_bridge` queues typed events.
2. `rpg_circlemud_combat_transaction` validates the event against the live RPG snapshot.
3. `rpg_circlemud_host_adapter` requires explicit host callbacks.
4. `rpg_circlemud_host_native` provides native CircleMUD callbacks for damage, death, XP, and mapped loot.

`rpg_circlemud_host_dispatch_queue()` combines transaction validation and host application while preserving the queue if any event fails.

## Native CircleMUD operations

### Damage

The native damage callback resolves the RPG monster's source key to the current CircleMUD NPC and calls the existing `damage()` combat entry point. CircleMUD's `damage()` return value of `-1` means the hit killed the target; that is considered a successful host mutation because the native death/extraction lifecycle owns the result.

### Death

A death event resolves the current NPC and applies lethal native damage. If CircleMUD has already placed the NPC at or below `POS_DEAD`, the event is considered already satisfied.

### Experience

XP is granted through CircleMUD's existing `gain_exp()` path so level advancement and legacy character bookkeeping remain in the host implementation.

### Loot

Loot uses the explicit RPG-catalog-to-CircleMUD-VNUM reverse mapping. The object is created with `read_object(..., VIRTUAL)` and transferred with `obj_to_char()`.

Only mapped catalog entries can cross this boundary. Unmapped RPG catalog items remain RPG-only.

## NPC identity

The legacy `char_data` structure does not provide a stable persistent RPG instance ID. The mob bridge derives a source key from prototype RNUM/VNUM, room, and deterministic occurrence order. The resolver returns a borrowed `char_data *` only for the duration of a host callback; no legacy pointer is stored in RPG state.

This is an integration-stage identity mechanism, not a persistent database identity. A future CircleMUD instance-ID field can replace it without changing the RPG event schema.

## Queue semantics

The host dispatch is fail-closed:

- invalid events are not applied;
- unavailable host callbacks are not applied;
- failed events remain queued;
- the queue is cleared only after every queued event succeeds.

This prevents a partial event batch from being silently acknowledged.

## Build

The following RPG host integration objects are part of `src/Makefile.in`:

- `rpg_circlemud_host_adapter.o`
- `rpg_circlemud_host_resolver.o`
- `rpg_circlemud_host_native.o`
- `rpg_circlemud_host_dispatch.o`

## Testing

The standalone RPG test runner includes combat bridge, transaction, and consumer regression coverage. Native host callbacks require a full CircleMUD build because they depend on `char_data`, `damage()`, `gain_exp()`, `read_object()`, and `obj_to_char()`.

No local compiler result is implied by repository changes alone; CI or a local CircleMUD build must validate the complete host-linked target.
