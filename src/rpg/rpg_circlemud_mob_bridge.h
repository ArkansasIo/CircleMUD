#ifndef CIRCLEMUD_RPG_CIRCLEMUD_MOB_BRIDGE_H
#define CIRCLEMUD_RPG_CIRCLEMUD_MOB_BRIDGE_H

struct char_data;

/* Synchronize a CircleMUD NPC prototype into the RPG world archetype layer. */
int rpg_circlemud_sync_mob_prototype(struct char_data *mob);

/* Synchronize all currently linked prototype NPCs by VNUM. */
void rpg_circlemud_sync_mob_prototypes(void);

#endif
