#ifndef CIRCLEMUD_RPG_CIRCLEMUD_BRIDGE_H
#define CIRCLEMUD_RPG_CIRCLEMUD_BRIDGE_H

struct char_data;

/* Synchronize one live CircleMUD player into the RPG runtime. */
int rpg_circlemud_sync_character(struct char_data *ch);

/* Remove a CircleMUD player from the RPG runtime. */
int rpg_circlemud_unsync_character(struct char_data *ch);

/* Synchronize all currently linked CircleMUD player characters. */
void rpg_circlemud_sync_players(void);

/* Lifecycle hook for extraction/logout code paths. Safe to call more than once. */
void rpg_circlemud_on_character_extracted(struct char_data *ch);

#endif
