# RPG ↔ CircleMUD Character Bridge

The RPG runtime now has a live adapter to CircleMUD's authoritative `struct char_data` player state.

## Data flow

```text
CircleMUD character_list
        |
        v
rpg_circlemud_sync_players()
        |
        v
rpg_live_register_player()
        |
        v
rpg_character
        |
        v
rpg_runtime_tick()
```

The bridge is called from the existing RPG heartbeat hook. It does not modify `comm.c`; the existing build-time heartbeat redirection remains the integration point.

## Player identity

The bridge uses `char_specials.saved.idnum` as the RPG player identifier. NPCs are ignored by the player bridge.

## Synchronized state

CircleMUD remains authoritative for the legacy character sheet. Each heartbeat copies:

- level
- experience
- STR / DEX / CON / INT / WIS / CHA
- attack power from damage roll
- armor
- current/max hit points
- current/max mana
- current/max movement as RPG stamina

The RPG runtime can therefore calculate combat, valuation, encounter scaling, loot and other systems against the same live player state used by CircleMUD.

## Lifecycle

`rpg_circlemud_sync_character()` registers a player on first observation and refreshes the RPG representation on later heartbeats.

`rpg_circlemud_unsync_character()` removes a player from the RPG runtime and is intended for logout/extraction integration.

## Persistence boundary

The bridge deliberately does **not** change the CircleMUD player-file structures. The legacy playerfile ABI remains intact. RPG-specific persistent fields should be added through the existing spare-field strategy or a separate RPG persistence store rather than changing `char_file_u` layout.

## Next integration points

1. Call `rpg_circlemud_unsync_character()` from the player extraction/logout path.
2. Map `carrying` and `equipment[]` into RPG inventory/equipment slots.
3. Map `in_room` and room/zone metadata into RPG world entities.
4. Map CircleMUD mobile instances into RPG monster instances.
5. Push RPG XP, health and loot results back through controlled gameplay APIs instead of directly replacing legacy combat state.
